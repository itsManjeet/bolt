find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

function(add_module id)
    file(GLOB_RECURSE ${id}_Files "${id}/*.cc" "${id}/*.hh")
    add_library(${id} ${${id}_Files})
    install(TARGETS ${id}
        LIBRARY DESTINATION lib)
endfunction(add_module)

file(GLOB_RECURSE Bolt_Files "*.cc" "*.hh")

find_library(CURL curl REQURIED)
find_package(PkgConfig)
pkg_check_modules(SYSTEMD systemd REQUIRED)

add_library(bolt SHARED ${Bolt_Files})
target_link_libraries(bolt 
    ${YAML_CPP_LIBRARIES}
    ${CURL}
    dpp
    systemd)

install(
    TARGETS bolt
    LIBRARY DESTINATION lib
)