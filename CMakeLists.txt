cmake_minimum_required(VERSION 3.16)
project(bolt
        VERSION 0.0.1
        DESCRIPTION "Smart Virtual Assistant")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(POSITION_INDEPENDENT_CODE ON)
include(GNUInstallDirs)


set(BOLT_PLUGIN_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}/bolt")
set(BOLT_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/bolt")

set(GENERATED_DIR ${PROJECT_BINARY_DIR}/generated)
configure_file(${PROJECT_SOURCE_DIR}/config.hxx.in ${GENERATED_DIR}/config.hxx)
include_directories(${GENERATED_DIR} ${PROJECT_SOURCE_DIR}/external)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
find_library(VOSK vosk REQUIRED)
find_library(ESPEAK_NG espeak-ng REQUIRED)
find_library(CURL curl REQUIRED)

include_directories(${PIPEWIRE_INCLUDE_DIRS})
file(GLOB LibBolt_Src "src/*.cxx" "src/*.hxx")
add_library(LIB${PROJECT_NAME} SHARED ${LibBolt_Src})
target_link_libraries(LIB${PROJECT_NAME} PUBLIC ${PIPEWIRE_LIBRARIES} ${VOSK} ${ESPEAK_NG} ${CURL})
set_target_properties(LIB${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cxx)
target_link_libraries(${PROJECT_NAME} LIB${PROJECT_NAME})

function(add_plugin id)
    if (EXISTS ${PROJECT_SOURCE_DIR}/plugins/${id}/${id}.cxx)
        add_library(${id} SHARED plugins/${id}/${id}.cxx)
        target_link_libraries(${id} PUBLIC LIB${PROJECT_NAME})
        target_include_directories(${id} PRIVATE ${PROJECT_SOURCE_DIR}/src)
        set_target_properties(${id}
                PROPERTIES
                PREFIX "")
        install(TARGETS ${id}
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/bolt/)
    elseif (EXISTS ${PROJECT_SOURCE_DIR}/plugins/${id}/${id})
        install(FILES
                ${PROJECT_SOURCE_DIR}/plugins/${id}/${id}
                DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/bolt/)
    endif ()
endfunction()

add_plugin(ostree)
add_plugin(greeting)
add_plugin(joke)
add_plugin(reminder)

install(TARGETS LIB${PROJECT_NAME} ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

file(GLOB Bolt_Headers "src/*.hxx")
install(FILES ${Bolt_Headers}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES data/model.json
        DESTINATION ${CMAKE_INSTALL_DATADIR}/bolt/)

configure_file(${PROJECT_SOURCE_DIR}/config.json.in ${PROJECT_SOURCE_DIR}/config.json)