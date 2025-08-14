cmake_minimum_required(VERSION 3.31) # 3.16+ for good LLVM support on most systems
project(tinyswiftc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER "clang++")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Find LLVM (preferred) -------------------------------------------------------
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} at ${LLVM_DIR}")
message(STATUS "Using LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})



# Sources ---------------------------------------------------------------------
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/**/*.cpp"
)
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "${CMAKE_BINARY_DIR}")

add_executable(tinyswiftc ${PROJECT_SOURCES})


# Common compile options (mirrors Makefile: -Wall -Wextra -g -v)
target_compile_options(tinyswiftc PRIVATE -Wall -Wextra -g)

target_link_libraries(tinyswiftc PRIVATE LLVM)

# Output directory similar to Makefile's build/ -------------------------------
set_target_properties(tinyswiftc PROPERTIES
    BUILD_RPATH "$(brew --prefix llvm)/lib"
)

message(STATUS "tinyswiftc configured. ENABLE_DEBUG_LOG=${ENABLE_DEBUG_LOG}")

