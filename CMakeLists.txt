cmake_minimum_required(VERSION 3.14-3.18)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

if(NOT DEFINED IMPROC_SUPERPROJECT_VERSION)
  set(IMPROC_SUPERPROJECT_VERSION 1.0.0)
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
  option(BUILD_SHARED_LIBS           "Build using shared libraries" ON)
  option(IMPROC_WITH_TESTS 			     "Building library with tests"  ON)

  add_subdirectory(${PROJECT_SOURCE_DIR}/../infrastructure ${CMAKE_BINARY_DIR}/infrastructure)
  add_subdirectory(${PROJECT_SOURCE_DIR}/../services       ${CMAKE_BINARY_DIR}/services)
endif()

# Add external dependencies
# QR-Code Generator
FetchContent_Declare(
  qrcodegen
  GIT_REPOSITORY  https://github.com/nayuki/QR-Code-generator
  GIT_TAG         v1.7.0
  SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/qrcodegen
)
FetchContent_MakeAvailable(qrcodegen)

# Data Matrix
FetchContent_Declare(
  libdmtx
  GIT_REPOSITORY  https://github.com/dmtx/libdmtx
  GIT_TAG         v0.7.5
  SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/libdmtx
)
FetchContent_MakeAvailable(libdmtx)

message(STATUS "Build ${PROJECT_NAME} Version: ${PROJECT_VERSION}")
message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Dir: ${CMAKE_INSTALL_PREFIX}")

add_library(
  ${PROJECT_NAME}
  
  ${PROJECT_SOURCE_DIR}/include

  ${PROJECT_SOURCE_DIR}/src/base_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/page_element_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/page_drawer.cpp
  ${PROJECT_SOURCE_DIR}/src/rotation_type.cpp
)
add_library(improc::drawer ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD           17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED  TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE        CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION                ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX          ${CMAKE_DEBUG_POSTFIX})

target_include_directories  (${PROJECT_NAME}   PRIVATE   ${PROJECT_SOURCE_DIR}/include)
target_include_directories  (${PROJECT_NAME}   INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                               $<INSTALL_INTERFACE:include> )

# Link dependencies
find_package(PkgConfig REQUIRED) 
pkg_check_modules(lib_opencv REQUIRED IMPORTED_TARGET opencv4)
target_link_libraries       (${PROJECT_NAME}   PRIVATE   	PkgConfig::lib_opencv)
target_link_libraries       (${PROJECT_NAME}   PRIVATE    improc::infrastructure)
target_link_libraries       (${PROJECT_NAME}   PRIVATE    improc::services)
target_link_libraries       (${PROJECT_NAME}   INTERFACE 	PkgConfig::lib_opencv)
target_link_libraries       (${PROJECT_NAME}   INTERFACE  improc::infrastructure)
target_link_libraries       (${PROJECT_NAME}   INTERFACE  improc::services)

# Tests configuration
if(IMPROC_WITH_TESTS)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test     ${CMAKE_BINARY_DIR}/drawer_test)
endif()

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  add_subdirectory(${PROJECT_SOURCE_DIR}/version  ${CMAKE_BINARY_DIR}/version)
endif()
