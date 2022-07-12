add_subdirectory(unknown)
add_subdirectory(http)
add_library(plugins SHARED
    "plugin.cc"
    "plugin.hh")

target_link_libraries(plugins 
    unknown
    http)