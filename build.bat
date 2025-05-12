mkdir build
cd build
cmake -G "Visual Studio 17 2022" -T ClangCL ..
cd ..
cmake --build build
@REM cmake --build build --config Release