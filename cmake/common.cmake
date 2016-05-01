# Platform
string(TOLOWER ${CMAKE_SYSTEM_NAME} common_system)

# Architecture
set(common_arch x86)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(common_arch x64)
endif()

# Configuration
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()

if(MSVC)
  set(common_config $<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
else()
  string(TOLOWER ${CMAKE_BUILD_TYPE} common_config)
endif()

# Executable Prefix
set(common_exe_prefix bin/${common_system}/${common_arch}/${common_config})

# Library Prefix
set(common_lib_prefix lib/${common_system}/${common_arch}/${common_config})

# Compiler
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  # Compiler Options
  foreach(flag
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
    string(REGEX REPLACE "/Z[iI7]" "" ${flag} "${${flag}}")
    if(CMAKE_VS_PLATFORM_TOOLSET MATCHES "_xp")
      set(${flag} "${${flag}} /arch:IA32")
    endif()
  endforeach()
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")

  # Definitions
  add_definitions(/D_UNICODE /DUNICODDE /DWIN32_LEAN_AND_MEAN /DNOMINMAX)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS)
  if (CMAKE_VS_PLATFORM_TOOLSET MATCHES "_xp")
    add_definitions(/DWINVER=0x0501 /D_WIN32_WINNT=0x0501)
  else()
    add_definitions(/DWINVER=0x0601 /D_WIN32_WINNT=0x0601)
  endif()

  # Linker Options
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
else()
  # Compiler Options
  add_compile_options(-static -fexceptions)
  if(CMAKE_CXX_COMPILER_ID)
    add_compile_options(-fcxx-exceptions)
  endif()

  # Linker Options
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

# CMake Prefix Path
get_filename_component(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)

# Functions
function(assign_source_group)
  foreach(source IN ITEMS ${ARGN})
    file(RELATIVE_PATH source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
    get_filename_component(source_path "${source_rel}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
  endforeach()
endfunction(assign_source_group)

function(common_library path name)
  if(NOT TARGET ${name})
    add_library(${name} STATIC IMPORTED)
    set_target_properties(${name} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${path}/include")
    set_property(TARGET ${name} APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)
    if(MSVC)
      set(prefix)
      set(extension lib)
    else()
      set(prefix lib)
      set(extension a)
    endif()
    set_target_properties(${name} PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
      IMPORTED_LOCATION_DEBUG "${path}/lib/${common_system}/${common_arch}/debug/${prefix}${name}.${extension}"
      IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
      IMPORTED_LOCATION_RELEASE "${path}/lib/${common_system}/${common_arch}/release/${prefix}${name}.${extension}")
  endif()
endfunction(common_library)
