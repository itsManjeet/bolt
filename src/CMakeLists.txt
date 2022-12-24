file(GLOB_RECURSE Bolt_Files "*.cxx" "*.hxx")
add_library(LIB_${PROJECT_NAME} SHARED ${Bolt_Files})

target_link_libraries(LIB_${PROJECT_NAME} PUBLIC dl)
install(TARGETS LIB_${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})