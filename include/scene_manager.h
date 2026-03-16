#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <raylib.h>

class BaseScene;

class SceneManager {
  private:
    std::unique_ptr<BaseScene> currentScene;

  public:
    Color primary_color     = GREEN;
    Color secondary_color   = LIME;
    Color teritary_color    = DARKGREEN;
    Color bg_color          = BLACK;

    // Core
    void ChangeScene(std::unique_ptr<BaseScene> newScene);
    void Update();
    void Draw();

    // Color customization
    void SetColors(Color primary, Color secondary, Color teritary, Color bg);
};

#endif // SCENE_MANAGER_H
