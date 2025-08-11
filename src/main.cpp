#include "parser/futures/futuresTrade.h"
#include "tui/homepage/homepage.h"
#include "utils/databaseFunctions/databaseFunctions.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    std::string filePath = "../src/data/example.csv";
    FuturesInfo info     = parseFileForFuturesInfo(filePath);
    updateDatabase(info.getTrades());
    printHomepage(info);

    return 0;
}
