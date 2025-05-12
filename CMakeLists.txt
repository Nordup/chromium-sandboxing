cmake_minimum_required(VERSION 3.20)
project(sandbox_test CXX)

# Force static runtime to match CEF
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Od /Ob0 /D_DEBUG")

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
    "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64"
    "C:/Users/Nordup/Downloads/cef_binary_136.1.4+g89c0a8c+chromium-136.0.7103.93_windows64_minimal/cef_binary_136.1.4+g89c0a8c+chromium-136.0.7103.93_windows64_minimal/Release"
)

# Output executable
add_executable(sandbox_test main.cpp BrokerServicesDelegateImpl.cpp)

# Link required libraries
target_link_libraries(sandbox_test
    # Chromium Embedded Framework (CEF). Download from https://cef-builds.spotifycdn.com/index.html#windows64
    cef_sandbox.lib
    # Core Windows libraries
    kernel32.lib
    user32.lib
    gdi32.lib
    advapi32.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
    # Additional Windows libraries
    winmm.lib
    shlwapi.lib
    propsys.lib
    powrprof.lib
    shcore.lib
    ntdll.lib
    setupapi.lib
    cfgmgr32.lib
    version.lib
    ws2_32.lib
    runtimeobject.lib
    dbghelp.lib
    userenv.lib
    delayimp.lib
    wbemuuid.lib
    mincore.lib
)
