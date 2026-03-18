#include "scene_manager.h"
#include "scenes/main_menu.h"
#include <raylib.h>

// Default functions
void MainMenu::Setup(SceneManager *manager) {
  logo = LoadTexture("resources/textures/game_logo.png");
  manager->PlayBackgroundMusic("resources/music/sidestep_menu_1.ogg");
  manager->SetMusicVolume(1.0f);
}

void MainMenu::Update(SceneManager *manager) {
  timer += GetFrameTime();

  if (IsKeyPressed(KEY_G)) selected = (selected + 1) % menu_item_count;

  primary = manager->primary_color;
}

void MainMenu::Draw() {
  int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();
  int centerX = screenWidth / 2, centerY = screenHeight / 2;

  int logoWidth = logo.width, logoHeight = logo.height;
  DrawTexture(logo, screenWidth / 2 - logoWidth / 2, screenHeight / 3 - logoHeight / 2, WHITE);

  for (int i = 0; i < menu_item_count; i++) {
    Rectangle border = {
      (float)centerX + 160 * (i - 1) - 75,
      (float)centerY - 50,
      150, 150
    };
    DrawRectangleLinesEx(border, 3, primary);
  }
}

void MainMenu::Teardown() {
  UnloadTexture(logo);
}
