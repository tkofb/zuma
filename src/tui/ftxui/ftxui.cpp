#include "ftxui/ftxui.h"

using namespace ftxui;
using namespace std;

void runUI(FuturesInfo info) {
    auto      screen = ScreenInteractive::Fullscreen();
    PageState state;

    int counter = 0;

    Component button = Button("Increment", [&] { counter++; });

    Component renderer = Renderer(button, [&] {
        return vbox({
                   text("Hello from FTXUI!"),
                   text("Counter = " + std::to_string(counter)),
                   separator(),
                   button->Render(),
                   separator(),
                   text("Press q to quit"),
               }) |
               border;
    });

    while (state.nextPage != Page::Exit) {
        Component pageComponent;

        switch (state.nextPage) {
        case Page::Homepage:
            pageComponent = buildHomepageComponent(info, state, screen);
            break;
        case Page::TradeDetail:

            pageComponent =
                buildEnterTradeComponent(info.getTrades()[state.tradeIndex], state, screen);
            break;
        case Page::Exit:
            break;
        }

        if (state.nextPage != Page::Exit) {
            std::cout << "Starting page loop: " << (int)state.nextPage << std::endl;
            screen.Loop(pageComponent);
            std::cout << "Exited page loop" << std::endl;
        }
    }
}