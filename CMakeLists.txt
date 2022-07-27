cmake_minimum_required(VERSION 3.10)
project(bolt
    VERSION 1.0.0)

add_definitions("-DDEBUG")

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
add_subdirectory(docs)
add_subdirectory(bolt)
add_subdirectory(gnome-search-provider)
add_subdirectory(services)
add_executable(bot "main.cc")
target_link_libraries(bot bolt)
install(
    TARGETS bot
    RUNTIME DESTINATION bin
    RENAME bolt
)