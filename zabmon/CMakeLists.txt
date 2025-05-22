cmake_minimum_required(VERSION 3.10)
project(zabmon)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
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
    ${SRC_DIR}/exceptions.cpp
    ${SRC_DIR}/grpcadapter.cpp
    ${SRC_DIR}/monitor.cpp
    ${SRC_DIR}/service.cpp
    ${SRC_DIR}/api/jsonrpcmanager.cpp
    ${SRC_DIR}/api/zabbixapi.cpp
    ${SRC_DIR}/structures/host.cpp
    ${SRC_DIR}/structures/hostgroup.cpp
    ${SRC_DIR}/structures/interfaces.cpp
    ${SRC_DIR}/structures/item.cpp
    ${SRC_DIR}/structures/itemvalues.cpp
    ${SRC_DIR}/structures/template.cpp
    ${SRC_DIR}/structures/value.cpp
    ${SRC_DIR}/structures/itemstatus.h
    ${GENERATED_DIR}/zabmon.pb.cc
    ${GENERATED_DIR}/zabmon.grpc.pb.cc
)

add_executable(zabmon ${SOURCES})
add_subdirectory(tests)

target_link_libraries(zabmon
nlohmann_json::nlohmann_json
CURL::libcurl
    gRPC::grpc++
    grpc
    gpr             
    absl::synchronization
    protobuf::libprotobuf
    pqxx
    pq
    Threads::Threads
)
