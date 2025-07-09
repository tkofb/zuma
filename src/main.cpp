#include "parser/futures/futuresTrade.h"
#include "tui/homepage/homepage.h"
#include <string>

using namespace std;

int main(int argc, char * argv[]){

   string filePath = "../src/data/example.csv";
   FuturesInfo info = parseFileForFuturesInfo(filePath);
   printHomepage();
   info.print();

   return 0;
}