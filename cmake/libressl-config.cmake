set(CMAKE_IMPORT_FILE_VERSION 1)

get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(THIRD_PARTY_PATH "${CURRENT_LIST_PATH}" PATH)

include(${CURRENT_LIST_PATH}/common.cmake)

common_library("${THIRD_PARTY_PATH}/libressl" crypto)
common_library("${THIRD_PARTY_PATH}/libressl" ssl)
common_library("${THIRD_PARTY_PATH}/libressl" tls)

set(CMAKE_IMPORT_FILE_VERSION)
