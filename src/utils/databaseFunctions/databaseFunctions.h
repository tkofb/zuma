#include "../../parser/futures/futuresTrade.h"
#include "../dotenvParser/dotenvParser.h"
#include <fmt/core.h>
#include <iostream>
#include <pqxx/pqxx>
#include <set>
#include <string>

using namespace std;

class JournalEntry {
  public:
    shared_ptr<string> title;
    shared_ptr<string> notes;
    shared_ptr<string> lesson;

    JournalEntry(shared_ptr<string> title, shared_ptr<string> notes, shared_ptr<string> lesson);
};

string createConnectionRequest();
void   addTradeToDatabase(string assetType, FuturesTrade trade, pqxx::work& worker);
void   printDatabase();
void   updateDatabase(vector<FuturesTrade> trades);
void   updateJournalEntry(const FuturesTrade& trade, const JournalEntry& journal);