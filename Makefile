###################################
##  	   DEFAULT PART 	     ##
###################################

##############################################
# Try not to change default part's options.  #
# It looks like CI mostly. Add custom        #
# commands in the section below 		     #
##############################################

.PHONY: build static_stage clang-format clang-tidy cpplint cppcheck test coverage default_test
.PHONY: all_tests valgrind_test tsan_test default_build valgrind_build tsan_build default_cmake
.PHONY: valgrind_cmake tsan_cmake clean docker

BUILD_DIR = build
COVERAGE_REPORT_DIR = ${BUILD_DIR}/lcov_html
DOCKER_IMAGE = serpentian/cognitio:latest

CMAKE_CHECK_TOOLS_FLAGS = -DENABLE_CHECK_TOOLS=ON -DENABLE_CLANG_FORMAT=ON -DENABLE_CPPCHECK=ON -DENABLE_CPPLINT=ON -DENABLE_CLANG_TIDY=ON
CMAKE_DEFAULT_SANITIZERS_FLAGS = -DENABLE_VALGRIND=OFF -DENABLE_LSAN=ON -DENABLE_ASAN=ON -DENABLE_UBSAN=ON -DENABLE_TSAN=OFF
CMAKE_VALGRIND_FLAGS = -DENABLE_VALGRIND=ON -DENABLE_LSAN=OFF -DENABLE_ASAN=OFF -DENABLE_UBSAN=OFF -DENABLE_TSAN=OFF
CMAKE_TSAN_FLAGS = -DENABLE_VALGRIND=OFF -DENABLE_LSAN=OFF -DENABLE_ASAN=OFF -DENABLE_UBSAN=OFF -DENABLE_TSAN=ON
CMAKE_DEBUG_DEFAULT_FLAGS = -DCMAKE_BUILD_TYPE=DEBUG -DWARNINGS_AS_ERRORS=ON ${CMAKE_CHECK_TOOLS_FLAGS}
CMAKE_DEBUG_TESTING_FLAGS = -DENABLE_TESTING=ON -DENABLE_COVERAGE=ON

MAKE_CMD = make -C ${BUILD_DIR}
MAKE_TEST_CMD = ${MAKE_CMD} all test ARGS="-VV"
DEFAULT_CMAKE_CMD = cmake -B ${BUILD_DIR} ${CMAKE_DEBUG_DEFAULT_FLAGS} ${CMAKE_DEBUG_TESTING_FLAGS} ${CMAKE_DEFAULT_SANITIZERS_FLAGS} .
VALGRIND_CMAKE_CMD = cmake -B ${BUILD_DIR} ${CMAKE_DEBUG_DEFAULT_FLAGS} ${CMAKE_DEBUG_TESTING_FLAGS} ${CMAKE_VALGRIND_FLAGS} .
TSAN_CMAKE_CMD = cmake -B ${BUILD_DIR} $(pwd) ${CMAKE_DEBUG_DEFAULT_FLAGS} ${CMAKE_DEBUG_TESTING_FLAGS} ${CMAKE_TSAN_FLAGS} .

build: default_build

###################################
#  		    STATIC STAGE 		  #
###################################

static: clang-format clang-tidy cpplint cppcheck

clang-format: default_cmake
	${MAKE_CMD} formatcheck

clang-tidy: default_cmake
	${MAKE_CMD} clang-tidy

cpplint: default_cmake
	${MAKE_CMD} cpplint

cppcheck: default_cmake
	${MAKE_CMD} cppcheck

###################################
#  		    TEST STAGE 		      #
###################################

coverage: default_test
	lcov --capture --directory ${BUILD_DIR} --exclude '*_test*' --output-file ${BUILD_DIR}/lcov_test.info
	genhtml ${BUILD_DIR}/lcov_test.info --output-directory ${COVERAGE_REPORT_DIR} --demangle-cpp --legend

test: default_test
default_test: clean default_cmake .test
valgrind_test: clean valgrind_cmake .test
tsan_test: clean tsan_cmake .test

.test:
	${MAKE_TEST_CMD}

###################################
#  		    BUILD STAGE 		  #
###################################

# In CI builds will be lanched
# under scan-build and fbinfer

default_build: default_cmake
	${MAKE_CMD}

default_cmake:
	${DEFAULT_CMAKE_CMD}

valgrind_cmake:
	${VALGRIND_CMAKE_CMD}

tsan_cmake:
	${TSAN_CMAKE_CMD}

###################################
##  	    CUSTOM PART 	     ##
###################################

clean:
	rm -rf ${BUILD_DIR}

docker:
    # docker build -t ${DOCKER_IMAGE} .
    # docker image pull serpentian/cognitio:latest
	docker run -v $(shell pwd):/home/project -it ${DOCKER_IMAGE}

# Protobuf compiling

# All locations of the proto files
CLI_PROTO_SRC_DIR=proto/cli
DATA_PROTO_SRC_DIR=proto/data

proto: cli-proto

cli-proto:
	protoc -I ${CLI_PROTO_SRC_DIR} --cpp_out=${CLI_PROTO_SRC_DIR} ${CLI_PROTO_SRC_DIR}/*.proto
	mkdir -p include/${CLI_PROTO_SRC_DIR} src/${CLI_PROTO_SRC_DIR}
	mv ${CLI_PROTO_SRC_DIR}/*.pb.h include/${CLI_PROTO_SRC_DIR}
	mv ${CLI_PROTO_SRC_DIR}/*.pb.cc src/${CLI_PROTO_SRC_DIR}

data-proto:
	protoc -I ${DATA_PROTO_SRC_DIR} --cpp_out=${DATA_PROTO_SRC_DIR} ${DATA_PROTO_SRC_DIR}/*.proto
	mkdir -p include/${DATA_PROTO_SRC_DIR} src/${DATA_PROTO_SRC_DIR}
	mv ${DATA_PROTO_SRC_DIR}/*.pb.h include/${DATA_PROTO_SRC_DIR}
	mv ${DATA_PROTO_SRC_DIR}/*.pb.cc src/${DATA_PROTO_SRC_DIR}
