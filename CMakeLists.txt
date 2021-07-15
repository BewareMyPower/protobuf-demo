cmake_minimum_required(VERSION 3.10)
project(protobuf-demo CXX)

set(CMAKE_CXX_STANDARD 14)

if (VCPKG_TRIPLET)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TRIPLET}")
    message(STATUS "VCPKG_TRIPLET: ${VCPKG_TRIPLET}")
    message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
endif ()

find_package(Protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR})

set(PROTO_SRC_PERSON person.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_PERSON} person.pb.h ./proto/person.proto)

set(PROTO_SRC_PERSON_LITE person-lite.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_PERSON_LITE} person-lite.pb.h ./proto/person-lite.proto)

set(PROTO_SRC_PULSAR_EXTERNAL_MESSAGE pulsar_external_message.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_PULSAR_EXTERNAL_MESSAGE} pulsar_external_message.pb.h ./proto/pulsar_external_message.proto)

set(PROTO_SRC_PULSAR_MESSAGE pulsar_message.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_PULSAR_MESSAGE} pulsar_message.pb.h ./proto/pulsar_message.proto)

set(PROTO_SRC_EXTERNAL_TEST ExternalTest.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_EXTERNAL_TEST} ExternalTest.pb.h ./proto/ExternalTest.proto)

set(PROTO_SRC_TEST Test.pb.cc)
protobuf_generate_cpp(${PROTO_SRC_TEST} Test.pb.h ./proto/Test.proto)

add_executable(person examples/person.cc ${PROTO_SRC_PERSON} ${PROTO_SRC_PERSON_LITE})
target_link_libraries(person protobuf::libprotobuf)

add_executable(pulsar_message examples/pulsar_message.cc ${PROTO_SRC_PULSAR_MESSAGE} ${PROTO_SRC_PULSAR_EXTERNAL_MESSAGE})
target_link_libraries(pulsar_message protobuf::libprotobuf)

add_executable(schema examples/schema.cc ${PROTO_SRC_TEST} ${PROTO_SRC_EXTERNAL_TEST})
target_link_libraries(schema protobuf::libprotobuf)

find_program(CLANG_FORMAT_BIN NAMES clang-format)
if (CLANG_FORMAT_BIN)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_BIN}")
    file(GLOB_RECURSE ALL_SOURCE_FILES examples/*.cc examples/*.h)
    message(STATUS "ALL_SOURCE_FILES: ${ALL_SOURCE_FILES}")
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_SOURCE_FILES})
    add_dependencies(person format)
endif ()
