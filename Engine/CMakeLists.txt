set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(Engine)

set(CMAKE_SUPPRESS_REGENERATION true)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS
	Source/*.cpp
	Source/*.h
	Source/*.hpp
)

file(GLOB_RECURSE SHADERFILES CONFIGURE_DEPENDS 
	Shaders/*.frag
	Shaders/*.vert
	Shaders/*.glsl
)
file(GLOB_RECURSE Assets CONFIGURE_DEPENDS 
	Assets/*.obj
)
assign_source_group(${SOURCE})
assign_source_group(${SHADERFILES})
file( COPY  ${Assets} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )



add_executable(GameInJin ${SOURCE} ${SHADERFILES})


set_property(TARGET GameInJin PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameInJin)


target_compile_features(GameInJin 
		PUBLIC
			 cxx_std_17
)
target_link_libraries(GameInJin ${CONAN_LIBS})