#include "parser/futures/futuresTrade.h"
#include <string>

using namespace std;

int main(int argc, char * argv[]){

   string filePath = "../src/data/example.csv";

   // USE IF YOU ARE RUNNING IN ROOT LEVEL
   // string filePath = "../src/data/example.csv";
   
   FuturesInfo info = parseFileForFuturesInfo(filePath);
   info.print();

   return 0;
}