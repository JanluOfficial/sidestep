#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>

class BaseScene;

class SceneManager {
  private:
    std::unique_ptr<BaseScene> currentScene;

  public:
    void ChangeScene(std::unique_ptr<BaseScene> newScene);
    void Update();
    void Draw();
};

#endif // SCENE_MANAGER_H
