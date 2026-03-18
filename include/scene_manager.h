#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <raylib.h>
#include <string>

class BaseScene;

// enum class AudioState { IDLE, PLAYING, FADING_OUT };

class SceneManager {
  private:
    std::unique_ptr<BaseScene> currentScene;
    Music current_music;
    bool is_music_playing = false;

  public:
    Color primary_color     = GREEN;
    Color secondary_color   = LIME;
    Color teritary_color    = DARKGREEN;
    Color bg_color          = BLACK;

    // Core
    void ChangeScene(std::unique_ptr<BaseScene> newScene);
    void Update();
    void Draw();

    // Music System
    void PlayBackgroundMusic(const std::string& fileName);
    void StopMusic();
    void SetMusicVolume(float volume);

    // Color customization
    void SetColors(Color primary, Color secondary, Color teritary, Color bg);

    // Cleanup
    ~SceneManager();
};

#endif // SCENE_MANAGER_H
