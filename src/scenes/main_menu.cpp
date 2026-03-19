#include "scene_manager.h"
#include "scenes/main_menu.h"
#include <raylib.h>

// Default functions
void MainMenu::Setup(SceneManager *manager) {
  logo = LoadTexture("resources/textures/game_logo.png");

  play_btn = LoadTexture("resources/ui/play.png");
  gear_btn = LoadTexture("resources/ui/gear.png");
  menu_tick = LoadSound("resources/sounds/menu_select.ogg");

  manager->PlayBackgroundMusic("resources/music/sidestep_menu_1.ogg");
  manager->SetMusicVolume(1.0f);
}

void MainMenu::Update(SceneManager *manager) {
  timer += GetFrameTime();

  if (IsKeyPressed(KEY_G)) {
    selected = (selected + 1) % menu_item_count;
    PlaySound(menu_tick);
  }
  if (IsKeyPressed(KEY_H)) {
    switch (selected) {
      case 0:
        // Transition to GameScene
        break;
      case 1:
        // Transition to SettingsScene
        break;
      case 2:
        // Exit
        manager->RequestClose();
        break;
      default:
        break;
    }
  }

  primary = manager->primary_color;
  bg = manager->bg_color;
}

void MainMenu::Draw() {
  int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();
  int centerX = screenWidth / 2, centerY = screenHeight / 2;

  int logoWidth = logo.width, logoHeight = logo.height;
  DrawTexture(logo, screenWidth / 2 - logoWidth / 2, screenHeight / 3 - logoHeight / 2, WHITE);

  for (int i = 0; i < menu_item_count; i++) {
    float x = (float)centerX + 160 * (i - 1) - 75;
    float y = (float)centerY - 50;

    Rectangle border = {x, y, 150, 150};
    if (i == selected) DrawRectangleRec(border, primary);
    else DrawRectangleLinesEx(border, 3, primary);


    DrawTexture((i == 0 ? play_btn : gear_btn), x, y, (i == selected ? bg : primary));
  }
}

void MainMenu::Teardown() {
  UnloadTexture(logo);
  UnloadTexture(play_btn);
  UnloadTexture(gear_btn);

  UnloadSound(menu_tick);
}
