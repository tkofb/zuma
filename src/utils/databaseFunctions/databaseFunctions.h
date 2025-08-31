#include "../../parser/futures/futuresTrade.h"
#include "../dotenvParser/dotenvParser.h"
#include "journalEntry.h"
#include <fmt/core.h>
#include <iostream>
#include <pqxx/pqxx>
#include <set>
#include <string>

using namespace std;

string createConnectionRequest();
void   addTradeToDatabase(string assetType, FuturesTrade trade, pqxx::work& worker);
void   printDatabase();
void   updateDatabase(vector<FuturesTrade> trades);
void   updateJournalEntry(const FuturesTrade& trade, const JournalEntry& journal);
void   getJournalEntry(set<string>& ids);