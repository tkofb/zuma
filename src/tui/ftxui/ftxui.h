#pragma once

#include "../../parser/futures/futuresTrade.h"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/util/ref.hpp>
#include <memory>
#include <string>

enum class Page { Homepage, TradeDetail, Exit };

struct PageState {
    Page nextPage   = Page::Homepage;
    int  tradeIndex = 0; // which trade to open
};

// Run the UI loop with FuturesInfo
void             runUI(FuturesInfo info);
ftxui::Component buildHomepageComponent(FuturesInfo& info, PageState& state,
                                        ftxui::ScreenInteractive& screen);
ftxui::Component buildEnterTradeComponent(const FuturesTrade& trade, PageState& state,
                                          ftxui::ScreenInteractive& screen);
