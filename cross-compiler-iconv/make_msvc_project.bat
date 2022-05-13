:: Set encodeing to UTF-8 for console
@CHCP 65001
:: MSVC(Visual Studoi 2015)编译 libexample 脚本
:: 需要 cJSON 库支持,参见 cjson.build(https://gitee.com/l0km/cjson.build.git)
:: 需要 openssl 库支持 
:: 需要 curl 库支持 
:: 可选参数:
::   /BUILD          - 编译项目
::   /ONLYBUILD      - 跳过生成项目工程文件,直接编译项目

ECHO OFF 
SETLOCAL
ECHO make libexample VS2015 project
where cmake
if errorlevel 1 (
	ECHO cmake NOT FOUND.
	exit /B -1
)
ECHO cmake found
SET sh_folder=%~dp0
rem 删除最后的 '\'
SET sh_folder=%sh_folder:~0,-1%

SET OPT_BUILD=0
SET OPT_CMAKE=1
SET BUILD_TYPE=Debug
:: parse command arguments
:loop
IF x%1 == x GOTO :pare_end
IF /I "%1" == "/BUILD"      SET OPT_BUILD=1
IF /I "%1" == "/ONLYBUILD"  SET OPT_BUILD=1 && SET OPT_CMAKE=0 
IF /I "%1" == "/RELEASE"    SET OPT_BUILD=1 && SET BUILD_TYPE=Release

SHIFT
GOTO :loop
:pare_end

SET project_folder=%sh_folder%\build\project.msvc
IF "%OUTDIR%x" == "x" SET OUTDIR=%sh_folder%\release\libexample_windows_vc_x86

ECHO project_folder=%project_folder%
ECHO OUTDIR=%OUTDIR%

IF %OPT_CMAKE% == 0 (
	PUSHD %project_folder%  || EXIT /B
	GOTO :cmake_build 
)

if EXIST %project_folder% rmdir %project_folder% /s/q
MKDIR %project_folder%
PUSHD %project_folder% || exit /B


ECHO creating x86_64 Project for Visual Studio 2015 ... %BUILD_TYPE%

cmake "%sh_folder%" -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=%OUTDIR%  || EXIT /B 255

:: 编译项目
:cmake_build
::执行编译安装
IF %OPT_BUILD% == 1 (
	cmake --build . --config %BUILD_TYPE% --target install -- /maxcpucount || EXIT /B 
)
POPD
ENDLOCAL