project(Bullock)

set(CMAKe_INCLUDE_CURRENT_DIR ON)

find_pacKage(Qt6Widgets)
set(CMAKE_AUTOMOC ON)

add_executable(
	Bullock MACOSX_BUNDLE
		callstackmodel.cpp
		channelview.cpp
		channelmodel.cpp
		main.cpp
		mainwindow.cpp
		processdata.cpp
		processselector.cpp
		server.cpp
		traceeventmodel.cpp
		traceview.cpp
)

target_link_libraries(
	Bullock
		OxMetalClaw
		OxStd
		Qt6::Core
		Qt6::Network
		Qt6::Widgets
)

if(APPLE)
	set_target_properties(Bullock PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()

install(
	TARGETS
		Bullock
	RUNTIME DESTINATION
		${BULLOCK_DIST_BIN}
	BUNDLE DESTINATION .
)
