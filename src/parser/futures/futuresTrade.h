#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string         getAssetCategory(string& line);
vector<string> parseFuturesTrade(const string& line);

class FuturesTrade {
    string id;
    string currency;
    string account;
    string symbol;
    string signal;
    string date;
    string time;
    int    quantity;
    double transactionPrice;
    double closingPrice;
    double notionalValue;
    double commission;
    double basis;
    double realizedProfitAndLoss;
    double markToMarketProfitAndLoss;
    string code;

  public:
    FuturesTrade(vector<string> fields);
    double getRealizedProfitAndLoss() const;
    double getMarkToMarketProfitAndLoss() const;
    string getCode();
    void   print() const;

    string getID() const;
    string getSymbol() const;
    string determineSignal(double entryPrice, double closingPrice,
                           double realizedProfitAndLoss) const;
    string getSignal() const;
    string getEntryDate() const;
    string getEntryTime() const;
    string getAccount() const;
    int    getQuantity() const;
    double getEntryPrice() const;
    double getClosingPrice() const;
};

class FuturesInfo {
  public:
    class Subtotal {
        int    quantity;
        double realizedProfitAndLoss;
        double markToMarketProfitAndLoss;
        string symbol;

      public:
        Subtotal(string symbol, int quantity, double realized, double markToMarket);
        int    getQuantity() const;
        double getRealizedPL() const;
        double getMTMPL() const;
        string getSymbol() const;
        void   print() const;
    };

  private:
    vector<FuturesTrade> trades;
    vector<Subtotal>     subtotals;
    double               realizedProfitAndLoss;
    double               markToMarketProfitAndLoss;
    int                  profitableTrades;
    int                  unprofitableTrades;
    double               gainsFromTrades;
    double               lossesFromTrades;
    double               avgWin;
    double               avgLoss;
    int                  totalTrades;
    double               winRate;

  public:
    FuturesInfo();
    FuturesInfo(vector<FuturesTrade> trades, vector<Subtotal> subtotals, double realized,
                double markToMarket);
    void                 updateNumberOfProfitableTrades();
    void                 print();
    vector<FuturesTrade> getTrades() const;
};

FuturesInfo parseFileForFuturesInfo(string filePath);
