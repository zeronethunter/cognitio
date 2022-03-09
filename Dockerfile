# Dockerfile for template project
FROM gcc

RUN echo "Installing dependencies..."
RUN apt-get update
RUN apt-get install -y clang clang-format clang-tidy \
                   cmake cppcheck curl gcc make python3 \
                   python3-pip tar valgrind vim lcov libcmocka-dev

RUN pip install cpplint scan-build

RUN echo "Installing dependencies not found in the package repos..."
RUN VERSION=1.1.0; \
    curl -sSL "https://github.com/facebook/infer/releases/download/v$VERSION/infer-linux64-v$VERSION.tar.xz" \
    | tar -C /opt -xJ && ln -s "/opt/infer-linux64-v$VERSION/bin/infer" /usr/local/bin/infer
