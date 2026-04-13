#include "raylib.h"
#include "scene_manager.h"
#include "scenes/logo_scene.h"
#include <memory>

int main() {
  InitWindow(1280, 720, "Sidestep");
  SetAudioStreamBufferSizeDefault(4096);
  InitAudioDevice();
  SetTargetFPS(120);

  SceneManager sceneman;
  sceneman.ChangeScene(std::make_unique<LogoScene>());

  while (!WindowShouldClose() && !sceneman.RequestingClose()) {
    if (IsKeyPressed(KEY_F3)) sceneman.SetDebug(!sceneman.DebugEnabled());
    sceneman.Update();

    BeginDrawing();
      ClearBackground(sceneman.bg_color);
      sceneman.Draw();

      if (sceneman.DebugEnabled()) {
        DrawText("DEBUG OVERLAY", 10, 10, 20, RED);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 30, 20, RED);
      }
    EndDrawing();
  }

  sceneman.Teardown();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
