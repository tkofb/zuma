#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include "parser/futures/futuresTrade.h"

using namespace std;


int main() {
   string filePath = "src\\data\\example.csv";
   FuturesInfo info = parseFileForFuturesInfo(filePath);
   info.print();

   return 0;
}