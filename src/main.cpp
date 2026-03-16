#include "raylib.h"
#include "scene_manager.h"
#include "scenes/logo_scene.h"
#include <memory>

bool debug = true;

int main() {
  InitWindow(1280, 720, "Sidestep");
  InitAudioDevice();
  SetTargetFPS(60);

  SceneManager sceneman;
  sceneman.ChangeScene(std::make_unique<LogoScene>());

  while (!WindowShouldClose()) {
    sceneman.Update();

    BeginDrawing();
      ClearBackground(BLACK);
      sceneman.Draw();

      if (debug) {
        DrawText("DEBUG OVERLAY", 10, 10, 20, RED);
      }
    EndDrawing();
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
