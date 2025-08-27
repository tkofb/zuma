#pragma once
#include "../../src/parser/futures/futuresTrade.h"
#include "../../utils/databaseFunctions/databaseFunctions.h"
#include "../ftxui/ftxui.h"
#include <ftxui/component/component.hpp>

ftxui::Component buildEnterTradeComponent(const FuturesTrade& trade, PageState& state,
                                          ftxui::ScreenInteractive& screen);
