add_library(http SHARED
    "http.cc"
    "http.hh")
find_library(CURL curl REQUIRED)
target_link_libraries(http ${CURL})