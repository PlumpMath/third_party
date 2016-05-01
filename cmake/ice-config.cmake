set(CMAKE_IMPORT_FILE_VERSION 1)

get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(THIRD_PARTY_PATH "${CURRENT_LIST_PATH}" PATH)

include(${CURRENT_LIST_PATH}/common.cmake)

find_package(libressl REQUIRED NO_CMAKE_SYSTEM_PATH)
find_package(zlib REQUIRED NO_CMAKE_SYSTEM_PATH)

common_library("${THIRD_PARTY_PATH}/ice" ice zlib)

if(WIN32)
  set_target_properties(ice PROPERTIES
    INTERFACE_LINK_LIBRARIES "crypto;zlib;shlwapi.lib")
else()
  set_target_properties(ice PROPERTIES
    INTERFACE_LINK_LIBRARIES "crypto;zlib")
endif()

set(CMAKE_IMPORT_FILE_VERSION)
