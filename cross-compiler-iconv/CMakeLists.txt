#cmake file for project iconv
#author:chentao
#created:2022/05/12


cmake_minimum_required(VERSION 3.0)


#	policy ruler configuration
if(POLICY CMP0048)
	cmake_policy(SET CMP0048 NEW)
endif()
if(POLICY CMP0077)
	cmake_policy(SET CMP0077 NEW)
endif()
if(POLICY CMP0074)
	cmake_policy(SET CMP0074 NEW)
endif()


#	Cmake-3.0以上的版本才允许使用VERSION option
project(iconv VERSION 1.0.0 LANGUAGES C)

#	要求C编译器且支持C99
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED ON)

#	是否生成动态库
option(BUILD_SHARED_LIBS "Setting ON to build shared library of iconv" OFF)

#	优先使用环境变量定义的BUILD_SHARED_LIBS
if(DEFINED ENV{BUILD_SHARED_LIBS})
	MESSAGE(STATUS "DEFINE BUILD_SHARED_LIBS = $ENV{BUILD_SHARED_LIBS}")
	SET(BUILD_SHARED_LIBS $ENV{BUILD_SHARED_LIBS})
endif()

if(MSVC)
	#	关闭C4819警告
	add_definitions("/wd4819")
	MESSAGE(STATUS "close VS Warning wd4819")
	#	关闭CRT_SECURE_NO_WARNINGS
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	MESSAGE(STATUS "close VS CRT_SECURE_NO_WARNINGS")
endif(MSVC)

# 	将USE_FOLDERS设置为全局变量，其值为ON
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#****************** 设置全局目标文件生成位置 **********************#
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

#****************** 设置生成的目标文件的安装位置 **********************#
SET(RUNTIME_INSTALL_DIR	bin)
SET(LIBRARY_INSTALL_DIR lib)
SET(INCLUDE_INSTALL_DIR include)


#================== 检查依赖的ANSI C文件是否存在===============#
include(CheckIncludeFile)
CHECK_INCLUDE_FILE(stdio.h HAS_STDIO_H)
CHECK_INCLUDE_FILE(malloc.h HAS_MALLOC_H)
CHECK_INCLUDE_FILE(string.h HAS_STRING_H)
CHECK_INCLUDE_FILE(ctype.h HAS_CTYPE_H)
CHECK_INCLUDE_FILE(limits.h HAS_LIMITS_H)
CHECK_INCLUDE_FILE(stdbool.h HAS_STDBOOL_H)


SET(SUBDIRECTORIES src)
#	检查是否存在子文件夹，如果存在子文件夹，将其全部添加
foreach(subdir ${SUBDIRECTORIES})
	if(IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${subdir}")
		MESSAGE(STATUS "Found SubDirectory ${subdir}, append it")
		add_subdirectory(${subdir})
	endif()
endforeach()