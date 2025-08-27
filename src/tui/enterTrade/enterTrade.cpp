#include "enterTrade.h"
#include "../ftxui/ftxui.h"
#include <atomic>
#include <memory>
#include <thread>

using namespace ftxui;

JournalEntry::JournalEntry(shared_ptr<string> title, shared_ptr<string> notes,
                           shared_ptr<string> lesson) {
    this->title  = std::move(title);
    this->notes  = std::move(notes);
    this->lesson = std::move(lesson);
}

void exitLoop(const JournalEntry& journal, const FuturesTrade& trade, ScreenInteractive& screen) {
    updateJournalEntry(trade, journal);
    screen.Exit();
}

ftxui::Component buildEnterTradeComponent(const FuturesTrade& trade, PageState& state,
                                          ftxui::ScreenInteractive& screen) {
    auto title          = std::make_shared<std::string>();
    auto notes          = std::make_shared<std::string>();
    auto lesson         = std::make_shared<std::string>();
    auto focus_index    = std::make_shared<int>(0);
    auto cursor_visible = std::make_shared<std::atomic<bool>>(true);
    auto running        = std::make_shared<std::atomic<bool>>(true);

    // Blink thread: toggle + post event to trigger redraws.
    auto blink_thread = std::make_shared<std::thread>([cursor_visible, running, &screen] {
        using namespace std::chrono_literals;
        while (running->load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(350ms);
            bool cur = cursor_visible->load(std::memory_order_relaxed);
            cursor_visible->store(!cur, std::memory_order_relaxed);
            screen.PostEvent(ftxui::Event::Custom); // or screen.Post(...) on older FTXUI
        }
    });

    auto make_field = [focus_index, cursor_visible](std::string label, std::string value,
                                                    int index) {
        if (index == *focus_index && cursor_visible->load(std::memory_order_relaxed))
            value.push_back('_');
        auto field = hbox({text(label + ": "), text(value)});
        if (index == *focus_index)
            return field | inverted;
        return field;
    };

    auto renderer = ftxui::Renderer([=] {
        return vbox({
                   make_field("Title", *title, 0),
                   make_field("Notes", *notes, 1),
                   make_field("Lesson", *lesson, 2),
                   separator(),
                   text("TAB: switch  |  ESC: back"),
               }) |
               border;
    });

    auto main = ftxui::CatchEvent(renderer, [=, &state, &screen](ftxui::Event event) {
        if (event == ftxui::Event::Custom)
            return true; // <- force redraw for blinking

        if (event == ftxui::Event::Tab) {
            *focus_index = (*focus_index + 1) % 3;
            return true;
        }
        if (event == ftxui::Event::Escape) {
            running->store(false, std::memory_order_relaxed);
            if (blink_thread->joinable())
                blink_thread->join();
            state.nextPage = Page::Homepage;
            JournalEntry journal(title, notes, lesson);
            exitLoop(journal, trade, screen);
            return true;
        }
        if (event.is_character()) {
            char c = event.character()[0];
            if (std::isalnum((unsigned char)c) || std::isspace((unsigned char)c)) {
                (*focus_index == 0 ? *title : *focus_index == 1 ? *notes : *lesson).push_back(c);
                return true;
            }
        }
        if (event == ftxui::Event::Backspace) {
            auto erase_last = [](std::string& s) {
                if (!s.empty())
                    s.pop_back();
            };
            if (*focus_index == 0)
                erase_last(*title);
            else if (*focus_index == 1)
                erase_last(*notes);
            else
                erase_last(*lesson);
            return true;
        }
        return false;
    });

    return main;
}
