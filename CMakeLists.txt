cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(regex VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# Add warning flags (GCC)
set(WARNING_FLAGS "-Werror;-Wall;-Wextra;-Wconversion;\
-Wsign-conversion;-Wfloat-equal;-Wundef;-Wshadow;\
-Wpointer-arith;-Wcast-align;-Wstrict-prototypes;-Wwrite-strings;\
-Waggregate-return;-Wcast-qual;-Wswitch-default;-Wswitch-enum;-Wunreachable-code;\
")

file(
  GLOB SRC_FILES
  ${PROJECT_SOURCE_DIR}/*.cpp
  ${PROJECT_SOURCE_DIR}/tests/*.cpp)

set_source_files_properties(
  ${SRC_FILES} PROPERTIES COMPILE_OPTIONS ${WARNING_FLAGS}
)

# Include tests
# add_subdirectory(tests)

add_executable(
  main
  main.cpp
  src/nfa.cpp
  src/parser.cpp
  src/regex.cpp
)

target_include_directories(main PUBLIC include)

