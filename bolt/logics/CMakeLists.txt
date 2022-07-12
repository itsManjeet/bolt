add_subdirectory(classifier)
add_library(logics SHARED
    "logic.cc"
    "logic.hh")

target_link_libraries(logics classifier)