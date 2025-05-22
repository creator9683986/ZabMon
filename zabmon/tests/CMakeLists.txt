cmake_minimum_required(VERSION 3.10)
project(zabmon_tests)

set(CMAKE_CXX_STANDARD 17)

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
include_directories(
    ${CMAKE_SOURCE_DIR}/../src
    ${GENERATED_DIR}
    /usr/include/librdkafka
)
link_directories(/usr/lib/x86_64-linux-gnu)
set(CMAKE_LIBRARY_PATH /usr/lib/x86_64-linux-gnu ${CMAKE_LIBRARY_PATH})

include_directories(${CMAKE_SOURCE_DIR}/../proto/generated)

set(GENERATED_DIR ${CMAKE_SOURCE_DIR}/../proto/generated)

set(TEST_SOURCES
    zabbixapi.cpp
    ${CMAKE_SOURCE_DIR}/src/api/zabbixapi.cpp
    ${CMAKE_SOURCE_DIR}/src/api/jsonrpcmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/exceptions.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/hostgroup.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/host.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/item.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/itemvalues.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/template.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/value.cpp
    ${CMAKE_SOURCE_DIR}/src/structures/interfaces.cpp
    ${GENERATED_DIR}/zabmon.pb.cc
    ${GENERATED_DIR}/zabmon.grpc.pb.cc
)

add_executable(zabmontest ${TEST_SOURCES})
target_link_libraries(zabmontest
nlohmann_json::nlohmann_json
CURL::libcurl
    gRPC::grpc++
    protobuf::libprotobuf
    GTest::GTest
    GTest::Main
    Threads::Threads
    rdkafka
    rdkafka++ 
)
