#include "input.h"
#include "scene_manager.h"
#include "scenes/settings_scene.h"
#include "scenes/main_menu.h"
#include <raylib.h>
#include <memory>

void SettingsScene::NavigateForward() {
  auto& item = (*current_menu)[selected];

  if (item.has_sub_menu()) {
    menu_stack.push_back(current_menu);
    current_menu = &item.sub_options;
    selected = 0;
  } else if (item.action) {
    item.action();
  }
}

void SettingsScene::NavigateBack(SceneManager *man) {
  if (!menu_stack.empty()) {
    current_menu = menu_stack.back();
    menu_stack.pop_back();
    selected = 0;
  } else {
    man->ChangeScene(std::make_unique<MainMenu>());
  }
}

void SettingsScene::Setup(SceneManager *manager) {
  menu_root = {
    {"Audio", nullptr, {
      {"Music Volume", []() { /* Adjust Vol */ }},
      {"SFX Volume",   []() { /* Adjust Vol */ }}
    }},
    {"Graphics", nullptr, {
      {"Fullscreen",   []() { /* Toggle */ }}
    }},
    {"Back to Main", [manager]() { manager->ChangeScene(std::make_unique<MainMenu>()); }}
  };

  current_menu = &menu_root;
}

void SettingsScene::Update(SceneManager *manager) {
  timer += GetFrameTime();

  if (IsInputLeftPressed()) {
    selected = (selected + 1) % current_menu_size();
  }

  if (IsInputRightPressed()) {
    NavigateForward();
  }
}

void SettingsScene::Draw() {
  DrawRectangle(10 * selected, 10, 10, 10, RED);
}

void SettingsScene::Teardown() {

}
