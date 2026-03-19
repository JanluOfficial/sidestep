#include "raylib.h"
#include "scene_manager.h"
#include "scenes/logo_scene.h"
#include <memory>

bool debug = true;

int main() {
  InitWindow(1280, 720, "Sidestep");
  SetAudioStreamBufferSizeDefault(4096);
  InitAudioDevice();
  SetTargetFPS(120);

  SceneManager sceneman;
  sceneman.ChangeScene(std::make_unique<LogoScene>());

  while (!WindowShouldClose() && !sceneman.RequestingClose()) {
    sceneman.Update();

    BeginDrawing();
      ClearBackground(sceneman.bg_color);
      sceneman.Draw();

      if (debug) {
        DrawText("DEBUG OVERLAY", 10, 10, 20, RED);
      }
    EndDrawing();
  }

  sceneman.Teardown();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
