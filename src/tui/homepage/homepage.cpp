#include "homepage.h"
#include "../ftxui/ftxui.h"

using namespace ftxui;
using namespace std;

string makeNecessaryPaddingAdjustments(const string& str, int width) {
    int len = int(str.length());
    if (width < len) {
        return str;
    }

    int diff = width - len;
    int pad1 = diff / 2;
    int pad2 = diff - pad1;
    return string(pad1, ' ') + str + string(pad2, ' ');
}

vector<vector<string>> handlePaddingForText(vector<vector<string>> data) {
    if (data.empty())
        return data;

    size_t num_cols = data[0].size();
    size_t num_rows = data.size();

    for (size_t col = 0; col < num_cols; col++) {
        int maxElemLen = 0;

        for (size_t row = 0; row < num_rows; row++) {
            maxElemLen = max(maxElemLen, int(data[row][col].length()));
        }

        for (size_t row = 0; row < num_rows; row++) {
            data[row][col] = makeNecessaryPaddingAdjustments(data[row][col], maxElemLen);
        }
    }

    return data;
}

string displayLeastSignificantDigit(string str) {
    auto dot = str.find('.');
    if (dot == string::npos)
        return str;

    str = str.substr(0, dot + 5);

    while (!str.empty() && str.back() == '0')
        str.pop_back();
    if (!str.empty() && str.back() == '.')
        str.pop_back();

    return str;
}

Element buildNavigableTradeTable(const vector<FuturesTrade>& trades, int selected_row) {
    vector<vector<string>> data = {
        {"Symbol", "Date", "Time", "Signal", "Entry Price", "Closing Price", "P&L"}};

    for (size_t i = 0; i < trades.size(); i++) {
        FuturesTrade trade = trades[i];
        string       pnl =
            '$' + displayLeastSignificantDigit(to_string(trade.getRealizedProfitAndLoss()));
        string closingPrice =
            '$' + displayLeastSignificantDigit(to_string(trade.getClosingPrice()));
        string entryPrice = '$' + displayLeastSignificantDigit(to_string(trade.getEntryPrice()));

        vector<string> row = {trade.getSymbol(),
                              trade.getEntryDate(),
                              trade.getEntryTime(),
                              trade.getSignal(),
                              entryPrice,
                              closingPrice,
                              pnl};

        if (row.size() != data[0].size()) {
            cerr << "Row size mismatch! Skipping trade.\n";
            continue;
        }

        data.push_back(row);
    }

    data = handlePaddingForText(data);

    vector<Element> row_elements;
    for (size_t i = 0; i < data.size(); ++i) {
        vector<Element> cells;
        for (const auto& cell : data[i]) {
            Element e        = text(cell);
            Element bordered = e | border;

            if ((int)i == selected_row) {
                bordered = bordered | bgcolor(Color::Blue) | color(Color::White);
            }
            cells.push_back(bordered);
        }
        row_elements.push_back(hbox(move(cells)));
    }

    return vbox(move(row_elements));
}

ftxui::Component buildHomepageComponent(FuturesInfo& info, PageState& state,
                                        ftxui::ScreenInteractive& screen) {
    auto trades       = std::make_shared<std::vector<FuturesTrade>>(info.getTrades());
    auto selected_row = std::make_shared<int>(1);

    // If there are no trades, keep the selector on the header row.
    if (trades->empty())
        *selected_row = 0;

    // Capture by VALUE the shared_ptrs. Capture by REF only state/screen.
    auto renderer = ftxui::Renderer(
        [trades, selected_row] { return buildNavigableTradeTable(*trades, *selected_row); });

    auto main =
        ftxui::CatchEvent(renderer, [trades, selected_row, &state, &screen](ftxui::Event event) {
            if (event == ftxui::Event::Character('j')) {
                // Header is row 0; data rows are [1..trades->size()]
                *selected_row =
                    std::min(*selected_row + 1, trades->empty() ? 0 : (int)trades->size());
                return true;
            }
            if (event == ftxui::Event::Character('k')) {
                *selected_row = std::max(*selected_row - 1, trades->empty() ? 0 : 1);
                return true;
            }
            if (event == ftxui::Event::Return) {
                if (!trades->empty() && *selected_row >= 1) {
                    state.tradeIndex = *selected_row - 1;
                    state.nextPage   = Page::TradeDetail;
                    screen.Exit();
                    return true;
                }
                return false;
            }
            if (event == ftxui::Event::Escape) {
                state.nextPage = Page::Exit;
                screen.Exit();
                return true;
            }
            return false;
        });

    return main;
}

// void printHomepage(FuturesInfo info) {
//     auto screen = ScreenInteractive::Fullscreen();

//     vector<FuturesTrade> trades       = info.getTrades();
//     Element              tradesTable  = buildNavigableTradeTable(trades, 1);
//     int                  selected_row = 1;

//     Component renderer = Renderer([&] { return buildNavigableTradeTable(trades, selected_row);
//     });

//     Component main = CatchEvent(renderer, [&](Event event) {
//         if (event == Event::Character('j')) {
//             selected_row = min(selected_row + 1, (int)trades.size());
//             return true;
//         }
//         if (event == Event::Character('k')) {
//             selected_row = max(selected_row - 1, 1);
//             return true;
//         }
//         if (event == Event::Return) {
//             screen.Exit();
//             showTradeDetail(trades[selected_row - 1]);
//             screen.Loop(main);
//             return true;
//         }
//         return false;
//     });

//     screen.Loop(main);
// }