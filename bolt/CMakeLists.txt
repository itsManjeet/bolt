find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

add_subdirectory(logics)
add_subdirectory(backend)
add_subdirectory(plugins)

add_library(bolt SHARED "bolt.cc" "bolt.hh")
target_link_libraries(bolt logics backend plugins ${YAML_CPP_LIBRARIES})