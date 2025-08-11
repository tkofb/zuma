#include "../../parser/futures/futuresTrade.h"
#include <iostream>
#include <string>

using namespace std;

string createConnectionRequest();
void   addTradeToDatabase(string assetType, FuturesTrade trade);
void   printDatabase();