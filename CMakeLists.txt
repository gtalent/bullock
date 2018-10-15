cmake_minimum_required(VERSION 2.8.8)

project(bullock)

set(QTDIR "" CACHE STRING "Path to Qt Libraries")

set(CMAKE_PREFIX_PATH ${QTDIR})
find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(address_sanitizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
else()
	add_definitions(-DNDEBUG)
endif()

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare")

	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	endif()

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		# forces colored output when using ninja
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
	endif()
endif()

if(APPLE)
	set(CMAKE_MACOSX_RPATH OFF)
	set(CMAKE_INSTALL_NAME_DIR "@executable_path/../Library/bullock")
	set(BULLOCK_DIST_BIN              bullock.app/Contents/MacOS)
	set(BULLOCK_DIST_LIB              bullock.app/Contents/Library)
	set(BULLOCK_DIST_PLUGIN           bullock.app/Contents/Plugins)
	set(BULLOCK_DIST_RESOURCES        bullock.app/Contents/Resources)
	set(BULLOCK_DIST_MAC_APP_CONTENTS bullock.app/Contents)
else()
	set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/../lib/bullock")
	if(NOT ${QTDIR} STREQUAL "")
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "${QTDIR}/lib")
	endif()
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(BULLOCK_DIST_BIN       bin)
	set(BULLOCK_DIST_LIB       lib)
	set(BULLOCK_DIST_RESOURCES share)
endif()

enable_testing()

add_subdirectory(src)
