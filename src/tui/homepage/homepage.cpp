#include "homepage.h"
#include <ftxui/screen/color.hpp>
#include "ftxui/screen/screen.hpp"
#include <ftxui/component/component.hpp>             // for Renderer, Button, Input
#include <ftxui/component/screen_interactive.hpp>    // for ScreenInteractive
#include <ftxui/dom/elements.hpp>                    // for text, vbox, etc.
#include <ftxui/component/event.hpp>

using namespace ftxui;
 
void printHomepage() {
    auto screen = ScreenInteractive::TerminalOutput();
    auto renderer = Renderer([] { return text("Hello world"); });
    renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character('q')) {
        screen.ExitLoopClosure()();
        return true;
    }
    return false;
    });
    screen.Loop(renderer);
}