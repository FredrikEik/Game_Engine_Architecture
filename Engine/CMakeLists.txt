cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(Engine)

set(CMAKE_SUPPRESS_REGENERATION true)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(GameInJin Source/main.cpp)
target_compile_features(GameInJin PRIVATE cxx_std_17)
target_link_libraries(GameInJin ${CONAN_LIBS})