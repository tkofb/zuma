#pragma once // Always good to prevent multiple inclusion
#include "../../src/parser/futures/futuresTrade.h"
#include "../ftxui/ftxui.h"
#include <ftxui/component/component.hpp> // Required for Component


ftxui::Component buildEnterTradeComponent(const FuturesTrade& trade, PageState& state);