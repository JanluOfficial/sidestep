#include "input.h"
#include "scene_manager.h"
#include "scenes/settings_scene.h"
#include "scenes/main_menu.h"
#include <raylib.h>
#include <cstring>
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

  primary = manager->primary_color;
  bg = manager->bg_color;
}

void SettingsScene::Draw() {
    ClearBackground(bg);
    int startPosX = 100;
    int startPosY = 100;
    int fontSize = 20;
    int spacing = 40;

    for (int i = 0; i < (int)current_menu->size(); i++) {
        const char* label = (*current_menu)[i].label.c_str();

        if (i == selected) {
            DrawText(">", startPosX - 20, startPosY + (i * spacing), fontSize, primary);
        }

        DrawText(label, startPosX, startPosY + (i * spacing), fontSize, primary);
    }
}

void SettingsScene::Teardown() {

}
