#!/bin/bash
# libexample 编译脚本(支持交叉编译)
# Optional Command Arguments:
#        ONLYBUILD 不执行 cmake 创建工程只编译版本
# Optional Environment Variables: 
#        TOOLCHAIN_FILE 指定交叉编译的工具链文件
# 		 MACHINE 目标平台, such as x86_64-linux-gnu,默认使用当前系统平台
#        PREFIX 安装路径
#        PROJECT_FOLDER cmake 生成的工程文件(Makefile)文件夹
#        BUILD_TYPE 编译版本类型(DEBUG|RELEASE),默认 DEBUG
#		 BUILD_PRJ 为 ON 时编译版本
#		 RELEASE_PRJ 为 ON 时编译并 release 版本
#        BUILD_TEST 为 ON 时编译测试和工具程序
# 安装对应平台的依赖库
# sudo apt-get install g++ gcc
# 需要 cJSON 库支持,参见 cjson.build(https://gitee.com/l0km/cjson.build.git)
# 需要 openssl,curl 库支持
# ubuntu :
# sudo apt-get install libssl-dev libcurl4-openssl-dev
# Xcode 编译方式
# G=Xcode ./gnu_build.sh

#字符串转大写
#参数1：字符串
function string_toupper()
{
    echo $1 | tr '[a-z]' '[A-Z]'
}
#字符串转小写
#参数1：字符串
function string_lower()
{
    echo $1 | tr '[A-Z]' '[a-z]'
}
sh_folder=$(cd "$(dirname $0)"; pwd -P)
folder_name=$(basename $sh_folder) 
# 定义编译的版本类型(DEBUG|RELEASE)
build_type=release
[[ "$(string_lower $BUILD_TYPE)" =~ DEBUG|RELEASE ]] && build_type=$(string_lower ${BUILD_TYPE})

# 是否编译测试和工具程序
build_test=True
[[ "$(string_toupper $BUILD_TEST)" =  ON ]] && build_test=True
[[ "$(string_toupper $BUILD_TEST)" = OFF ]] && build_test=False

# 并行编译参数
parallel_job=
# 默认cmake -G 生成的工程文件类型
[ -z "$G" ] && G="Eclipse CDT4 - Unix Makefiles"
[[ "$G" =~ .*Makefiles ]] && parallel_job=-j8

BUILD_PRJ=ON
echo build_type=$build_type

# 执行 cmake 创建工程
create_prj=ON
while [[ $# -gt 0 ]]
do
# 转小写
key=$(string_toupper ${1})
case $key in
ONLYBUILD)
	create_prj=OFF;BUILD_PRJ=ON;shift;;
*) shift;;
esac
done

machine=$(g++ -dumpmachine)
gcc_version=$(g++ -dumpversion)

[ -n "$MACHINE" ] && machine=$MACHINE
[ -n "$GCC_VERSION" ] && gcc_version=$GCC_VERSION

# 是否以开发模式编译
[ -z "$SKCAPCI_DEV" ] && export SKCAPCI_DEV=ON

# 工具链文件
toolchain=
[ -n "$TOOLCHAIN_FILE" ] && toolchain="-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE"

conan_profile=
[ -n "$CONAN_PROFILE" ] && conan_profile="-pr:h $CONAN_PROFILE"

# 安装路径
[ -n "$PREFIX" ] && install_prefix="$PREFIX"
[ -z "$PREFIX" ] && install_prefix=$sh_folder/release/libexample-$machine

# 编译路径
[ -n "$PROJECT_FOLDER" ] && prj_folder="$PROJECT_FOLDER"
[ -z "$PROJECT_FOLDER" ] && prj_folder=$sh_folder/../libexample-$machine.prj
[ -z "$PROJECT_FOLDER" ] && [[ "$(string_toupper ${RELEASE_PRJ})" =~ ON ]] && prj_folder=$sh_folder/build/libexample-$machine

echo =========================================================================
echo       Configuration: libexample $machine:$build_type
echo     Build Directory: $prj_folder
echo   Install Directory: $install_prefix
echo =========================================================================

if [[ "$(string_toupper ${create_prj})" =~ ON ]]
then
	[ -d $prj_folder ] && rm -fr $prj_folder
	mkdir -p $prj_folder || exit 

	pushd $prj_folder || exit

	echo "creating Project for $machine ..."
	cmake "$sh_folder" -G "$G" -DCMAKE_BUILD_TYPE=$build_type \
		-DCMAKE_INSTALL_PREFIX=$install_prefix \
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		-DENABLE_SKCAPCI_TEST=$build_test \
		$toolchain || exit
		
else
	pushd $prj_folder || exit
fi

if [[ "$(string_toupper ${RELEASE_PRJ})" =~ ON ]] || [[ "${BUILD_PRJ})" =~ ON ]]
then
	cmake --build . --target install/strip -- $parallel_job || exit 
fi

popd

