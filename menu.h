#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include <string>
#include <vector>
#include <functional>
using namespace std;

class Menu {
  public:
    string title = "";
    vector<string> items = {};
    vector<string> option_text = {};
    vector<function<void()>> callbacks = {};
    vector<Sound> sounds = {};
    Menu(string title);
    int selected = 0;

    void addItem(string item, function<void()> callback = nullptr, Sound sound = LoadSound("resources/sounds/menu_select.ogg"));
    void update_option_text(int index, string text);
    void incrementSelected();
    void executeSelected();
};

void DrawMenu(Menu menu);

#endif // MENU_H
