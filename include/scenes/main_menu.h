#ifndef SCENES_MAIN_MENU_H
#define SCENES_MAIN_MENU_H

#include "base_scene.h"
#include <raylib.h>

class MainMenu : public BaseScene {
  private:
    float timer = 0.0f;
    Texture2D logo;

    Texture2D play_btn;
    Texture2D gear_btn;

    Sound menu_tick;

    const int menu_item_count = 3;

    int selected = 0;

  public:
    Color primary;
    Color bg;

    void Setup(SceneManager *manager) override;
    void Update(SceneManager *manager) override;
    void Draw() override;
    void Teardown() override;
};

#endif // SCENES_MAIN_MENU_H
