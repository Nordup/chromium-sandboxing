cmake_minimum_required(VERSION 3.20)
project(sandbox_test CXX)

# Use Clang-cl
set(CMAKE_C_COMPILER "clang-cl")
set(CMAKE_CXX_COMPILER "clang-cl")
set(CMAKE_C_COMPILER_FRONTEND_VARIANT "MSVC")
set(CMAKE_CXX_COMPILER_FRONTEND_VARIANT "MSVC")

# Force C++20 using MSVC-compatible syntax
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /DUNICODE /D_UNICODE /DNOMINMAX")

# Include directories
include_directories(
    "C:/src/chromium/src/"
    "C:/src/chromium/src/out/sandbox_build/gen/"
    "C:/src/chromium/src/buildtools/third_party/libc++/"
    "C:/src/chromium/src/third_party/perfetto/include/"
    "C:/src/chromium/src/out/sandbox_build/gen/third_party/perfetto/build_config/"
    "C:/src/chromium/src/out/sandbox_build/gen/third_party/perfetto/"
    "C:/src/chromium/src/base/allocator/partition_allocator/src/"
    "C:/src/chromium/src/out/sandbox_build/gen/base/allocator/partition_allocator/src/"
    "C:/src/chromium/src/third_party/abseil-cpp/"
    "C:/src/chromium/src/third_party/boringssl/src/include/"
    "C:/src/chromium/src/third_party/protobuf/src/"
)

# Library search paths
link_directories(
    "./libs/"
    "C:/src/chromium/src/out/sandbox_build"
)

# Output executable
add_executable(sandbox_test main.cpp)

# Link required libraries
target_link_libraries(sandbox_test
    sandbox
    libc++.dll
)

# Optional: treat warnings as errors
# target_compile_options(sandbox PRIVATE /W4 /WX)
