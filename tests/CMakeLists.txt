include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tests 
  main.cpp
  ${PROJECT_SOURCE_DIR}/src/nfa.cpp
  test_nfa.cpp
)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(
  tests 
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
