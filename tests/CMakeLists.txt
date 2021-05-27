include (CTest)

set (TESTS_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/")

include_directories (${CMAKE_CURRENT_BINARY_DIR})
configure_file (test_common.h.in ${CMAKE_CURRENT_BINARY_DIR}/test_common.h)

if (NOT CRITERION_FOUND)
  include (FindPkgConfig)
  pkg_check_modules (CRITERION criterion>=2.0)
endif ()

function (create_test TEST_NAME TEST_PATH)
  set (TEST_EXECUTABLE test-${TEST_NAME})
  add_executable (${TEST_EXECUTABLE} ${TEST_PATH})
  target_link_libraries (${TEST_EXECUTABLE} mdo-assets ${CRITERION_LIBRARIES})
  add_test (NAME ${TEST_NAME} COMMAND ${TEST_EXECUTABLE} --verbose)
endfunction ()

create_test (asset_ctx asset_ctx.c)
create_test (decompressors decompressors.c)
create_test (lump lump.c)
