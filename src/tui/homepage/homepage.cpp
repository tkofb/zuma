#include "homepage.h"
#include "../ftxui.h"

using namespace ftxui;
using namespace std;

Element buildNavigableTradeTable(const vector<FuturesTrade> &trades,
                                 int selected_row) {
  vector<vector<string>> data = {
      {"Symbol", "Entry", "Quantity", "Entry Price", "Closing Price", "P&L"}};

  for (const FuturesTrade &trade : trades) {
    data.push_back({trade.getSymbol(), trade.getEntryDate(),
                    to_string(trade.getQuantity()),
                    to_string(trade.getEntryPrice()),
                    to_string(trade.getClosingPrice()),
                    to_string(trade.getRealizedProfitAndLoss())});
  }

  vector<Element> row_elements;
  for (size_t i = 0; i < data.size(); ++i) {
    vector<Element> cells;
    for (const auto &cell : data[i]) {
      Element e = text(cell);
      Element bordered = e | border;

      if ((int)i == selected_row) {
        bordered =
            bordered | bgcolor(Color::Blue) | color(Color::White); // Highlight
      }
      cells.push_back(bordered);
    }
    row_elements.push_back(hbox(move(cells)));
  }

  return vbox(move(row_elements));
}

void printHomepage(FuturesInfo info) {
  auto screen = ScreenInteractive::Fullscreen();

  vector<FuturesTrade> trades = info.getTrades();
  Element tradesTable = buildNavigableTradeTable(trades, 1);
  int selected_row = 1;

  Component renderer =
      Renderer([&] { return buildNavigableTradeTable(trades, selected_row); });

  Component main = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('j')) {
      selected_row = min(selected_row + 1, (int)trades.size());
      return true;
    }
    if (event == Event::Character('k')) {
      selected_row = max(selected_row - 1, 1);
      return true;
    }
    if (event == Event::Return) {
      screen.Exit();
      showTradeDetail(trades[selected_row - 1]);
      screen.Loop(main);
      return true;
    }
    return false;
  });

  screen.Loop(main);
}