cmake_minimum_required(VERSION 3.14-3.18)

include(FetchContent)
include(GoogleTest)

# Add external dependencies
#GOOGLETEST
FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.11.0
  SOURCE_DIR      ${PROJECT_SOURCE_DIR}/test/external/googletest
)
FetchContent_MakeAvailable(googletest)

mark_as_advanced(
  gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest      PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)

# Create file to get folder with test
configure_file(
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME}_test_config.hpp.in 
  ${PROJECT_SOURCE_DIR}/test/${PROJECT_NAME}_test_config.hpp
)

add_executable(
  ${PROJECT_NAME}_test
    
  ${PROJECT_SOURCE_DIR}/test/test_logger_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_base_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_page_element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_page_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_rotation_type.cpp
  ${PROJECT_SOURCE_DIR}/test/test_corecv_json_parser.cpp
  ${PROJECT_SOURCE_DIR}/test/test_image_file_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_qrcode_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_barcode_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_data_matrix_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_text_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_grid_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_layout_drawer.cpp
  ${PROJECT_SOURCE_DIR}/test/test_page_drawer_type.cpp
  ${PROJECT_SOURCE_DIR}/test/test_metric_pixel_converter.cpp
  ${PROJECT_SOURCE_DIR}/test/test_metric_pixel_json_converter.cpp
)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME}_test PROPERTIES FOLDER                 ${PROJECT_SOURCE_DIR}/test)
set_target_properties(${PROJECT_NAME}_test PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_include_directories(${PROJECT_NAME}_test   PRIVATE   ${PROJECT_SOURCE_DIR}/test)

target_link_libraries(${PROJECT_NAME}_test  PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test  PRIVATE gtest gtest_main)

gtest_discover_tests(
  ${PROJECT_NAME}_test
  WORKING_DIRECTORY                           ${PROJECT_SOURCE_DIR}/test
  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY    "${PROJECT_SOURCE_DIR}/test"
)