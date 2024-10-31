cmake_minimum_required(VERSION 3.10)
project(ClientServerApp)

# Initialize C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Executable file
add_executable(ClientServerApp main.cpp)

# Set options
target_compile_options(ClientServerApp PRIVATE -Wall -Wextra -pedantic)

# Check for POSIX libraries (Linux/Unix if needed)
if(UNIX)
    target_link_libraries(ClientServerApp PRIVATE pthread)
endif()
