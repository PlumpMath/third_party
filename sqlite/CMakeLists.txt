cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(sqlite VERSION 0.1.1 LANGUAGES C CXX)

# Common
include(../cmake/common.cmake)

# Dependencies
find_package(ice REQUIRED NO_CMAKE_SYSTEM_PATH)

# Definitions
add_definitions(-DSQLITE_OMIT_LOAD_EXTENSION)

# Sources
file(GLOB_RECURSE headers include/*.h)
assign_source_group(${headers})

file(GLOB_RECURSE sources src/*.h src/*.cc src/*.c)
assign_source_group(${sources})

# Library
add_library(sqlite STATIC ${headers} ${sources})
if(MSVC)
  target_compile_options(sqlite PRIVATE -wd4996)
endif()

# Include Directories
target_include_directories(sqlite PUBLIC src
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# Link Libraries
target_link_libraries(sqlite PUBLIC ice)

# Install Headers
install(DIRECTORY include/ DESTINATION include
  FILES_MATCHING PATTERN "*.h")

# Install Libraries
install(TARGETS sqlite EXPORT sqlite DESTINATION ${common_lib_prefix})

# Generate Config
#export(TARGETS sqlite EXPORT sqlite FILE sqlite-config.cmake)

# Install Config
#install(EXPORT sqlite FILE sqlite-config.cmake DESTINATION lib/cmake)
