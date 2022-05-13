#======== 将与当前CMakeLists.txt同一目录下的源文件都添加到_SOURCES中 =========
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} _SOURCES)
#======== 使用源文件来生成iconv库 ========
add_library(iconv ${_SOURCES})

#======== 设置iconv库需要的头文件 ========
target_include_directories(
	iconv
	PRIVATE	
	$<BUILD_INTERFACE:${LIBRARY_OUTPUT_DIRECTORY}>
	$<INSTALL_INTERFACE:${LIBRARY_INSTALL_DIR}>
)

SET(_lib_header_)
list(APPEND _lib_header_ iconv.h)

set_target_properties(
	iconv
	PROPERTIES
	PUBLIC_HEADER ${_lib_header_}
	DEBUG_POSTFIX _d
	INTERFACE_POSITION_INDEPENDENT_CODE ON
)

SET(_install_targets)
list(APPEND _install_targets iconv)


#========================= 安装脚本 =======================
if(WIN32 AND NOT CYGWIN)
	SET(CONFIG_INSTALL_DIR cmake)
else()
	SET(CONFIG_INSTALL_DIR ${LIBRARY_INSTALL_DIR}/cmake/iconv)
endif()

include(CMakePackageConfigHelpers)
#SET(iconv_DEPENDENCY "")

configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/iconv-config.cmake
	INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
	iconv-config-version.cmake VERSION
	${PROJECT_VERSION} 
	COMPATIBILITY 
	SameMajorVersion
)


install(
	FILES		
		${CMAKE_CURRENT_BINARY_DIR}/iconv-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/iconv-config-version.cmake
	DESTINATION 
		${CONFIG_INSTALL_DIR}
)

install(
	TARGETS
	${_install_targets} EXPORT iconv-targets
	RUNTIME			DESTINATION		${RUNTIME_INSTALL_DIR}
	LIBRARY			DESTINATION		${LIBRARY_INSTALL_DIR}
	ARCHIVE			DESTINATION		${LIBRARY_INSTALL_DIR}
	PUBLIC_HEADER	DESTINATION		${INCLUDE_INSTALL_DIR}/iconv
)

install(
	EXPORT	iconv-targets
	DESTINATION
		${INCLUDE_INSTALL_DIR}/iconv
)
#  export library information for CMake projects
#install(EXPORT iconv DESTINATION ${CONFIG_INSTALL_DIR})


unset(_SOURCES)
unset(_lib_header_)


