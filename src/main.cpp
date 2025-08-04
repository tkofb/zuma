#include "parser/futures/futuresTrade.h"
#include "tui/homepage/homepage.h"
#include "utils/dotenvParser/dotenvParser.h"
#include <iostream>
#include <pqxx/pqxx>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  std::string filePath = "../src/data/example.csv";
  FuturesInfo info = parseFileForFuturesInfo(filePath);
  printHomepage(info);
  string key = "DB_HOST_FAIL";

  try {
    dotenv(key);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
