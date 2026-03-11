#include "scene_manager.h"
#include "base_scene.h"
#include <memory>
using namespace std;

void SceneManager::ChangeScene(unique_ptr<BaseScene> newScene) {
  if (currentScene) currentScene->Teardown();
  currentScene = std::move(newScene);
  currentScene->Setup();
};

void SceneManager::Update() {
  if (currentScene) currentScene->Update();
};

void SceneManager::Draw() {
  if (currentScene) currentScene->Draw();
};
