# Template project

BUILD_DIR=build
THREADS_NUM=$(shell nproc)
COVERAGE_REPORT_DIR=${BUILD_DIR}/lcov_html
CMAKE_DEBUG_FLAGS=

.PHONY: all
all: coverage

report: coverage
	xdg-open ${COVERAGE_REPORT_DIR}/index.html

coverage: test
	lcov --capture --directory ${BUILD_DIR} --exclude '*_test*' \
		--output-file ${BUILD_DIR}/lcov_test.info
	genhtml ${BUILD_DIR}/lcov_test.info --output-directory ${COVERAGE_REPORT_DIR} \
		--demangle-cpp --legend

test: debug
	make -C ${BUILD_DIR} test

debug: clean
	cmake -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=DEBUG ${CMAKE_DEBUG_FLAGS}
	make -j${THREADS_NUM} -C ${BUILD_DIR} all

release: clean
	cmake -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=RELEASE
	make -j${THREADS_NUM} -C ${BUILD_DIR}

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}
