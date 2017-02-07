################################# Fever ########################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(svLibrary VERSION 0.0.1 LANGUAGES C CXX)

add_library(sv
  src/console/Commands.c
  src/console/Console.cpp
  src/console/Shell.c
  src/console/Tokenizer.c
  src/platform/Platform.cpp
  src/platform/SDL2Platform.cpp
  )

target_include_directories(sv
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  src
  )

set_property(TARGET sv PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET sv PROPERTY CXX_STANDARD 11)

# target_compile_features(sv
#   )

target_link_libraries(sv
  sdl2
  )

install(TARGETS sv EXPORT svConfig
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
install(DIRECTORY include/ DESTINATION include)

# Make project importable from the install directory
install(EXPORT svConfig DESTINATION share/sv/cmake)

# Make project importable from the build directory
export(TARGETS sv FILE svConfig.cmake)

################################# Unit tests ###################################
add_executable(svTest
  test/test.cpp
  )

target_link_libraries(svTest
  gtest
  sv
  )

add_test(svTest svTest)
