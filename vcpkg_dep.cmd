@echo off
Echo. 
Echo Install x64/x86 
pause
cls
vcpkg install --triplet x64-windows
vcpkg install --triplet x86-windows
Echo. 
Echo Install ARM/ARM64
pause
cls
vcpkg install --triplet arm64-windows
vcpkg install --triplet arm-windows
pause
cls
exit