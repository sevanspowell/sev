################################# Fever ########################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(SEVLibrary VERSION 0.0.1 LANGUAGES C CXX)

add_library(SEV
  src/commands.c
  src/shell.c
  src/tokenizer.c
  )

target_include_directories(SEV
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  src
  )

set_property(TARGET SEV PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET SEV PROPERTY C_STANDARD 99)

# target_compile_features(SEV
#   )

# target_link_libraries(SEV
#   )

install(TARGETS SEV EXPORT SEVConfig
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
install(DIRECTORY include/ DESTINATION include)

# Make project importable from the install directory
install(EXPORT SEVConfig DESTINATION share/SEV/cmake)

# Make project importable from the build directory
export(TARGETS SEV FILE SEVConfig.cmake)

################################# Unit tests ###################################
add_executable(SEVTest
  test/test.cpp
  )

target_link_libraries(SEVTest
  gtest
  SEV
  )

add_test(SEVTest SEVTest)
