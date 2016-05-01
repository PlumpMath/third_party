set(CMAKE_IMPORT_FILE_VERSION 1)

get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(THIRD_PARTY_PATH "${CURRENT_LIST_PATH}" PATH)

include(${CURRENT_LIST_PATH}/common.cmake)

find_package(libressl REQUIRED NO_CMAKE_SYSTEM_PATH)

common_library("${THIRD_PARTY_PATH}/asio" asio)
common_library("${THIRD_PARTY_PATH}/asio" asio_ssl)

if(MSVC)
  set_target_properties(asio asio_ssl PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "ASIO_STANDALONE=1;ASIO_SEPARATE_COMPILATION=1;ASIO_MSVC=1")
else()
  set_target_properties(asio asio_ssl PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "ASIO_STANDALONE=1;ASIO_SEPARATE_COMPILATION=1")
endif()

if(MSVC)
  set_target_properties(asio PROPERTIES
    INTERFACE_LINK_LIBRARIES "comctl32.lib;ws2_32.lib")
endif()

set_target_properties(asio_ssl PROPERTIES
  INTERFACE_LINK_LIBRARIES "asio;crypto;ssl")

set(CMAKE_IMPORT_FILE_VERSION)
