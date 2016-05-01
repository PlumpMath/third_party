set(CMAKE_IMPORT_FILE_VERSION 1)

get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(THIRD_PARTY_PATH "${CURRENT_LIST_PATH}" PATH)

include(${CURRENT_LIST_PATH}/common.cmake)

foreach(name
    boost_atomic
    boost_chrono
    boost_container
    boost_context
    boost_coroutine
    boost_date_time
    boost_exception
    boost_filesystem
    boost_graph
    boost_iostreams
    boost_log
    boost_log_setup
    boost_math_c99
    boost_math_c99f
    boost_math_c99l
    boost_math_tr1
    boost_math_tr1f
    boost_math_tr1l
    boost_prg_exec_monitor
    boost_program_options
    boost_random
    boost_regex
    boost_serialization
    boost_signals
    boost_system
    boost_test_exec_monitor
    boost_thread
    boost_timer
    boost_type_erasure
    boost_unit_test_framework
    boost_wave
    boost_wserializationg)
  common_library("${THIRD_PARTY_PATH}/boost" ${name})
  set_target_properties(${name} PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_NO_LIB=1")
endforeach()

set(CMAKE_IMPORT_FILE_VERSION)
