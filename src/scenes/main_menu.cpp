#include "scenes/main_menu.h"
#include <raylib.h>

void MainMenu::Setup() {
  logo = LoadTexture("resources/textures/game_logo.png");
}

void MainMenu::Update(SceneManager *manager) {
  return;
}

void MainMenu::Draw() {
  DrawTexture(logo, 50, 50, WHITE);
}

void MainMenu::Teardown() {
  UnloadTexture(logo);
}
