PROJECT_NAME=Bullock
BUILDCORE_PATH=deps/buildcore
VCPKG_PKGS=
include ${BUILDCORE_PATH}/base.mk

ifeq ($(OS),darwin)
	PROJECT_EXECUTABLE=./dist/${CURRENT_BUILD}/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
else
	PROJECT_EXECUTABLE=./dist/${CURRENT_BUILD}/bin/${PROJECT_NAME}
endif

.PHONY: run
run: install
	${PROJECT_EXECUTABLE}
.PHONY: debug
debug: install
	${DEBUGGER} ${PROJECT_EXECUTABLE}
