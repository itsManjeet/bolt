cmake_minimum_required(VERSION 3.16)
project(bolt
    VERSION 0.0.1
    DESCRIPTION "Smart Virtual Assistant")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(POSITION_INDEPENDENT_CODE ON)
include(GNUInstallDirs)


set(BOLT_PLUGIN_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}/bolt")
set(BOLT_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/bolt")

set(GENERATED_DIR ${PROJECT_BINARY_DIR}/generated)
configure_file(${PROJECT_SOURCE_DIR}/config.hxx.in ${GENERATED_DIR}/config.hxx)
include_directories(${GENERATED_DIR} ${PROJECT_SOURCE_DIR}/external)


add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(cmd)
add_subdirectory(data)
