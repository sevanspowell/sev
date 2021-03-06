################################# Fever ########################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(svLibrary VERSION 0.0.1 LANGUAGES C CXX)

add_library(sv
  src/ClientVariables.cpp
  src/Common.cpp
  src/Engine.cpp
  src/Globals.cpp
  src/Log.cpp
  src/ProgramOptions.cpp
  src/client/Client.cpp
  src/console/Commands.c
  src/console/Console.cpp
  src/console/ConsoleCommands.cpp
  src/console/Shell.c
  src/console/Tokenizer.c
  src/input/Input.cpp
  src/network/Connection.cpp
  src/network/Sockets.cpp
  src/platform/Keycodes.cpp
  src/platform/Platform.cpp
  src/platform/SDL2Platform.cpp
  src/resource/ConfigResourceLoader.cpp
  src/resource/DateTime.cpp
  src/resource/Resource.cpp
  src/resource/ResourceCache.cpp
  src/resource/ResourceFolderPC.cpp
  src/script/ScriptInterface.cpp
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

target_compile_features(sv PUBLIC 
  cxx_strong_enums
  )

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
