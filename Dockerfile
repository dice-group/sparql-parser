FROM ubuntu:focal AS builder
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update && \
    apt-get -qq install -y cmake git python3-pip python3-setuptools python3-wheel g++

# setup conan
RUN pip3 install conan && \
    conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris


# make build dir
RUN mkdir /build

# copy conan and run conan
COPY conanfile.py /conanfile.py
RUN cd build && \
    conan install .. --build=missing

# copy project files except for conanfile (see above)
COPY include /include
COPY include /include
COPY tests /tests
COPY CMakeLists.txt /CMakeLists.txt

# change working directory
WORKDIR /build
# run cmake
RUN cmake -Dsparql-parser_BUILD_TESTS=ON
# build
RUN make -j $(nproc)

CMD ["./tests"]
