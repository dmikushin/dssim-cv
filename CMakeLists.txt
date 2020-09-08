cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(dssim)

if (UNIX)
# Use $ORIGIN or @loader_path, which allow to pick up
# dependencies relative to the executable folder
if (APPLE)
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_BINARY_DIR}" "@loader_path" "@loader_path/.." "@loader_path/lib" "@loader_path/../lib")
else()
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_BINARY_DIR}" "$ORIGIN" "$ORIGIN/.." "$ORIGIN/lib" "$ORIGIN/../lib")
endif()
endif()

# Have the RPATH set during the build already
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Search path for CMake include files.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE "src/${PROJECT_NAME}.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_NAME_DIR "@rpath")

install(TARGETS ${PROJECT_NAME} DESTINATION .)

set_target_properties(${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

if ("x${CMAKE_BUILD_TYPE}" STREQUAL "x")
set(CMAKE_BUILD_TYPE Release)
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCH)
string(TOLOWER ${CMAKE_SYSTEM_NAME} OS)
set(ARCHIVE "${PROJECT_NAME}-${ARCH}-${OS}-${BUILD_TYPE}")
set(CPACK_PACKAGE_FILE_NAME ${ARCHIVE})
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)

