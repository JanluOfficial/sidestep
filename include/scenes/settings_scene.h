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

    bool has_sub_menu() const { return !sub_options.empty(); }
};

class SettingsScene : public BaseScene {
  private:
    float timer = 0.0f;

    int sub_menu = 0;
    int selected = 0;

  public:
    Color primary;
    Color bg;

    void Setup(SceneManager *manager) override;
    void Update(SceneManager *manager) override;
    void Draw() override;
    void Teardown() override;
};

#endif // SCENES_SETTINGS_H
