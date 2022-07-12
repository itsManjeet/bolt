add_subdirectory(discord)
add_library(backend SHARED
    "backend.cc"
    "backend.hh")
target_link_libraries(backend discord)