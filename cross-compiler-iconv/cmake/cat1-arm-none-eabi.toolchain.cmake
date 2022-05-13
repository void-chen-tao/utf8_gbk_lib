INCLUDE(CMakeForceCompiler)

# This one is important
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT $ENV{CAT1_SDK_PREFIX} STREQUAL "")
    set(CAT1_SDK_PREFIX $ENV{CAT1_SDK_PREFIX})
endif()
if(NOT CAT1_SDK_PREFIX)
	message(WARNING "NOT DEFINE CAT1 SDK_PREFIX FOR CROSS COMIPILER")
endif()

message(STATUS CAT1_SDK_PREFIX=${CAT1_SDK_PREFIX})

set(_compiler_prefix "${CAT1_SDK_PREFIX}/prebuilts/win32/gcc-arm-none-eabi")
if(NOT EXISTS ${_compiler_prefix})
	if(NOT $ENV{CROSS_COMPILER_PREFIX} STREQUAL "")
	    set(_compiler_prefix $ENV{CROSS_COMPILER_PREFIX})
	elseif(CROSS_COMPILER_PREFIX)
	    set(_compiler_prefix ${CROSS_COMPILER_PREFIX})
	else()
		find_program(_armcc_path armcc)
		if(NOT _armcc_path)
			message(FATAL_ERROR "NOT FOUND compiler armcc in system path")
		endif()
		get_filename_component(_bin ${_armcc_path} DIRECTORY )
		get_filename_component(_compiler_prefix ${_bin} DIRECTORY )
	endif()	
endif()
set(_suffix)
if(WIN32)
	set(_suffix .exe)
endif()

# Specify the cross compiler
SET(CMAKE_C_COMPILER "${_compiler_prefix}/bin/arm-none-eabi-gcc${_suffix}")
# CXX compiler not work,disable CXX compiler
#SET(CMAKE_CXX_COMPILER "${_compiler_prefix}/bin/arm-none-eabi-cpp${_suffix}")
SET(CMAKE_AR "${_compiler_prefix}/bin/arm-none-eabi-ar${_suffix}")

UNSET(CMAKE_C_FLAGS_INIT CACHE)
SET(CMAKE_C_FLAGS_INIT "-std=gnu11 -c -march=armv7-a -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mno-unaligned-access -g -Os -Wall -ffunction-sections -fdata-sections -DINT_MAX=2147483647 -DINT_MIN=(-INT_MAX-1)" CACHE STRING "" FORCE)
#UNSET(CMAKE_CXX_FLAGS_INIT CACHE)
#SET(CMAKE_CXX_FLAGS_INIT "         -march=armv7-a -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mno-unaligned-access -g -Os -Wall -ffunction-sections -fdata-sections -DINT_MAX=2147483647 -DINT_MIN=(-INT_MAX-1)" CACHE STRING "" FORCE)
#UNSET(CMAKE_EXE_LINKER_FLAGS_INIT CACHE)
#SET(CMAKE_EXE_LINKER_FLAGS_INIT "" CACHE STRING "" FORCE)
UNSET(CMAKE_AR_FLAGS_INIT CACHE)
SET(CMAKE_AR_FLAGS_INIT "-rs" CACHE STRING "" FORCE)  #AR option,cat1 -o 选项不兼容，需要改为-rs

# set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> ${CMAKE_AR_FLAGS_INIT} <TARGET> <OBJECTS>" CACHE STRING "C Archive Create")
# set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS>")
#SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> ${CMAKE_AR_FLAGS_INIT} -o <TARGET> <OBJECTS>" CACHE STRING "CXX Archive Create")

include_directories("${CAT1_SDK_PREFIX}/inc"
					"${CAT1_SDK_PREFIX}/inc/std_inc"
					"${CAT1_SDK_PREFIX}/inc/std_inc/machine"
					"${CAT1_SDK_PREFIX}/inc/std_inc/sys"										
					)

# Where is the target environment
SET(CMAKE_FIND_ROOT_PATH "${_compiler_prefix}")

if(CAT1_SDK_PREFIX)
	list(APPEND CMAKE_FIND_ROOT_PATH ${CAT1_SDK_PREFIX})
endif()

# Search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(USE_CURL FALSE)
SET(MAC_AS_SN FALSE)
SET(USE_CAT1 TRUE)
SET(NATIVE_VPRINTF TRUE)
SET(SKCAPCI_NOEXE TRUE)
SET(USE_STD_LOCALTIME OFF)
SET(USE_STD_ERRNO OFF)

# For libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CONAN_DISABLE_CHECK_COMPILER True)
unset(_compiler_prefix)
