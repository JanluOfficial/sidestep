#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "base_scene.h"
#include "player.h"
#include "map.h"
#include <raylib.h>

class GameScene : public BaseScene {
private:
  float timer = 0.0f;
  float tick_timer = 0.0f;

  Sound milestone1k;
  Sound milestone10k;
  Sound milestone100k;
  Sound milestone1m;

  ProceduralMap map;
  Player player;

  int moved;
  int old_score;

  const float game_speed = 5;

public:
  Color primary;
  Color bg;

  void Setup(SceneManager *manager) override;
  void Update(SceneManager *manager) override;
  void Draw() override;
  void Teardown() override;
};

#endif // GAME_SCENE_H
