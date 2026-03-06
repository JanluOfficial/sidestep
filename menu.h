#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <functional>
using namespace std;

class Menu {
  public:
    string menu_title = "";
    vector<string> items = {};
    vector<string> option_text = {};
    vector<function<void()>> callbacks = {};
    Menu(string title);
    int selected = 0;

    void addItem(string item, function<void()> callback = nullptr);
    void update_option_text(int index, string text);    
    void incrementSelected();
    void executeSelected();
};

void DrawMenu(Menu menu);

#endif // MENU_H
