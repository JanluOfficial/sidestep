#include "scene_manager.h"
#include "scenes/logo_scene.h"
#include <raylib.h>
#include <cmath>

void LogoScene::Setup() {
  logos[0] = LoadTexture("resources/textures/jpix0.png");
  logos[1] = LoadTexture("resources/textures/jpix1.png");
  logos[2] = LoadTexture("resources/textures/jpix2.png");
  logos[3] = LoadTexture("resources/textures/jpix3.png");

  buildup = LoadSound("resources/sounds/janlu_jingle_buildup.ogg");
  jingle = LoadSound("resources/sounds/janlu_jingle.ogg");

  PlaySound(buildup);
}

void LogoScene::Update(SceneManager* manager) {
  timer += GetFrameTime();
  textureId = (int)fmin(floor(timer * 3), 3.0f);

  if (timer > 1.5f && !jinglePlayed) {
    PlaySound(jingle);
    jinglePlayed = true;
  }

  if (timer > 3.0f && !IsSoundPlaying(jingle)) {
    //manager->ChangeScene(std::make_unique<MainMenuScene>());
    DrawText("SWITCH TO MAIN WILL TRIGGER HERE", 50, 50, 20, WHITE);
  }
}

void LogoScene::Draw() {
  DrawTexture(
    logos[textureId],
    GetScreenWidth() / 2 - logos[textureId].width / 2,
    GetScreenHeight() / 2 - logos[textureId].height / 2,
    WHITE
  );
}

void LogoScene::Teardown() {
  for (int i = 0; i < 4; i++) UnloadTexture(logos[i]);
  UnloadSound(buildup);
  UnloadSound(jingle);
}
