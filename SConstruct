# Note: This build depends on chromuim source code
# ./chromium-sandbox/** just for reference

import os

env = Environment()
env.Replace(CC = "clang-cl")

env.Append(CXXFLAGS=['/std:c++20', '/DUNICODE', '/D_UNICODE', '/DNOMINMAX'],
           CPPPATH=[
            'C:/src/chromium/src/',
            'C:/src/chromium/src/out/sandbox_build/gen/',
            'C:/src/chromium/src/buildtools/third_party/libc++/',
            'C:/src/chromium/src/third_party/perfetto/include/',
            'C:/src/chromium/src/out/sandbox_build/gen/third_party/perfetto/build_config/',
            'C:/src/chromium/src/out/sandbox_build/gen/third_party/perfetto/',
            'C:/src/chromium/src/base/allocator/partition_allocator/src/',
            'C:/src/chromium/src/out/sandbox_build/gen/base/allocator/partition_allocator/src/',
            'C:/src/chromium/src/third_party/abseil-cpp/',
            'C:/src/chromium/src/third_party/boringssl/src/include/',
            'C:/src/chromium/src/third_party/protobuf/src/'],
           LIBPATH=['./libs/'],
           LIBS=['sandbox'],
           LINKFLAGS=[])

env.Program(target='sandbox', source='main.cpp')
