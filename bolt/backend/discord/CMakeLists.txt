add_subdirectory(dpp)
add_library(discord SHARED
    "discord.cc"
    "discord.hh")
target_include_directories(discord PRIVATE dpp/include)
target_link_libraries(discord dpp)