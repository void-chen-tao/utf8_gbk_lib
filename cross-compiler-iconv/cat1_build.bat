:: Set encodeing to UTF-8 for console
@CHCP 65001
:: CAT1(arm-none-eabi)编译 libexample 脚本,安装到MercuryLSDK %CAT1_SDK_PREFIX%/Third-party/libexample
:: 可选参数:
::   /ONLYBUILD      - 跳过生成项目工程文件,直接编译项目

ECHO OFF 
SETLOCAL
IF NOT DEFINED CAT1_SDK_PREFIX (
	ECHO error: CAN NOT FOUND MercuryLSDK,because NOT DEFINED environment variable: 'CAT1_SDK_PREFIX'
	EXIT /B -1
)
:: 获取 cat1 SDK包位置
ECHO CAT1_SDK_PREFIX=%CAT1_SDK_PREFIX% 

where cmake
if errorlevel 1 (
	ECHO cmake NOT FOUND.
	exit /B -1
)
ECHO cmake found
SET sh_folder=%~dp0
:: 删除最后的 '\'
SET sh_folder=%sh_folder:~0,-1%

SET OPT_BUILD=1
SET OPT_CMAKE=1
:: parse command arguments
:loop
IF x%1 == x GOTO :pare_end
IF /I "%1" == "/ONLYBUILD"  SET OPT_BUILD=1 && SET OPT_CMAKE=0 

SHIFT
GOTO :loop
:pare_end

:: 定义编译的版本类型(Debug|Release)
SET build_type=Release

SET project_folder=%sh_folder%\build\cat1
SET OUTDIR=%sh_folder%/release/libexample_cat1
ECHO project_folder=%project_folder%
ECHO OUTDIR=%OUTDIR%

IF %OPT_CMAKE% == 0 (
	pushd %project_folder%
	GOTO :conan_build 
)

ECHO build Project for cat1(arm-none-eabi) ...
if EXIST %project_folder% rmdir %project_folder% /s/q
mkdir %project_folder% || exit /B
pushd %project_folder%

cmake "%sh_folder%"  -G "Eclipse CDT4 - Unix Makefiles" ^
	-DCMAKE_INSTALL_PREFIX=%OUTDIR% ^
	-DCMAKE_TOOLCHAIN_FILE=%sh_folder%\cmake\cat1-arm-none-eabi.toolchain.cmake || EXIT /B 255

:: 编译项目
:cmake_build

cmake --build %project_folder% --target install

POPD
ENDLOCAL