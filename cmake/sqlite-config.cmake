set(CMAKE_IMPORT_FILE_VERSION 1)

get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(THIRD_PARTY_PATH "${CURRENT_LIST_PATH}" PATH)

include(${CURRENT_LIST_PATH}/common.cmake)

find_package(ice REQUIRED NO_CMAKE_SYSTEM_PATH)

common_library("${THIRD_PARTY_PATH}/sqlite" sqlite)

set_target_properties(sqlite PROPERTIES
  INTERFACE_LINK_LIBRARIES "ice")

set(CMAKE_IMPORT_FILE_VERSION)
