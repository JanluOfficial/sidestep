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

// Music System Implementation
void SceneManager::PlayBackgroundMusic(const std::string& fileName) {
  if (is_music_playing) {
    StopMusic();
  }

  currentMusic = LoadMusicStream(fileName.c_str());
  PlayMusicStream(currentMusic);
  is_music_playing = true;
}

void SceneManager::StopMusic() {
  if (is_music_playing) {
    StopMusicStream(currentMusic);
    UnloadMusicStream(currentMusic);
    is_music_playing = false;
  }
}

void SceneManager::SetMusicVolume(float volume) {
  if (is_music_playing) {
    SetMusicVolume(currentMusic, volume);
  }
}

// Color customization
void SceneManager::SetColors(Color primary, Color secondary, Color teritary, Color bg) {
  primary_color     = primary;
  secondary_color   = secondary;
  teritary_color    = teritary;
  bg_color          = bg;
}

// Cleanup (unloads the assets because we
// can't have memory leaks in the big 26)
SceneManager::~SceneManager() {
    if (is_music_playing) {
        UnloadMusicStream(currentMusic);
    }
}
