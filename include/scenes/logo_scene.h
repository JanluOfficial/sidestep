#ifndef SCENES_LOGO_SCENE_H
#define SCENES_LOGO_SCENE_H

#include "base_scene.h"
#include <raylib.h>

class LogoScene : public BaseScene {
  private:
    float timer = 0.0f;
    int textureId = 0;
    Texture2D logos[4];
    Sound buildup;
    Sound jingle;
    bool jinglePlayed = false;

  public:
    void Setup() override;
    void Update(SceneManager *manager) override;
    void Draw() override;
    void Teardown() override;
};

#endif // SCENES_LOGO_SCENE_H
