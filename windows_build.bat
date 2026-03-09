rmdir /s /q build 2>nul
rmdir /s /q _CPack_Packages 2>nul

cmake -S . -B build ^
    -G "Visual Studio 18 2026" ^
    -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE="C:\Users\Antoiix\vcpkg\scripts\buildsystems\vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET=x64-windows

cmake --build build --config Release

cpack --config build/CPackConfig.cmake