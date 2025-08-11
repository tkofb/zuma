#include "../../parser/futures/futuresTrade.h"
#include "../dotenvParser/dotenvParser.h"
#include <fmt/core.h>
#include <iostream>
#include <pqxx/pqxx>

using namespace std;

string createConnectionRequest() {
    string connectionString =
        fmt::format("host={} port={} dbname={} user={} password={}", dotenv("DB_HOST"),
                    dotenv("DB_PORT"), dotenv("DB_NAME"), dotenv("DB_USER"), dotenv("DB_PASS"));

    return connectionString;
}

void printDatabase() {
    string           connectionString = createConnectionRequest();
    pqxx::connection connectionObject(connectionString.c_str());
    pqxx::work       worker(connectionObject);
    string           stmt = "SELECT * FROM trades;";

    try {
        pqxx::result res = worker.exec(stmt);

        for (auto const& row : res) {
            for (auto const& field : row)
                std::cout << field.c_str() << '\t';
            std::cout << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void addTradeToDatabase(string assetType, FuturesTrade trade) {
    string connectionString = createConnectionRequest();

    cout << connectionString << endl;
    pqxx::connection connectionObject(connectionString.c_str());
    cout << connectionObject.is_open() << endl;

    pqxx::work   worker(connectionObject);
    stringstream stmt;

    stmt << "INSERT INTO trades VALUES ('" << trade.getID() << "','" << assetType << "','"
         << trade.getSymbol() << "','" << trade.getAccount() << "','" << trade.getQuantity()
         << "','" << trade.getEntryPrice() << "','" << trade.getClosingPrice() << "','"
         << trade.getRealizedProfitAndLoss() << "','" << trade.getMarkToMarketProfitAndLoss()
         << "','" << trade.getCode() << "','" << trade.getSignal() << "','" << trade.getEntryDate()
         << "','" << trade.getEntryTime() << "');";

    try {

        pqxx::result addTradeResponse = worker.exec(stmt.str());
        worker.commit();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Exception on statement:[" << stmt.str() << "]\n";
        return;
    }
}
