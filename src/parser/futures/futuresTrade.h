#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string getAssetCategory(string &line);
vector<string> parseFuturesTrade(const string &line);

class FuturesTrade {
  string currency;
  string account;
  string symbol;
  string datetime;
  double quantity;
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
  string getCode();
  void print() const;

  string getSymbol() const;
  string getEntryDate() const;
  string getAccount() const;
  double getQuantity() const;
  double getEntryPrice() const;
  double getClosingPrice() const;
};

class FuturesInfo {
public:
  class Subtotal {
    double quantity;
    double realizedProfitAndLoss;
    double markToMarketProfitAndLoss;
    string symbol;

  public:
    Subtotal(string symbol, double quantity, double realized,
             double markToMarket);
    double getQuantity() const;
    double getRealizedPL() const;
    double getMTMPL() const;
    string getSymbol() const;
    void print() const;
  };

private:
  vector<FuturesTrade> trades;
  vector<Subtotal> subtotals;
  double realizedProfitAndLoss;
  double markToMarketProfitAndLoss;
  int profitableTrades;
  int unprofitableTrades;
  double gainsFromTrades;
  double lossesFromTrades;
  double avgWin;
  double avgLoss;
  int totalTrades;
  double winRate;

public:
  FuturesInfo();
  FuturesInfo(vector<FuturesTrade> trades, vector<Subtotal> subtotals,
              double realized, double markToMarket);
  void updateNumberOfProfitableTrades();
  void print();
  vector<FuturesTrade> getTrades() const;
};

FuturesInfo parseFileForFuturesInfo(string filePath);
