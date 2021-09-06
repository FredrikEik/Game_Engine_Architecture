set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(Engine)

set(CMAKE_SUPPRESS_REGENERATION true)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
	Source/*.cpp
	Source/*.h
)

file(GLOB SHADERFILES CONFIGURE_DEPENDS 
	Shaders/*.frag
	Shaders/*.vert
	Shaders/*.glsl
)

MACRO(SOURCE_GROUP_BY_FOLDER target)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${SOURCES})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}/${target}" ${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
ENDMACRO(SOURCE_GROUP_BY_FOLDER)

add_executable(GameInJin ${SOURCES} ${SHADERFILES})
target_compile_features(GameInJin 
		PUBLIC
			 cxx_std_17
)

target_link_libraries(GameInJin ${CONAN_LIBS})