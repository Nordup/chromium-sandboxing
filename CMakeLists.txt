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
    "C:/src/chromium/src/out/sandbox_build"
    "C:/src/chromium/src/out/sandbox_build/obj/sandbox/win"
    "C:/src/chromium/src/out/sandbox_build/obj/base/win"
    "C:/src/chromium/src/out/sandbox_build/win_clang_x64_for_rust_host_build_tools/rustlib/windows_x86_64_msvc_lib_v0_52"
    "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64"
    "C:/src/chromium/src/out/sandbox_build/obj/sandbox/win"
    "C:/src/chromium/src/out/sandbox_build/obj/sandbox"
)

# Output executable
add_executable(sandbox_test main.cpp)

# Link required libraries
target_link_libraries(sandbox_test
    base.dll.lib
    sandbox.lib
    libc++.dll.lib
    pe_image.lib
    third_party_abseil-cpp_absl.dll.lib
    third_party_perfetto_libperfetto.dll.lib
    third_party_boringssl.dll.lib
    third_party_zlib.dll.lib
    cppgen_plugin.lib
    protozero_plugin.lib
    base_allocator_partition_allocator_src_partition_alloc_raw_ptr.dll.lib
    base_allocator_partition_allocator_src_partition_alloc_allocator_shim.dll.lib
    base_allocator_partition_allocator_src_partition_alloc_allocator_core.dll.lib
    base_allocator_partition_allocator_src_partition_alloc_allocator_base.dll.lib
    windows.0.52.0.lib
    delayimp.lib
    common.lib
    service_resolver.lib
)

# Copy DLLs to executable directory
add_custom_command(TARGET sandbox_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/src/chromium/src/out/sandbox_build/base.dll"
        "C:/src/chromium/src/out/sandbox_build/libc++.dll"
        "C:/src/chromium/src/out/sandbox_build/third_party_abseil-cpp_absl.dll"
        "C:/src/chromium/src/out/sandbox_build/third_party_perfetto_libperfetto.dll"
        "C:/src/chromium/src/out/sandbox_build/third_party_boringssl.dll"
        "C:/src/chromium/src/out/sandbox_build/third_party_zlib.dll"
        "C:/src/chromium/src/out/sandbox_build/base_allocator_partition_allocator_src_partition_alloc_raw_ptr.dll"
        "C:/src/chromium/src/out/sandbox_build/base_allocator_partition_allocator_src_partition_alloc_allocator_shim.dll"
        "C:/src/chromium/src/out/sandbox_build/base_allocator_partition_allocator_src_partition_alloc_allocator_core.dll"
        "C:/src/chromium/src/out/sandbox_build/base_allocator_partition_allocator_src_partition_alloc_allocator_base.dll"
        $<TARGET_FILE_DIR:sandbox_test>
)
