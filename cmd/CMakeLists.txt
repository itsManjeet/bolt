add_executable(${PROJECT_NAME}
    "bolt.cxx")
target_link_libraries(${PROJECT_NAME} LIB_${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})