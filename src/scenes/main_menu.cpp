#include "scenes/main_menu.h"
#include <raylib.h>

void MainMenu::Setup() {
  logo = LoadTexture("resource/textures/game_logo.png");
}

void MainMenu::Update(SceneManager *manager) {
  return;
}

void MainMenu::Draw() {
  return;
}

void MainMenu::Teardown() {
  UnloadTexture(logo);
}
