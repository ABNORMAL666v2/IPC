cmake_minimum_required(VERSION 3.0)
project(ipcv3)

# Specify the include directories for curl and rapidjson
include_directories(
    G:/repos/ipcv3/vcpkg_installed/x64-windows/include
)

# Add your source files
add_executable(ipc ipcv3.cpp)

# Link against libcurl
target_link_libraries(ipc curl)

