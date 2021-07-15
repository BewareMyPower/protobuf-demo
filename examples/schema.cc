// Simulate Pulsar's protocol native schema.
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <iostream>
#include <set>
#include <vector>

#include "hex_dump.h"
#include "pulsar_message.pb.h"

using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorSet;
using std::cout;

void internalSerializeFileDescriptor(const FileDescriptor* file_descriptor,
                                     FileDescriptorSet& file_descriptor_set,
                                     std::set<std::string>& cache) {
  if (cache.find(file_descriptor->name()) != cache.cend()) {
    return;
  }
  for (int i = 0; i < file_descriptor->dependency_count(); i++) {
    const auto dependency = file_descriptor->dependency(i);
    if (cache.find(dependency->name()) == cache.cend()) {
      internalSerializeFileDescriptor(dependency, file_descriptor_set, cache);
    }
  }
  file_descriptor->CopyTo(file_descriptor_set.add_file());
  // mark current file_descriptor as added to avoid repeated class name
  cache.emplace(file_descriptor->name());
}

FileDescriptorSet serializeFileDescriptor(
    const FileDescriptor* file_descriptor) {
  FileDescriptorSet file_descriptor_set;
  std::set<std::string> cache;
  internalSerializeFileDescriptor(file_descriptor, file_descriptor_set, cache);
  return file_descriptor_set;
}

int main(int argc, char* argv[]) {
  const auto descriptor = pulsar::TestMessage::GetDescriptor();
  const auto file_descriptor = descriptor->file();

  const FileDescriptorSet file_descriptor_set =
      serializeFileDescriptor(file_descriptor);
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

  return 0;
}
