FROM ubuntu:latest

RUN echo "Updating repositories..."
RUN apt-get update -y

RUN echo "Installing dependencies..."
RUN apt-get install -y clang clang-format clang-tidy cmake cppcheck curl make python3 python3-pip tar \
                    valgrind vim lcov doxygen libgtest-dev libgrpc++-dev grpc-proto git

RUN echo "Installing gRPC & Protobuf..."
RUN apt-get install -y protobuf-compiler protobuf-compiler-grpc libprotobuf23

RUN echo "Installing other dependecies..."
RUN apt-get install -y libspdlog-dev libboost-all-dev
RUN pip install cpplint scan-build

RUN echo "Installing dependencies not found in the package repos..."
RUN VERSION=1.1.0; \
    curl -sSL "https://github.com/facebook/infer/releases/download/v$VERSION/infer-linux64-v$VERSION.tar.xz" \
    | tar -C /opt -xJ && ln -s "/opt/infer-linux64-v$VERSION/bin/infer" /usr/local/bin/infer
