##  说明：
今天做嵌入式项目时，遇到了需要将utf-8编码方式转码为gbk方式的问题。本来是打算使用C++11提供的codecvt来实现转码任务的，可是其嵌入式编译器不支持C++；然后想到了C语言标准的iconv库，将其下载下来编译成静态库超过了1MB内存，因嵌入式系统存储空间优先，所以只能放弃；本来是打算自己来编写utg-8转unicode再转gbk的库，可是在unicode转gbk时，是没有任何规则的，只能通过查表来计算；然后我想到了将iconv库中的表格和相应的查找函数一直过来。

###  文件格式说明
- libiconv
  - export
    - iconv.h
  
  - src
    - ascii.h
    - ascii.c
    - utf8.h
    - utf8.c
    - iconv.h
    - iconv.c
    - ces_gbk.h
    - ces_gbk.c
    - gbk.h
    - gbk.c
    - gb2312.h
    - gb2312.c
    - gbkext_inv.h
    - gbkext_inv.c
    - cp936.h
    - cp936.c
    - converters.h
    - converters.c
    - CMakeLists.txt
  
  - CMakeLists.txt



##  cmake

### add_library
Add a library to the project using the specified source files.
使用明确指明的源文件为当前任务生成一个库


- Normal Libraries

    add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] [<source>...])
    instructions text

    1、Adds a library target called <name> to be built from the source files listed in the command invocation. The <name> corresponds to the logical target name and must be globally unique within a project. The actual file name of the library built is constructed based on conventions of the native platform (such as lib<name>.a or <name>.lib).

    2、New in version 3.1: Source arguments to add_library may use "generator expressions" with the syntax \$\<\.\.\.\>. See the [cmake-generator-expressions](https://cmake.org/cmake/help/v3.23/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) manual for available expressions.
            
    3、New in version 3.11:The source files can be omitted if they are added later using [target_sources()](https://cmake.org/cmake/help/v3.23/command/target_sources.html#command:target_sources)
            
    4、*STATIC*,*SHARED*,or *MODULE* may be given to specify the type of library to be created. *STATIC* libraries are archives of object files for use when linking other targets. *SHARED* libraries are linked dynamically and loaded at runtime. *MODULE* libraries are plugins that are not linked into other targets but may be loaded dynamically at runtime using dlopen-like functionality. if no type is given explicitly the type is *STATIC* or *SHARED* based on whether the current value of the variable [BUILD_SHARED_LIBS](https://cmake.org/cmake/help/v3.23/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS) is **ON**. For *SHARED* and *MODULE* libraries the [POSITION_INDEPENDENT_CODE](https://cmake.org/cmake/help/v3.23/prop_tgt/POSITION_INDEPENDENT_CODE.html#prop_tgt:POSITION_INDEPENDENT_CODE) target property is set to **ON** automatically. A *SHARED* library may be marked with the [FRAMEWORK](https://cmake.org/cmake/help/v3.23/prop_tgt/FRAMEWORK.html#prop_tgt:FRAMEWORK) target property to create an macOS Framework.
            
    5、New in version 3.8: A **STATIC** library may be marked with the **FRAMEWORK** target property to create a static Framework.
            
    6、If a library does not export any symbols, it must not be declared as a **SHARED** library. For example, a Windows resource DLL or a managed C++/CLI DLL that exports no unmanaged symbols would need to be a *MODULE* library. This is because CMake expects a *SHARED* library to always have an associated import library on Windows.
            
    7、By default the library file will be created in the build tree directory corresponding to the source tree directory in which the command was invoked. See documentation of the [ARCHIVE_OUTPUT_DIRECTORY](https://cmake.org/cmake/help/v3.23/prop_tgt/ARCHIVE_OUTPUT_DIRECTORY.html#prop_tgt:ARCHIVE_OUTPUT_DIRECTORY),[LIBRARY_OUTPUT_DIRECTORY](https://cmake.org/cmake/help/v3.23/prop_tgt/LIBRARY_OUTPUT_DIRECTORY.html#prop_tgt:LIBRARY_OUTPUT_DIRECTORY),and [RUNTIME_OUTPUT_DIRECTORY](https://cmake.org/cmake/help/v3.23/prop_tgt/RUNTIME_OUTPUT_DIRECTORY.html#prop_tgt:RUNTIME_OUTPUT_DIRECTORY) target properties to change this location. See documentation of the [OUTPUT_NAME](https://cmake.org/cmake/help/v3.23/prop_tgt/OUTPUT_NAME.html#prop_tgt:OUTPUT_NAME) target property to change the <name> part of the final file name.
            
    8、If EXCLUDE_FROM_ALL is given the corresponding property will be set on the created target. See documentation of the [EXCLUDE_FROM_ALL](https://cmake.org/cmake/help/v3.23/prop_tgt/EXCLUDE_FROM_ALL.html#prop_tgt:EXCLUDE_FROM_ALL) target propetry for details.
            
    9、See the [cmake-buildsystem(7)](https://cmake.org/cmake/help/v3.23/manual/cmake-buildsystem.7.html#manual:cmake-buildsystem(7)) manual for more on defining buildsystem properties.

    10、See also [HEADER_FILE_ONLY](https://cmake.org/cmake/help/v3.23/prop_sf/HEADER_FILE_ONLY.html#prop_sf:HEADER_FILE_ONLY) on what to do if some sources are pre-processed, and you want to have the original sources reachable from within IDE.

    翻译
    1、根据add_library函数中显式调用的源文件列表来生成<name>库对象。函数中的<name>参数将会被视为一个全局作用域的对象——所以在整个项目中必须是独一无二的。而通过这个函数实际生成的库名与操作系统平台有关。如：linux下将会实际生成lib<name>.a、在windows下将会实际生成<name>.lib
    
    2、在Cmake3.1及其以上版本中，add_library中的参数是通过“generator expressions”也就是带有$<...>的语法来实现的。
        
    3、在Cmake3.11及其以上版本中，可以在**add_library**函数中先不指定具体的源文件，再后续使用**target_sources()**函数来添加具体的源文件
        
    4、参数*静态库*、*动态库*、*模块*等用于指明你想要创建库的类型。*静态库*会将编译器根据源文件生成的目标文件整理为一个整体然后一起添加到连接对象中去。*动态库*将会在链接阶段被动态连接，在程序运行阶段动态加载到可执行文件中去。*模块*不会在链接阶段与对象建立任何连接，但是能够像dlopen-like函数一样在运行时被调用。如果在当前函数中没有明确的指出生成库的类型，cmake将会根据变量**BUILD_SHARED_LIBS**来决定生成的库的类型。**ON**表示生成动态库，**OFF**表示生成静态库。如果是动态库或者静态库变量**POSITION_INDEPENDENT_CODE**将会被自动的设置为**ON**。

    5、在Cmake3.8及其以上版本中，静态库也会被标记为静态库Framework参数

    6、如果该库不向外导出任何符号表，则不能将其声明为动态库。例如，在windows平台下导出没有非托管的C/C++的动态库符号表需要使用*模块*参数。这是因为在windows平台下的cmake导出动态库总是会和引入的库产生关联。

    7、在默认情况下，库文件将会在生成树文件夹中被创建，而生成树文件夹是与源文件树文件夹——当前命令被调用的所在位置即为源文件树文件夹。而参数ARCHIVE_OUTPUT_DIRECTORY,LIBRARY_OUTPUT_DIRECTORY,RUNTIME_OUTPUT_DIRECTORY能够用来改变生成库的位置。而使用OUTPUT_NAME参数能够用来修改生成库的名称。

    8、同时可以使用**EXCLUDE_FROM_ALL**参数来设置生成对象的一些属性。

        
    补充说明
    - BUILD_SHARED_LIBS
      作用于add_library函数的全局标志，如果设置为ON则生成动态库

      如果该参数存在并且为ON，对于所有没有明确指出生成库类型的的add_library调用都将生成动态库。该变量通常作为一个选型添加到项目中，这样使用者可以自定义生成库的格式。
        
    - POSITION_INDEPENDENT_CODE
      Whether to create a position-independent target
      是否创建一个位置独立的对象

      The POSITION_INDEPENDENT_CODE property determines whether position independent executables or shared libraries will be created. This property is Ture by default for SHARED and MODULE library targets and False otherwise. This property is initialized by the value of the CMAKE_POSITION_INDEPENDENT_CODE variable if it is set when a target is created.
      该参数用于决定生成的可执行文件或者动态库文件是否是位置独立的。对于静态库对象和模块对象，该参数默认为True，表示位置独立其他的情况默认为False。如果在一个对象还没有被创建前，设置了CMAKE_POSITION_INDEPENDENT_CODE，该参数将会自动初始化POSITION_INDEPENDENT_CODE参数

    - FRAMEWORK
      Build SHARED or STATIC library as Framework Bundle on the macOS and IOS.
      macOS或者IOS平台特有的库架构

      If such a library target has this property set to TRUE it will be buildt as a framework when built on the macOS and IOS. it will have the directory structure required for a framework and will be suitable to be used with the -framework option. This property is initialized by the value of the CMAKE_FRAMEWORK variable if it is set when a target is created.
      如果一个库对象存在该属性，且被设置为TRUE，如果在macOS或者IOS平台上将会依靠该性质来创建对象。

    - ARCHIVE_OUTPUT_DIRECTORY
      Output directory in which to build ARCHIVE target files.
      在哪个路径下创建ARCHIVE对象

      This property specifies the directory into which archive target files should be built. The property value may use generator expressions. Multi-configuration generators (Visual Studio, Xcode, Ninja Multi-Config) append a per-configuration subdirectory to the specified directory unless a generator expression is uesd.
      该属性用于指明应该在哪一个文件夹下生成项目。该属性的值是一个generator expressions。多配置生成器(Visual Studio, Xcode, Ninja Multi-Config)除非使用生成器表达式，否则会在指定的目录下追加每个配置的子目录。

      This property is initialized by the value of the CMAKE_ARCHIVE_OUTPUT_DIRECTORY varable if it is set when a target is created.
      如果在生成对象之前定义了CMAKE_ARCHIVE_OUTPUT_DIRECTORY变量，该变量将会自动被初始化为CMAKE_ARCHIVE_OUTPUT_DIRECTORY

    - LIBRARY_OUTPUT_DIRECTORY
      Output directory in which to build [LIBRARY](https://cmake.org/cmake/help/v3.23/manual/cmake-buildsystem.7.html#library-output-artifacts) target files.
      生成库文件的文件夹

      This property specifies the directory into which library target files should be built. The property value may use generator expressions. Multi-configuration generators(Visual Studio, Xcode, Ninja Multi-Config) append a per-configuration subdirectory to the specified directory unless a generator expression is used.

      This property is initialized by the value of the CMAKE_LIBRARY_OUTPUT_DIRECTORY variable if it is set when a target is created.

  - Object Libraries
  - Interface Libraries
  - Imported Libraries
  - Alias Libraries
  


###  set_property
Set a named proprety in a given scope.
设置作用域

```cmake
set_property(
  <GLOBAL>                                      |
  DIRECTORY [<dir>]                             |
  TARGET    [<target1> ...]                     |
  SOURCE    [<src1> ...]
            [DIRECTORY <dirs> ...]      
            [TARGET_DIRECTORY <targets> ...]    |
  INSTALL   [<file1> ...]                       |
  TEST      [<test1> ...]                       |
  CACHE     [<entry1> ...]                      >
  [APPEND]  [APPEND_STRING]
  PROPERTY  <name> [<value1> ...]
)
```


- describe

  1、Sets one property on zero or more objects of a scope. The first argument determines the scope in which the property is set. It must be one of the follwing:

  GLOBAL:
    Scope is unique and does not accept a name.

  DIRECTORY:
    Scope defaults to the current directory but other directories (already processed by CMake) may be named by full or relative path. Relative paths are treated as relative to the current source directory. See also the [set_directory_properties()](https://cmake.org/cmake/help/v3.23/command/set_directory_properties.html#command:set_directory_properties) command.
    New in version 3.19:<dir> may reference a binary directory.
  TARGET:
    Scope may name zero or more existion targets. See also the [set_target_properties()](https://cmake.org/cmake/help/v3.23/command/set_directory_properties.html#command:set_directory_properties) command.
  SOURCE
    Scope may name zero or more source files. By default, source file properties are only visible to targets added in the same directory (CMakeLists.txt)
    New in version 3.18: Visibility can be set in other directory scopes using one or both of the following sub-options:

    DIRECTORY <dirs>...
      The source file property will be set in each of the <dires> directories' scopes. CMake must already know about each of these directories, either by having added them through a call to [add_subdirectory](https://cmake.org/cmake/help/v3.23/command/add_subdirectory.html#command:add_subdirectory) or it being the top level source directory. Relative paths are treated as relative to the current source directory.
      New in version 3.19:<dirs> may reference a binary directory

    TARGET_DIRECTORY <targets>...
      The source file property will be set in each of the directory scopes where any of the specified <targets> were created (the <targets> must therefore already exist).
      See also the [set_source_files_properties()](https://cmake.org/cmake/help/v3.23/command/set_source_files_properties.html#command:set_source_files_properties)

    INSTALL
      New in version 3.1

      Scope may name zero or more installed file paths. These are made available to CPack to influence deployment.
      Both the property key and value may use generator expressions. Specific properties may apply to installed files and/or directories.
      Paht components have to be separated by forward slashes, must be normalized and are case sensitive.
      To reference the installation prefix itself with a relative path use.
      Currently installed file properties are only defined for the WIX generator where the given paths are relative to the installation prefix.




  - 中文
  1、通过设置该函数中的一个参数来设置0-多个对象的作用域范围，且第一个参数必须是一下关键字
  GLOBAL:
    全局作用域且无需指定名称
  DIRECTORY：
    作用域默认设置为当前CMakeLists.txt文件，对于已经执行过的CMake脚本，使用该关键字指定的名称都会被命名为绝对路径+<name>或者是相对路径+<name>。其中相对路径是以当前源文件夹作为根目录而言的先对路径。
    在CMake版本3.19及以上，关键字dir可以是二进制文件夹
  TARGET:
    作用域可以是0到多个已经存在的target
  SOURCE:
    将作用域设置为0到多个源文件范围。默认情况下，源文件只对同一个文件夹下的CMakeLists.txt脚本生成的target可见。
    在CMake版本3.18及其以上：使用下述子选项，源文件的可见性能够被设置为其他的文件夹范围。

    DIRECTORY <dirs>...
      源文件的作用域将被设置为所给参数的文件夹作用域。但是在之前必须已经定义了这些文件夹，或者是已经使用了add_subdirectory函数添加了该文件夹，或者是顶层源文件夹。相对路径是相对于当前源文件夹而言。
      在CMake版本3.19及其以上，该参数可以指定二进制文件夹

    TARGET_DIRECTORY <targets>...
      源文件的作用域将会被设置为创建对应targets对象所在的文件夹作用域
    
  


###  set
Set a normal, cache, or environment variable to a given value. See the [cmake-language(7) variables](https://cmake.org/cmake/help/v3.23/manual/cmake-language.7.html#cmake-language-variables) documentation for the scopes and interaction of normal variables and cache entries.
Signatures of this command that specify a <value>... placeholder expect zero or more arguments. Multiple arguments will be joined as a [semicolon-separated list](https://cmake.org/cmake/help/v3.23/manual/cmake-language.7.html#cmake-language-lists) to form the actual variable value to be set. Zero arguments will cause normal variables to be unset. See the [unset()](https://cmake.org/cmake/help/v3.23/command/unset.html#command:unset) command to unset variables explicitly.

将普通、缓存或环境变量设置为给定的值。参阅**camke-language(7) variables**文档来了解普通变量的交互和作用域以及缓冲变量的接入。
该函数最明显的特征是使用<value>... 占位符可以是0到多个参数。对于多值参数，CMake将使用分号划分列表来存储多值参数。如果对于普通变量但是没有给定参数，将会使用unset函数。

- Set Normal Variable
  set(<variable> <value>... [PARENT_SCOPE])
  
  Set the given <variable> in the current function or directory scope.
  If the PARENT_SCOPE option is given the variable will be set in the scope above the current scope. Each new directory or function creates a new scope. This command will set the value of a variable into the parent directory or calling function (whichever is applicable to the case at hand). The previous state of the variable's value stays the same in the current scope (e.g., if it was undefined before, it is still undefined and if it had a value, it is still that value).


###  foreach
Evaluate a group of commands for each value in a list.

```cmake
foreach(<loop_var> <items>)
  <commands>
endforeach()
```

where <items> is a list of items that are separated by semicolon or whitespace. All commands between foreach and the matching endforeach are recorded without being invoked. Once the endforeach is evaluated, the recorded list of commands is invoked once for each item in <items>. At the beginning of each iteration the variable <loop_var> will be set to the value of the current item.
The scope of <loop_var> is restricted to the loop scope. See policy CMP0124 for details.
The commands [break()](https://cmake.org/cmake/help/v3.23/command/break.html#command:break) and [continue()](https://cmake.org/cmake/help/v3.23/command/continue.html#command:continue) provide means to escape from the normal control flow.
Per legacy, the endforeach() command admits an optional <loop_var> argument. If used, it must be a verbatim repeat of the argument of the opening foreach command.


###  add_subdirectory
Add a subdirectory to the build.

```cmake
add_subdirectory(source_dir [binary_dir] [EXCLUD_FROM_ALL])
```

Adds a subdirectory to the build. The source_dir specifies the directory in which the source CMakeLists.txt and code files are located. If it is a relative path it will be evaluated with respect to the current directory (the typical usage), but it may also be an absolute path. The binary_dir specifies the directory in which to place the output files. If it is a relative path it will be evaluated with respect to the current output directory, but it may also be an absolute path. If binary_dir is not specified, the value of source_dir, before expanding any relative path, will be used (the typical usage). The CMakeLists.txt file in the specified source directory will be processed immediately by CMake before processing in the current input file continues beyond this command.

if nthe EXCLUDE_FROM_ALL argument is provided then targets in the subdirectory will not be included in the ALL target of the parent directory by default, and will be excluded from IDE project files. Users must explicitly build targets in the subdirectory. This is meant for use when the subdirectory contains a separate part of the project that is useful but not necessary, such as a set of examples. Typically the subdirectory should contain its own [project()](https://cmake.org/cmake/help/v3.23/command/project.html#command:project) command invocation so that a full build system will be generated in the subdirectory (such as a VS IDE solution file). Note that inter-target dependencies supersede this exclusion. If a target built by the parent project depends on a target in the subdirectory, the dependee target will be included in the parent project build system to statisfy the dependency.


为build添加一个子文件夹。参数source_dir是当前调用源CMakeLists.txt文件中的代码文件。如果是相对路径，将会被解释为当前文件夹路径下的相对路径，同时也可以是绝对路径。如果参数binary_dir没有明确指出，在任何需要扩展的相对路径下，都会使用source_dir的路径。指定源目录下的CMakeLists.txt文件将立即被CMake处理，直到当前输入文件的处理超出此命令范围。  


###  aux_source_directory
Find all source files in a directory.

```cmake
aux_source_directory(<dir> <variable>)
```

Collects the names of all the source files in the specified directory and stores the list in the <variable> provided. This command is intended to be used by projects that use explicit template instantiation. Template instantiation files can be stored in a Templates subdirectory and collected automatically using this command to avoid manually listing all instantiations.



###  set_target_properties
Targets can have properties that affect how they are built.
用于设置如何生成targets对象

```cmake
set_target_properties(
  target1 target2 ...
  PROPERTIES prop1 value1
  prop2 value2 ...
)
```

Sets properties on targets. The syntax for the command is to list all the targets you want to change, and then provide the values you want to set next. You can use any prop value pair you want and extract it later with the [get_property()](https://cmake.org/cmake/help/v3.23/command/get_property.html#command:get_property) or [get_target_property()](https://cmake.org/cmake/help/v3.23/command/get_target_property.html#command:get_target_property) command.

See also the set_property(TARGET) command.
See Properties on Targets for the list of properties known to CMake