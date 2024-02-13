cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 11)

# Add your source files
add_executable(ipc_util main.cpp)

# Specify the path to libcurl
set(CURL_LIBRARY "lib/libcurl")
set(CURL_INCLUDE_DIR "include")

# Link against libcurl
target_include_directories(IPC PRIVATE "${CURL_INCLUDE_DIR}")
target_link_libraries(IPC PRIVATE "${CURL_LIBRARY}")
