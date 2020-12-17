FROM ubuntu:focal AS builder
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update && \
    apt-get -qq install -y cmake git python3-pip python3-setuptools python3-wheel g++



# make build dir
RUN mkdir /Sparql-parser
RUN mkdir /Sparql-parser/build


# setup conan
RUN pip3 install conan && \
    conan user && \
    conan profile new --detect default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan remote add dice-group https://api.bintray.com/conan/dice-group/tentris


# copy conan and run conan
COPY conanfile.py /Sparql-parser/conanfile.py
RUN cd Sparql-parser/build && \
    conan install .. --build=missing

 # copy project files except for conanfile (see above)
 COPY include /Sparql-parser/include
 COPY cmake /Sparql-parser/cmake
 COPY tests /Sparql-parser/tests
 COPY CMakeLists.txt /Sparql-parser/CMakeLists.txt

# change working directory
WORKDIR /Sparql-parser/build
# run cmake
RUN cmake -Dsparql-parser_BUILD_TESTS=OFF ..
# build
RUN make -j $(nproc)

CMD ["./tests"]
