cmake_minimum_required(VERSION 3.10)
project(protobuf-demo CXX)

set(CMAKE_CXX_STANDARD 11)

if (VCPKG_TRIPLET)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TRIPLET}")
    message(STATUS "VCPKG_TRIPLET: ${VCPKG_TRIPLET}")
    message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
endif ()

find_package(Protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR})

# person.cc
set(PROTO_SRC_PERSON person.pb.cc)
set(PROTO_HDR_PERSON person.pb.h)
protobuf_generate_cpp(${PROTO_SRC_PERSON} ${PROTO_HDR_PERSON} ./proto/person.proto)

set(PROTO_SRC_PERSON_LITE person-lite.pb.cc)
set(PROTO_HDR_PERSON_LITE person-lite.pb.h)
protobuf_generate_cpp(${PROTO_SRC_PERSON_LITE} ${PROTO_HDR_PERSON_LITE} ./proto/person-lite.proto)

add_executable(person examples/person.cc ${PROTO_SRC_PERSON} ${PROTO_SRC_PERSON_LITE})
target_link_libraries(person protobuf::libprotobuf)

# pulsar_message.cc
set(PROTO_SRC_PULSAR_EXTERNAL_MESSAGE pulsar_external_message.pb.cc)
set(PROTO_HDR_PULSAR_EXTERNAL_MESSAGE pulsar_external_message.pb.h)
protobuf_generate_cpp(${PROTO_SRC_PULSAR_EXTERNAL_MESSAGE} ${PROTO_HDR_PULSAR_EXTERNAL_MESSAGE} ./proto/pulsar_external_message.proto)

set(PROTO_SRC_PULSAR_MESSAGE pulsar_message.pb.cc)
set(PROTO_HDR_PULSAR_MESSAGE pulsar_message.pb.h)
protobuf_generate_cpp(${PROTO_SRC_PULSAR_MESSAGE} ${PROTO_HDR_PULSAR_MESSAGE} ./proto/pulsar_message.proto)

add_executable(pulsar_message examples/pulsar_message.cc ${PROTO_SRC_PULSAR_MESSAGE} ${PROTO_SRC_PULSAR_EXTERNAL_MESSAGE})
target_link_libraries(pulsar_message protobuf::libprotobuf)

find_program(CLANG_FORMAT_BIN NAMES clang-format)
if (CLANG_FORMAT_BIN)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_BIN}")
    file(GLOB_RECURSE ALL_SOURCE_FILES examples/*.cc examples/*.h)
    message(STATUS "ALL_SOURCE_FILES: ${ALL_SOURCE_FILES}")
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_SOURCE_FILES})
    add_dependencies(person format)
endif ()
