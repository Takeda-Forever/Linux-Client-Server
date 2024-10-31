cmake_minimum_required(VERSION 3.10)
project(ClientServerApp)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исполняемый файл
add_executable(ClientServerApp main.cpp)

# Устанавливаем опции для компилятора
target_compile_options(ClientServerApp PRIVATE -Wall -Wextra -pedantic)

# Проверка для POSIX библиотек (Linux/Unix, если нужно)
if(UNIX)
    target_link_libraries(ClientServerApp PRIVATE pthread)
endif()
