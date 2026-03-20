#include "scene_manager.h"
#include "base_scene.h"
#include <memory>
#include <string>
#include <raylib.h>
using namespace std;

// Core
void SceneManager::ChangeScene(unique_ptr<BaseScene> newScene) {
  nextScene = std::move(newScene);
};

void SceneManager::Update() {
  if (is_music_playing) UpdateMusicStream(current_music);

  if (nextScene) {
    if (currentScene) currentScene->Teardown();
    currentScene = std::move(nextScene);
    currentScene->Setup(this);
  }

  if (currentScene) currentScene->Update(this);
};

void SceneManager::Draw() {
  if (currentScene) currentScene->Draw();
};

void SceneManager::Teardown() {
  if (currentScene) currentScene->Teardown();
}

// Music System Implementation
void SceneManager::PlayBackgroundMusic(const std::string& fileName) {
  if (is_music_playing) {
    StopMusic();
  }

  current_music = LoadMusicStream(fileName.c_str());
  PlayMusicStream(current_music);
  is_music_playing = true;
}

bool SceneManager::IsMusicPlaying() {
  return is_music_playing;
}

void SceneManager::StopMusic() {
  if (is_music_playing) {
    StopMusicStream(current_music);
    UnloadMusicStream(current_music);
    is_music_playing = false;
  }
}

void SceneManager::SetMusicVolume(float volume) {
  if (is_music_playing) {
    ::SetMusicVolume(current_music, volume);
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
    UnloadMusicStream(current_music);
  }
}

// Close stuff
bool SceneManager::RequestingClose() { return requesting_close; }
void SceneManager::RequestClose() { requesting_close = true; }
