#include <iostream>
#include <string>

#include "person.pb.h"

using std::cout;

inline void dumpPerson(const tutorial::Person& person,
                       const std::string& description) {
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
  return 0;
}
