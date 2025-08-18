#include "databaseFunctions.h"

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

void getTradeIdsFromDatabase(set<string>& ids) {
    string           connectionString = createConnectionRequest();
    pqxx::connection connectionObject(connectionString.c_str());
    pqxx::work       worker(connectionObject);
    string           stmt = "SELECT id FROM trades;";

    try {
        pqxx::result res = worker.exec(stmt);

        for (auto const& row : res) {
            for (auto const& field : row) {
                ids.insert(field.c_str());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void updateDatabase(vector<FuturesTrade> trades) {
    set<string> ids;
    getTradeIdsFromDatabase(ids);
    string           connectionString = createConnectionRequest();
    pqxx::connection connectionObject(connectionString.c_str());
    pqxx::work       worker(connectionObject);

    for (FuturesTrade trade : trades) {
        if (ids.find(trade.getID()) == ids.end()) {
            addTradeToDatabase("Futures", trade, worker);
        }
    }

    worker.commit();
}

void addTradeToDatabase(string assetType, FuturesTrade trade, pqxx::work& worker) {
    try {
        pqxx::params params;
        params.append({trade.getID(), assetType, trade.getSymbol(), trade.getAccount(),
                       trade.getQuantity(), trade.getEntryPrice(), trade.getClosingPrice(),
                       trade.getRealizedProfitAndLoss(), trade.getMarkToMarketProfitAndLoss(),
                       trade.getCode(), trade.getSignal(), trade.getEntryDate(),
                       trade.getEntryTime()});

        pqxx::result addTradeResponse =
            worker.exec("INSERT INTO trades VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, "
                        "$11, $12, $13)",
                        params);
        worker.commit();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
