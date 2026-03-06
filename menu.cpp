#include "menu.h"
#include <cstring>
#include <vector>
#include <raylib.h>
using namespace std;

#define TITLE_POS screenHeight / 2 - 100

Menu::Menu(string title) {
    menu_title = title;
}

void Menu::addItem(string item, function<void()> callback) {
    items.push_back(item);
    callbacks.push_back(callback);
}

void Menu::update_option_text(int index, string text) {
    if (index >= 0 && index < (int)items.size()) {
        option_text.resize(items.size());
        option_text[index] = text;
    }
}

void Menu::incrementSelected() {
    selected++;
    if (!(selected < (int)items.size())) {
        selected = 0;
    }
}

void Menu::executeSelected() {
    if (selected >= 0 && selected < (int)callbacks.size() && callbacks[selected] != nullptr) {
        callbacks[selected]();
    }
}

void DrawMenu(Menu menu) {
    int screenHeight = GetScreenHeight();
    DrawText(menu.menu_title.c_str(), 150, TITLE_POS, 40, GREEN);

    for (int i = 0; i < (int)menu.items.size(); i++) {
        int posX = 150, posY = TITLE_POS + 100 + (50 * i);
        DrawText(menu.items.at(i).c_str(), posX, posY, 30, (menu.selected == i) ? GREEN : WHITE);
        if (i < (int)menu.option_text.size() && !menu.option_text.at(i).empty()) {
            DrawText(menu.option_text.at(i).c_str(), posX + 400, posY, 30, (menu.selected == i) ? GREEN : WHITE);
        }
    }
}
