#include "enterTrade.h"
#include "../ftxui.h"

using namespace ftxui;
using namespace std;

void showTradeDetail(const FuturesTrade &trade) {
  auto screen = ScreenInteractive::Fullscreen();

  Component detail = Renderer([&] {
    return vbox({
        text("Trade Details") | bold | color(Color::Green),
        separator(),
        text("Symbol: " + trade.getSymbol()),
        text("Entry Date: " + trade.getEntryDate()),
        text("Quantity: " + to_string(trade.getQuantity())),
        text("Entry Price: " + to_string(trade.getEntryPrice())),
        text("Closing Price: " + to_string(trade.getClosingPrice())),
        text("P&L: " + to_string(trade.getRealizedProfitAndLoss())),
        separator(),
        text("Press q to return"),
    });
  });

  Component main = CatchEvent(detail, [&](Event event) {
    if (event == Event::Character('q')) {
      screen.Exit(); // Return to homepage
      return true;
    }
    return false;
  });

  screen.Loop(main);
}