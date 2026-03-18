#include "scene_manager.h"
#include "scenes/main_menu.h"
#include <raylib.h>

// Default functions
void MainMenu::Setup() {
  logo = LoadTexture("resources/textures/game_logo.png");
}

void MainMenu::Update(SceneManager *manager) {
  timer += GetFrameTime();

  if (IsKeyPressed(KEY_G)) selected = (selected + 1) % menu_item_count;

  if (IsKeyPressed(KEY_A)) manager->SetColors(PINK, PURPLE, BLUE, WHITE); 

  primary = manager->primary_color;
}

void MainMenu::Draw() {
  int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();
  int centerX = screenWidth / 2, centerY = screenHeight / 2;
  
  int logoWidth = logo.width, logoHeight = logo.height;
  DrawTexture(logo, screenWidth / 2 - logoWidth / 2, screenHeight / 3 - logoHeight / 2, WHITE);

  for (int i = 0; i < menu_item_count; i++) {
    DrawRectangleLines(centerX - 150, centerY - 25 + i * 50, 300, 50, primary);
  }
}

void MainMenu::Teardown() {
  UnloadTexture(logo);
}
