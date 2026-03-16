#ifndef SCENES_MAIN_MENU_H
#define SCENES_MAIN_MENU_H

#include "base_scene.h"
#include <raylib.h>

class MainMenu : public BaseScene {
  private:
    float timer = 0.0f;
    Texture2D logo;

  public:
    void Setup() override;
    void Update(SceneManager *manager) override;
    void Draw() override;
    void Teardown() override;
};

#endif // SCENES_MAIN_MENU_H
