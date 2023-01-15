cmake_minimum_required(VERSION 3.8.8)

project(Bullock)

include(deps/buildcore/base.cmake)

find_package(Qt6 COMPONENTS Core Network Widgets REQUIRED)

include(address_sanitizer)

set(CMAKE_CXX_STANDARD 20)
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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
endif()

if(APPLE)
	set(CMAKE_MACOSX_RPATH OFF)
	set(CMAKE_INSTALL_NAME_DIR "@executable_path/../Library/Bullock")
	set(BULLOCK_DIST_BIN              Bullock.app/Contents/MacOS)
	set(BULLOCK_DIST_LIB              Bullock.app/Contents/Library)
	set(BULLOCK_DIST_MODULE           Bullock.app/Contents/Plugins)
	set(BULLOCK_DIST_RESOURCES        Bullock.app/Contents/Resources)
	set(BULLOCK_DIST_MAC_APP_CONTENTS Bullock.app/Contents)
else()
	set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/../lib/Bullock")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(BULLOCK_DIST_BIN       bin)
	set(BULLOCK_DIST_LIB       lib)
	set(BULLOCK_DIST_RESOURCES share)
endif()

enable_testing()

add_subdirectory($ENV{OX_PATH} $ENV{OX_PATH})
include_directories(
	SYSTEM
		$ENV{OX_PATH}/src
)
add_subdirectory(src)

install(
	FILES
		rsrc/icons/bullock128.png
		rsrc/icons/bullock512.png
	DESTINATION
		${BULLOCK_DIST_RESOURCES}/icons
)
