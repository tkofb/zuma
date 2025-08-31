#include <iostream>
#include <string>


class JournalEntry {
  public:
    shared_ptr<string> title;
    shared_ptr<string> notes;
    shared_ptr<string> lesson;

    JournalEntry(shared_ptr<string> title, shared_ptr<string> notes, shared_ptr<string> lesson);
};