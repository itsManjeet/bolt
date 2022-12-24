add_executable(Test_${PROJECT_NAME}
    "TestNaiveBayes.cxx")

target_compile_definitions(Test_${PROJECT_NAME} PRIVATE 
    -DDATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data"
    -DCACHE_PATH="${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(
  Test_${PROJECT_NAME}
  GTest::gtest_main
  LIB_${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(Test_${PROJECT_NAME})