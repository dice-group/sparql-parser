FROM ubuntu:focal AS builder
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update && \
    apt-get -qq install -y make cmake git python3-pip python3-setuptools python3-wheel uuid-dev openjdk-11-jdk  libstdc++-10-dev g++-10  clang-11 pkg-config



# setup conan
RUN pip3 install conan
RUN conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler.libcxx=libstdc++11  default  && \
    conan profile update settings.compiler.version=10 default  && \
    conan profile new --detect clang11 && \
    conan profile update settings.compiler=clang clang11 &&\
    conan profile update settings.compiler.version=11 clang11 && \
    conan profile update settings.compiler.libcxx=libstdc++11 clang11


RUN  conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris


 # copy project files
COPY include /sparql-parser/include
COPY cmake /sparql-parser/cmake
COPY tests /sparql-parser/tests
COPY CMakeLists.txt /sparql-parser/CMakeLists.txt
COPY conanfile.py /sparql-parser/conanfile.py


#gcc
WORKDIR /sparql-parser/build_gcc

ENV CXX="g++-10"
ENV CC="gcc-10"

RUN conan install .. --build=missing  -o sparql-parser:with_tests=True

## change working directory
WORKDIR /sparql-parser/build_gcc
#
# run cmake
RUN CC=gcc-10 CXX=g++-10 cmake -DCMAKE_BUILD_TYPE=Release -DSPARQL_PARSER_BUILD_TESTS=ON ..
# build
RUN make -j $(nproc)

#for clang
## change working directory
WORKDIR /sparql-parser/build_clang

ENV CXX="clang++-11"
ENV CC="clang-11"

RUN conan install .. --build=missing --profile clang11 -o sparql-parser:with_tests=True
# run cmake
RUN CC=clang-11 CXX=clang++-11 cmake -DCMAKE_BUILD_TYPE=Release -DSPARQL_PARSER_BUILD_TESTS=ON ..
# build
RUN make -j $(nproc)

WORKDIR /sparql-parser/build_clang
RUN ./tests/tests

WORKDIR /sparql-parser/build_gcc
RUN ./tests/tests