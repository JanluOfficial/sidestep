#include "scene_manager.h"
#include "base_scene.h"
#include <memory>
#include <raylib.h>
using namespace std;

// Core
void SceneManager::ChangeScene(unique_ptr<BaseScene> newScene) {
  if (currentScene) currentScene->Teardown();
  currentScene = std::move(newScene);
  currentScene->Setup();
};

void SceneManager::Update() {
  if (currentScene) currentScene->Update(this);
};

void SceneManager::Draw() {
  if (currentScene) currentScene->Draw();
};

// Color customization
void SceneManager::SetColors(Color primary, Color secondary, Color teritary, Color bg) {
  primary_color     = primary;
  secondary_color   = secondary;
  teritary_color    = teritary;
  bg_color          = bg;
}
