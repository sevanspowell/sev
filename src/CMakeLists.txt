cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(svGameEngineLibrary VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/modules")

######### SDL2 ########## 
find_package(SDL2 REQUIRED)
add_library(sdl2 SHARED IMPORTED)
set_property(TARGET sdl2 PROPERTY
  INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR})
set_property(TARGET sdl2 PROPERTY
  IMPORTED_LOCATION ${SDL2_LIBRARY})

enable_testing()
add_subdirectory(libs/googletest EXCLUDE_FROM_ALL)
add_subdirectory(svLibrary)
