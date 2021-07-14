#include <iostream>
#include <string>

#include "person-lite.pb.h"
#include "person.pb.h"

using std::cout;

template <typename T>
inline void dumpPerson(const T& person, const std::string& description) {
  cout << "# " << description << "\n";
  cout << std::boolalpha;

  if (person.has_name()) {
    cout << "name: \"" << person.name() << "\"\n";
  } else {
    cout << "'name' field not set\n";
  }

  if (person.has_id()) {
    cout << "id: " << person.id() << "\n";
  } else {
    cout << "'id' field not set\n";
  }

  cout.flush();
}

int main(int argc, char* argv[]) {
  tutorial::Person person;
  dumpPerson(person, "Initial");

  person.set_name("xyz");
  person.set_id(7);
  dumpPerson(person, R"(After set name to "xyz" and id to 7)");

  tutorial::lite::Person person2;
  dumpPerson(person2, "Initial lite person");
  person2.set_name("abc");
  person2.set_id(11);
  dumpPerson(person2, R"(After set name to "abc" and id to 11)");

  std::cout << sizeof(person) << ", " << sizeof(person2) << std::endl;
  return 0;
}
