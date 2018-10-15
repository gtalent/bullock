project(bullock)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
set(CMAKE_AUTOMOC ON)

add_executable(
	bullock MACOSX_BUNDLE
		main.cpp
		mainwindow.cpp
		processdata.cpp
		procselector.cpp
		server.cpp
		traceeventmodel.cpp
		traceview.cpp
)

target_link_libraries(
	bullock
		Qt5::Core
		Qt5::Network
		Qt5::Widgets
)

if(APPLE)
	set_target_properties(bullock PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()

install(
	TARGETS
		bullock
	RUNTIME DESTINATION
		${BULLOCK_DIST_BIN}
	BUNDLE DESTINATION .
)
