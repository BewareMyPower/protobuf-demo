#include <google/protobuf/descriptor.h>

#include <iostream>

#include "pulsar_message.pb.h"

using std::cout;

inline std::string to_string(const pulsar::SubMessage& sub_msg) {
  return "SubMessage{ .foo=\"" + sub_msg.foo() +
         "\", .bar=" + std::to_string(sub_msg.bar()) + " }";
}

inline std::string to_string(
    const pulsar::external::ExternalMessage& external_msg) {
  return "ExternalMessage{ .stringField=\"" + external_msg.stringfield() +
         "\", .doubleField=" + std::to_string(external_msg.doublefield()) +
         " }";
}

void showTestMessageExample();

void descriptorExample();

int main(int argc, char* argv[]) {
  showTestMessageExample();
  descriptorExample();
  return 0;
}

void showTestMessageExample() {
  cout << "# showTestMessageExample\n";
  pulsar::TestMessage test_message;

  // 1. Initialize
  test_message.set_stringfield("MyTestMessage");
  test_message.set_doublefield(1.29);
  test_message.set_intfield(8);
  test_message.set_testenum(pulsar::TestEnum::SHARED);

  pulsar::SubMessage sub_message;
  sub_message.set_foo("SubFoo");
  sub_message.set_bar(3.14);
  test_message.set_allocated_nestedfield(&sub_message);

  constexpr int numRepeated = 5;
  test_message.mutable_repeatedfield()->Reserve(numRepeated);
  for (int i = 0; i < numRepeated; i++) {
    test_message.add_repeatedfield("repeated-" + std::to_string(i));
  }

  test_message.set_allocated_externalmessage(
      new pulsar::external::ExternalMessage);
  test_message.mutable_externalmessage()->set_stringfield(
      "MyExternalTestMessage");
  test_message.mutable_externalmessage()->set_doublefield(-9.21);

  // 2. Print all fields
  cout << "stringField: \"" << test_message.stringfield() << "\"\n";
  cout << "doubleField: " << test_message.doublefield() << "\n";
  cout << "intField: " << test_message.intfield() << "\n";
  cout << "testNum: " << pulsar::TestEnum_Name(test_message.testenum()) << "\n";
  cout << to_string(test_message.nestedfield()) << "\n";
  cout << "repeatedField:\n";
  for (int i = 0; i < test_message.repeatedfield_size(); i++) {
    cout << "[" << i << "] " << test_message.repeatedfield(i) << "\n";
  }
  cout << to_string(test_message.externalmessage()) << "\n";

  // 3. We must release the nestedField, otherwise the pointer will be deleted
  // by Protobuf automatically.
  test_message.release_nestedfield();
}

void descriptorExample() {
  cout << "# descriptorExample\n";
  const auto descriptor = pulsar::TestMessage::GetDescriptor();
  cout << "name: " << descriptor->name() << "\n";
  cout << "full_name: " << descriptor->full_name() << "\n";
  cout << "index: " << descriptor->index() << "\n";
  cout << "DebugString: " << descriptor->DebugString() << "\n";
  cout.flush();
}
