#ifndef SCENES_SETTINGS_H
#define SCENES_SETTINGS_H

#include "base_scene.h"
#include <string>
#include <vector>
#include <raylib.h>
#include <functional>

struct MenuOption {
  std::string label;
  std::string secondary_label;

  std::function<void()> action = nullptr;  // Action to perform when selected if not sub menu
  std::vector<MenuOption> sub_options; // If vector not empty, open sub menu when selected

  MenuOption(std::string l, std::function<void()> a = nullptr, std::vector<MenuOption> sub = {})
    : label(l), action(a), sub_options(sub) {}

  bool has_sub_menu() const { return !sub_options.empty(); };
  int sub_menu_size() const { return sub_options.size(); };
};

class SettingsScene : public BaseScene {
  private:
    float timer = 0.0f;

    int selected = 0;

    std::vector<MenuOption> menu_root;
    std::vector<MenuOption>* current_menu = nullptr;
    std::vector<std::vector<MenuOption>*> menu_stack;

    int current_menu_size() const { return current_menu->size(); };

  public:
    Color primary;
    Color bg;

    void NavigateForward();
    void NavigateBack(SceneManager *man);

    void Setup(SceneManager *manager) override;
    void Update(SceneManager *manager) override;
    void Draw() override;
    void Teardown() override;
};

#endif // SCENES_SETTINGS_H
