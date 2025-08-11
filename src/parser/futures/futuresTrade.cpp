#include "futuresTrade.h"

string getAssetCategory(string& line) {
    string lineHolder = line;
    string result     = "";
    for (size_t i = 0; i < 4; i++) {
        size_t loc = lineHolder.find(',');
        result     = lineHolder.substr(0, loc);
        lineHolder = lineHolder.substr(loc + 1);
    }
    return result;
}

vector<string> parseFuturesTrade(const string& line) {
    vector<string> result;
    string         field;
    bool           inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }

    result.push_back(field);
    return result;
}

// ========== FuturesTrade ==========

string createTradeID(const string& symbol, const string& date, const string& time,
                     const string& account, const string& mtm_profit) {
    string hash =
        std::to_string(std::hash<string>()(symbol)) + std::to_string(std::hash<string>()(date)) +
        std::to_string(std::hash<string>()(time)) + std::to_string(std::hash<string>()(account)) +
        std::to_string(std::hash<string>()(mtm_profit));

    hash = std::to_string(std::hash<string>()(hash));
    return hash;
}

FuturesTrade::FuturesTrade(vector<string> fields) {
    id = createTradeID(fields[6], fields[7], fields[8], fields[5], fields[15]);
    cout << "Trade ID: " << id << endl;
    currency                  = fields[4];
    account                   = fields[5];
    symbol                    = fields[6];
    quantity                  = stoi(fields[8]);
    transactionPrice          = stod(fields[9]);
    closingPrice              = stod(fields[10]);
    notionalValue             = stod(fields[11]);
    commission                = stod(fields[12]);
    basis                     = stod(fields[13]);
    realizedProfitAndLoss     = stod(fields[14]);
    markToMarketProfitAndLoss = stod(fields[15]);
    code                      = fields[16];
    signal = determineSignal(transactionPrice, closingPrice, realizedProfitAndLoss);

    string datetime  = fields[7];
    string delimiter = ", ";
    date             = datetime.substr(0, datetime.find(delimiter));
    time = datetime.substr(datetime.find(delimiter) + delimiter.length(), datetime.length());
}

double FuturesTrade::getRealizedProfitAndLoss() const {
    return realizedProfitAndLoss;
}
double FuturesTrade::getMarkToMarketProfitAndLoss() const {
    return markToMarketProfitAndLoss;
}

string FuturesTrade::getCode() {
    return code;
}
vector<FuturesTrade> FuturesInfo::getTrades() const {
    return trades;
}
string FuturesTrade::getSymbol() const {
    return symbol;
}
string FuturesTrade::determineSignal(double entryPrice, double closingPrice,
                                     double realizedProfitAndLoss) const {
    if ((closingPrice >= entryPrice && realizedProfitAndLoss >= 0) ||
        (closingPrice < entryPrice && realizedProfitAndLoss < 0)) {
        return "BUY";
    } else {
        return "SELL";
    }
}
string FuturesTrade::getSignal() const {
    return signal;
}
string FuturesTrade::getID() const {
    return id;
}
string FuturesTrade::getEntryDate() const {
    return date;
}
string FuturesTrade::getEntryTime() const {
    return time;
}
string FuturesTrade::getAccount() const {
    return account;
}
int FuturesTrade::getQuantity() const {
    return abs(quantity);
}
double FuturesTrade::getEntryPrice() const {
    return transactionPrice;
}
double FuturesTrade::getClosingPrice() const {
    return closingPrice;
}

void FuturesTrade::print() const {
    string side = quantity > 0 ? "BUY" : "SELL";
    cout << "[" << date << ", " << time << "] " << symbol << " | " << side << " " << abs(quantity)
         << " @ " << transactionPrice << " | "
         << "P/L: " << realizedProfitAndLoss << " | "
         << "Code: " << code << endl;
}

// ========== FuturesInfo::Subtotal ==========

FuturesInfo::Subtotal::Subtotal(string symbol, int quantity, double realized, double markToMarket)
    : symbol(symbol), quantity(quantity), realizedProfitAndLoss(realized),
      markToMarketProfitAndLoss(markToMarket) {}

int FuturesInfo::Subtotal::getQuantity() const {
    return quantity;
}

double FuturesInfo::Subtotal::getRealizedPL() const {
    return realizedProfitAndLoss;
}

double FuturesInfo::Subtotal::getMTMPL() const {
    return markToMarketProfitAndLoss;
}

string FuturesInfo::Subtotal::getSymbol() const {
    return symbol;
}

void FuturesInfo::Subtotal::print() const {
    cout << "Qty: " << quantity << " | Symbol: " << symbol
         << " | Realized P/L: " << realizedProfitAndLoss
         << " | MTM P/L: " << markToMarketProfitAndLoss << endl;
}

// ========== FuturesInfo ==========

FuturesInfo::FuturesInfo()
    : realizedProfitAndLoss(0), markToMarketProfitAndLoss(0), profitableTrades(0),
      unprofitableTrades(0), gainsFromTrades(0), lossesFromTrades(0), avgWin(0), avgLoss(0),
      totalTrades(0), winRate(0.0) {}

FuturesInfo::FuturesInfo(vector<FuturesTrade> trades, vector<Subtotal> subtotals, double realized,
                         double markToMarket)
    : trades(trades), subtotals(subtotals), realizedProfitAndLoss(realized),
      markToMarketProfitAndLoss(markToMarket), profitableTrades(0), unprofitableTrades(0),
      gainsFromTrades(0), lossesFromTrades(0), avgWin(0), avgLoss(0), totalTrades(0), winRate(0.0) {
    updateNumberOfProfitableTrades();
}

void FuturesInfo::updateNumberOfProfitableTrades() {
    for (FuturesTrade& trade : trades) {
        double pnl = trade.getRealizedProfitAndLoss();
        if (pnl < 0) {
            unprofitableTrades++;
            lossesFromTrades += pnl;
        } else {
            profitableTrades++;
            gainsFromTrades += pnl;
        }
    }
    if (unprofitableTrades != 0) {
        avgLoss = lossesFromTrades / unprofitableTrades;
    }
    if (profitableTrades != 0) {
        avgWin = gainsFromTrades / profitableTrades;
    }
    totalTrades = profitableTrades + unprofitableTrades;
    winRate     = totalTrades > 0 ? static_cast<double>(profitableTrades) / totalTrades : 0.0;
}

void FuturesInfo::print() {
    for (FuturesTrade& trade : trades) {
        trade.print();
    }
    for (Subtotal& subtotal : subtotals) {
        subtotal.print();
    }
    cout << "Total"
         << " | Realized P/L: " << realizedProfitAndLoss
         << " | MTM P/L: " << markToMarketProfitAndLoss << " | Win Rate: " << winRate
         << " | AVG Loss: " << avgLoss << " | AVG Win: " << avgWin
         << " | Profitable Trades: " << profitableTrades
         << " | Unprofitable Trades: " << unprofitableTrades << endl;
}

FuturesInfo parseFileForFuturesInfo(string filePath) {
    string      line;
    ifstream    iFile;
    FuturesInfo result;

    iFile.open(filePath);
    if (!iFile.is_open()) {
        cout << "Failed to open file: " << filePath << endl;
        return result;
    }

    vector<FuturesTrade>          listOfFuturesTrades;
    vector<FuturesInfo::Subtotal> listOfFuturesSubtotals;

    while (getline(iFile, line)) {
        size_t location      = line.find(',');
        string statementType = line.substr(0, location);

        if (statementType == "Trades") {
            string assetType = getAssetCategory(line);

            if (assetType == "Futures") {
                vector<string> parsedTrade = parseFuturesTrade(line);
                double         realized    = stod(parsedTrade[14]);
                double         mark        = stod(parsedTrade[15]);

                if (parsedTrade[2] == "Order") {
                    FuturesTrade trade(parsedTrade);

                    if (trade.getCode().find('C') != string::npos) {
                        listOfFuturesTrades.push_back(trade);
                    }
                } else if (parsedTrade[1] == "SubTotal") {
                    string                symbol   = parsedTrade[5];
                    int                   quantity = stoi(parsedTrade[8]);
                    FuturesInfo::Subtotal subtotal(symbol, quantity, realized, mark);
                    listOfFuturesSubtotals.push_back(subtotal);
                } else if (parsedTrade[1] == "Total") {
                    FuturesInfo info(listOfFuturesTrades, listOfFuturesSubtotals, realized, mark);
                    result = info;
                }
            }
        }
    }

    return result;
}
