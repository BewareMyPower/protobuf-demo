// Simulate Pulsar's protocol native schema.
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <iostream>
#include <vector>

#include "Test.pb.h"
#include "hex_dump.h"

using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorSet;
using std::cout;

void internalCollectFileDescriptors(const FileDescriptor* file_descriptor,
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

  return 0;
}
