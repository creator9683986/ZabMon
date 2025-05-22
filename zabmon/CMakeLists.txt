cmake_minimum_required(VERSION 3.10)
project(zabmon)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(absl REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../proto/generated")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(${SRC_DIR})
include_directories(${GENERATED_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${GENERATED_DIR}/zabmon.pb.cc
    ${GENERATED_DIR}/zabmon.grpc.pb.cc
)

add_executable(zabmon ${SOURCES})

target_link_libraries(zabmon
    gRPC::grpc++
    grpc
    gpr             
    absl::synchronization
    protobuf::libprotobuf
    pqxx
    pq
    Threads::Threads
)
