// Simulate Pulsar's protocol native schema.
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <iostream>
#include <vector>

#include "Test.pb.h"
#include "hex_dump.h"

using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorSet;
using std::cout;

inline void internalCollectFileDescriptors(
    const FileDescriptor* file_descriptor,
    FileDescriptorSet& file_descriptor_set) {
  file_descriptor->CopyTo(file_descriptor_set.add_file());
  for (int i = 0; i < file_descriptor->dependency_count(); i++) {
    // collect the file descriptors recursively
    internalCollectFileDescriptors(file_descriptor->dependency(i),
                                   file_descriptor_set);
  }
}

FileDescriptorSet collectFileDescriptors(
    const FileDescriptor* file_descriptor) {
  FileDescriptorSet file_descriptor_set;
  internalCollectFileDescriptors(file_descriptor, file_descriptor_set);
  return file_descriptor_set;
}

int main(int argc, char* argv[]) {
  const auto descriptor = proto::TestMessage::GetDescriptor();
  const auto file_descriptor = descriptor->file();

  const FileDescriptorSet file_descriptor_set =
      collectFileDescriptors(file_descriptor);
  const auto root_message_type_name = descriptor->full_name();
  const auto root_file_descriptor_name = descriptor->file()->name();

  cout << "file description set's file count: "
       << file_descriptor_set.file_size() << "\n";
  for (int i = 0; i < file_descriptor_set.file_size(); i++) {
    cout << "[" << i << "] " << file_descriptor_set.file(i).name() << "\n";
  }

  std::vector<char> bytes(file_descriptor_set.ByteSizeLong());
  file_descriptor_set.SerializeToArray(bytes.data(), bytes.size());
  cout << "serialized to " << bytes.size() << " bytes:\n";
  cout << dumpBytes(bytes.data(), bytes.size(), 16) << "\n";

  cout << "root message type name: " << root_message_type_name << "\n";
  cout << "root file descriptor name: " << root_file_descriptor_name << "\n";

  using namespace boost::archive::iterators;
  using base64 = base64_from_binary<transform_width<const char*, 6, 8>>;

  const std::string schema_json =
      R"({"FileDescriptorSet":")" +
      std::string(base64(bytes.data()), base64(bytes.data() + bytes.size())) +
      R"(","rootMessageTypeName":")" + root_message_type_name +
      R"(","rootFileDescriptorName":")" + root_file_descriptor_name + R"("})";
  cout << schema_json << "\n";

  // The same of EXPECTED_SCHEMA_JSON in
  // https://github.com/apache/pulsar/blob/master/pulsar-client/src/test/java/org/apache/pulsar/client/impl/schema/ProtobufNativeSchemaTest.java
  const std::string expected_schema_json =
      "{\"FileDescriptorSet\":"
      "\"CtMDCgpUZXN0LnByb3RvEgVwcm90bxoSRXh0ZXJuYWxUZXN0LnByb3RvImUKClN1Yk1lc3"
      "NhZ2U"
      "SCwoDZm9vGAEgASgJEgsKA2JhchgCIAEoARo9Cg1OZXN0ZWRNZXNzYWdlEgsKA3VybBgBIAE"
      "oCRINCgV0aXRsZRgCIAEoCRIQCghzbmlwcGV0cxgDIAMoCSLlAQoLV"
      "GVzdE1lc3NhZ2USEwoLc3RyaW5nRmllbGQYASABKAkSEwoLZG91YmxlRmllbGQYAiABKAESE"
      "AoIaW50RmllbGQYBiABKAUSIQoIdGVzdEVudW0YBCABKA4yDy5wcm90by5U"
      "ZXN0RW51bRImCgtuZXN0ZWRGaWVsZBgFIAEoCzIRLnByb3RvLlN1Yk1lc3NhZ2USFQoNcmVw"
      "ZWF0ZWRGaWVsZBgKIAMoCRI4Cg9leHRlcm5hbE1lc3NhZ2UYCyABKAsyHy5wcm90by"
      "5leHRlcm5hbC5FeHRlcm5hbE1lc3NhZ2UqJAoIVGVzdEVudW0SCgoGU0hBUkVEEAASDAoIRk"
      "FJTE9WRVIQAUItCiVvcmcuYXBhY2hlLnB1bHNhci5jbGllbnQuc2NoZW1hLnByb3"
      "RvQgRUZXN0YgZwcm90bzMKoAEKEkV4dGVybmFsVGVzdC5wcm90bxIOcHJvdG8uZXh0ZXJuYW"
      "wiOwoPRXh0ZXJuYWxNZXNzYWdlEhMKC3N0cmluZ0ZpZWxkGAEgA"
      "SgJEhMKC2RvdWJsZUZpZWxkGAIgASgBQjUKJW9yZy5hcGFjaGUucHVsc2FyLmNsaWVudC5zY"
      "2hlbWEucHJvdG9CDEV4dGVybmFsVGVzdGIGcHJvdG8z\","
      "\"rootMessageTypeName\":\"proto.TestMessage\","
      "\"rootFileDescriptorName\":\"Test.proto\"}";
  cout << expected_schema_json << "\n";
  if (schema_json == expected_schema_json) {
    cout << "OK\n";
  } else {
    cout << "Failed\n";
    if (schema_json.size() != expected_schema_json.size()) {
      cout << "size is not equal: " << schema_json.size() << " vs. "
           << expected_schema_json.size() << "\n";
    }
    for (size_t i = 0;
         i < std::min(schema_json.size(), expected_schema_json.size()); i++) {
      if (schema_json[i] != expected_schema_json[i]) {
        cout << i << " " << schema_json[i] << " != " << expected_schema_json[i]
             << "\n";
      }
    }
  }

  return 0;
}
