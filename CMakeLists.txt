cmake_minimum_required(VERSION 3.14-3.18)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")

if(NOT DEFINED IMPROC_SUPERPROJECT_VERSION)
  set(IMPROC_SUPERPROJECT_VERSION 1.0.0)
endif()

if(NOT DEFINED IMPROC_DRAWER_WITH_TESTS)
  set(IMPROC_DRAWER_WITH_TESTS OFF)
endif()

if(IMPROC_WITH_TESTS OR IMPROC_DRAWER_WITH_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

if(IMPROC_DRAWER_WITH_QRCODE_SUPPORT)
  list(APPEND VCPKG_MANIFEST_FEATURES "qrcode")
endif()

if(IMPROC_DRAWER_WITH_BARCODE_DATA_MATRIX_SUPPORT)
  list(APPEND VCPKG_MANIFEST_FEATURES "data-matrix")
endif()

if(IMPROC_DRAWER_WITH_TEXT_SUPPORT)
  list(APPEND VCPKG_MANIFEST_FEATURES "text")
endif()

project(
  improc_drawer
  VERSION     ${IMPROC_SUPERPROJECT_VERSION}
  LANGUAGES   CXX
)

include(FetchContent)

# Project configuration
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"      CACHE PATH "Archive output dir.")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"      CACHE PATH "Library output dir.")
  set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin"      CACHE PATH "PDB (MSVC debug symbol)output dir.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"      CACHE PATH "Executable/dll output dir.")
  set(CMAKE_DEBUG_POSTFIX d)
  option(IMPROC_WITH_TESTS 			     "Building library with tests"          OFF)
  option(IMPROC_WITH_COVERAGE  	     "Building library with code coverage"  OFF)
  option(BUILD_SHARED_LIBS           "Building shared libraries"            ON)
  option(IMPROC_DRAWER_WITH_QRCODE_SUPPORT              "Building drawer module with QR-Code support"                 ON)
  option(IMPROC_DRAWER_WITH_BARCODE_DATA_MATRIX_SUPPORT "Building drawer module with barcode and data matrix support" ON)
  option(IMPROC_DRAWER_WITH_TEXT_SUPPORT                "Building drawer module with text generation support"         ON)

  add_subdirectory(${PROJECT_SOURCE_DIR}/modules/infrastructure ${CMAKE_BINARY_DIR}/infrastructure)
  add_subdirectory(${PROJECT_SOURCE_DIR}/modules/services       ${CMAKE_BINARY_DIR}/services)
  add_subdirectory(${PROJECT_SOURCE_DIR}/modules/corecv         ${CMAKE_BINARY_DIR}/corecv)
endif()

# Add external dependencies
if(IMPROC_DRAWER_WITH_QRCODE_SUPPORT)
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(unofficial-nayuki-qr-code-generator CONFIG REQUIRED)
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS unofficial::nayuki-qr-code-generator::nayuki-qr-code-generator)
  else()
    # qrcodegen
    FetchContent_Declare(
      qrcodegen
      GIT_REPOSITORY  https://github.com/nayuki/QR-Code-generator
      GIT_TAG         v1.8.0
      SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/qrcodegen/qrcodegen
    )
    FetchContent_MakeAvailable(qrcodegen)
    add_library(
      qrcodegen
      
      ${PROJECT_SOURCE_DIR}/external/qrcodegen/qrcodegen/cpp/qrcodegen.hpp
      ${PROJECT_SOURCE_DIR}/external/qrcodegen/qrcodegen/cpp/qrcodegen.cpp
    )
    set_target_properties(qrcodegen PROPERTIES CXX_STANDARD           11)
    set_target_properties(qrcodegen PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
    set_target_properties(qrcodegen PROPERTIES LINKER_LANGUAGE        CXX)
    set_target_properties(qrcodegen PROPERTIES VERSION                1.8.0)
    set_target_properties(qrcodegen PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

    target_include_directories  (qrcodegen  INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/qrcodegen>
                                                      $<INSTALL_INTERFACE:include> )
    message(STATUS "Build qrcodegen Version: 1.8.0")
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS qrcodegen)
  endif()
endif()

if(IMPROC_DRAWER_WITH_BARCODE_DATA_MATRIX_SUPPORT OR IMPROC_DRAWER_WITH_QRCODE_SUPPORT)
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(ZXing CONFIG REQUIRED)
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS ZXing::Core ZXing::ZXing)
  else()
    # zxing-cpp
    FetchContent_Declare(
      zxingcpp
      GIT_REPOSITORY  https://github.com/nu-book/zxing-cpp
      GIT_TAG         v2.2.1
      SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/zxingcpp
    )
    set(BUILD_EXAMPLES        OFF)
    set(BUILD_BLACKBOX_TESTS  OFF)
    FetchContent_MakeAvailable(zxingcpp)
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS ZXing)
  endif()
endif()

if(IMPROC_DRAWER_WITH_TEXT_SUPPORT)
  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(Freetype REQUIRED)
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS Freetype::Freetype)
  else()
    # FreeType
    FetchContent_Declare(
      freetype
      GIT_REPOSITORY  https://gitlab.freedesktop.org/freetype/freetype
      GIT_TAG         VER-2-13-2
      SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/freetype
    )
    FetchContent_MakeAvailable(freetype)
    list(APPEND IMPROC_DRAWER_EXTRA_LIBS freetype)
  endif()
endif()

message(STATUS "Build ${PROJECT_NAME} Version: ${PROJECT_VERSION}")
message(STATUS "CMake Toolchain File: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Dir: ${CMAKE_INSTALL_PREFIX}")

set(
  IMPROC_DRAWER_CORE_FILES

  ${PROJECT_SOURCE_DIR}/include/improc/drawer/drawer_types/image_file_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/base_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/element_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/grid_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/metric_pixel_converter.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/metric_pixel_json_converter.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/page_drawer_type.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/page_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/engine/page_element_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/layout_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/logger_drawer.hpp

  ${PROJECT_SOURCE_DIR}/src/base_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/page_element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/page_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/image_file_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/grid_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/layout_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/page_drawer_type.cpp
  ${PROJECT_SOURCE_DIR}/src/metric_pixel_converter.cpp
  ${PROJECT_SOURCE_DIR}/src/metric_pixel_json_converter.cpp
)

set(
  IMPROC_DRAWER_QRCODE_FILES

  ${PROJECT_SOURCE_DIR}/include/improc/drawer/drawer_types/qrcode_drawer.hpp
  ${PROJECT_SOURCE_DIR}/src/qrcode_drawer.cpp
)

set(
  IMPROC_DRAWER_BARCODE_DATA_MATRIX_FILES

  ${PROJECT_SOURCE_DIR}/include/improc/drawer/drawer_types/barcode_drawer.hpp
  ${PROJECT_SOURCE_DIR}/include/improc/drawer/drawer_types/data_matrix_drawer.hpp
  ${PROJECT_SOURCE_DIR}/src/barcode_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/data_matrix_drawer.cpp
)

set(
  IMPROC_DRAWER_TEXT_FILES

  ${PROJECT_SOURCE_DIR}/include/improc/drawer/drawer_types/text_drawer.hpp
  ${PROJECT_SOURCE_DIR}/src/text_drawer.cpp
)

set(IMPROC_DRAWER_LIB_FILES ${IMPROC_DRAWER_CORE_FILES})
if(IMPROC_DRAWER_WITH_QRCODE_SUPPORT)
  set(IMPROC_DRAWER_LIB_FILES ${IMPROC_DRAWER_LIB_FILES} ${IMPROC_DRAWER_QRCODE_FILES})
endif()
if(IMPROC_DRAWER_WITH_BARCODE_DATA_MATRIX_SUPPORT)
  set(IMPROC_DRAWER_LIB_FILES ${IMPROC_DRAWER_LIB_FILES} ${IMPROC_DRAWER_BARCODE_DATA_MATRIX_FILES})
endif()
if(IMPROC_DRAWER_WITH_TEXT_SUPPORT)
  set(IMPROC_DRAWER_LIB_FILES ${IMPROC_DRAWER_LIB_FILES} ${IMPROC_DRAWER_TEXT_FILES})
endif()

add_library(${PROJECT_NAME} SHARED ${IMPROC_DRAWER_LIB_FILES})
add_library(improc::drawer ALIAS ${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} improc::corecv ${IMPROC_DRAWER_EXTRA_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION                ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_include_directories  (${PROJECT_NAME}   PRIVATE   ${PROJECT_SOURCE_DIR}/include)
target_include_directories  (${PROJECT_NAME}   INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                               $<INSTALL_INTERFACE:include> )

# Link dependencies
target_link_libraries       (${PROJECT_NAME}   PRIVATE    ${IMPROC_DRAWER_EXTRA_LIBS})
target_link_libraries       (${PROJECT_NAME}   PRIVATE    improc::corecv)
target_link_libraries       (${PROJECT_NAME}   INTERFACE  ${IMPROC_DRAWER_EXTRA_LIBS})
target_link_libraries       (${PROJECT_NAME}   INTERFACE  improc::corecv)

# Tests configuration
if(IMPROC_WITH_TESTS OR IMPROC_DRAWER_WITH_TESTS)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test     ${CMAKE_BINARY_DIR}/improc_drawer_test)
endif()

# # Install configuration
# if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
#   set(CMAKE_INSTALL_LIBDIR "lib")
# endif()
# if (NOT DEFINED CMAKE_INSTALL_BINDIR)
#   set(CMAKE_INSTALL_BINDIR "bin")
# endif()
# if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
#   set(CMAKE_INSTALL_INCLUDEDIR "include")
# endif()

# # Include files
# install(
#   TARGETS     ${PROJECT_NAME}
#   EXPORT      ${PROJECT_NAME}_targets
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/improc                  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES     ${PROJECT_SOURCE_DIR}/LICENSE                         DESTINATION share/licenses/improc/drawer)
# if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#   install(FILES   ${PROJECT_SOURCE_DIR}/external/opencv/LICENSE         DESTINATION share/licenses/improc/opencv)
#   if(IMPROC_DRAWER_WITH_QRCODE_SUPPORT)
#     install(FILES ${PROJECT_SOURCE_DIR}/external/qrcodegen/qrcodegen/Readme.markdown  DESTINATION share/licenses/improc/qrcodegen)
#   endif()

#   if(IMPROC_DRAWER_WITH_QRCODE_SUPPORT OR IMPROC_DRAWER_WITH_BARCODE_DATA_MATRIX_SUPPORT)
#     install(FILES ${PROJECT_SOURCE_DIR}/external/zxingcpp/LICENSE                     DESTINATION share/licenses/improc/ZXing)
#   endif()

#   if(IMPROC_DRAWER_WITH_TEXT_SUPPORT)
#     install(FILES ${PROJECT_SOURCE_DIR}/external/freetype/docs/GPLv2.TXT              DESTINATION share/licenses/improc/freetype)
#   endif()
# endif()

# # pkg-config files
# if(IS_ABSOLUTE ${CMAKE_INSTALL_INCLUDEDIR})
#   set(PKG_CONFIG_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
# else()
#   set(PKG_CONFIG_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
# endif()
# if(IS_ABSOLUTE ${CMAKE_INSTALL_LIBDIR})
#   set(PKG_CONFIG_LIBDIR ${CMAKE_INSTALL_LIBDIR})
# else()
#   set(PKG_CONFIG_LIBDIR "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
# endif()
# configure_file(
#   ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in
#   ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc 
#   @ONLY )
# install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# # CMake config files
# set(CONFIG_TARGETS_FILE "${PROJECT_NAME}ConfigTargets.cmake")
# set(EXPORT_DESTINATION_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
# install(EXPORT ${PROJECT_NAME}_targets FILE ${CONFIG_TARGETS_FILE} DESTINATION ${EXPORT_DESTINATION_DIR})

# include(CMakePackageConfigHelpers)
# configure_package_config_file(
#   ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}_config.cmake.in
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake 
#   INSTALL_DESTINATION ${EXPORT_DESTINATION_DIR} )
# write_basic_package_version_file(
#   ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#   VERSION ${IMPROC_SUPERPROJECT_VERSION}
#   COMPATIBILITY SameMajorVersion)
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake 
#               ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#         DESTINATION ${EXPORT_DESTINATION_DIR})

# # Install configuration
# set(${PROJECT_NAME}_libs ${PROJECT_NAME} improc_corecv ${IMPROC_DRAWER_EXTRA_LIBS})
# install(
#   TARGETS     ${${PROJECT_NAME}_libs}
#   DESTINATION lib
#   EXPORT      ${PROJECT_NAME}_target 
# )
