cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(Engine)

set(CMAKE_SUPPRESS_REGENERATION true)
set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/main.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(GameInJin ${SOURCES})
target_compile_features(GameInJin 
		PUBLIC
			 cxx_std_17
)

target_link_libraries(GameInJin ${CONAN_LIBS})