#include "scene_manager.h"
#include "scenes/game_scene.h"
#include "scenes/main_menu.h"
#include "player.h"
#include "map.h"
#include <raylib.h>
#include <string>

void GameScene::Setup(SceneManager *manager) {
  milestone1k = LoadSound("resources/sounds/milestone_t.ogg");
  milestone10k = LoadSound("resources/sounds/milestone_tt.ogg");
  milestone100k = LoadSound("resources/sounds/milestone_ht.ogg");
  milestone1m = LoadSound("resources/sounds/milestone_m.ogg");
}

void GameScene::Update(SceneManager *manager) {
  if (IsKeyPressed(KEY_G)) {
    if (player.x > 0 && map.map[1][player.x - 1] == 1) {
      for (int ix = player.x - 1; ix >= 0; ix--) {
        if (map.map[1][ix] == 0) break;
        player.x = ix;
        player.score += 10;
        moved++;
      }
    }
  }

  if (IsKeyPressed(KEY_H)) {
    if (player.x < 6 && map.map[1][player.x + 1] == 1) {
      for (int ix = player.x + 1; ix <= 6; ix++) {
        if (map.map[1][ix] == 0) break;
        player.x = ix;
        player.score += 10;
        moved--;
      }
    }
  }

  if (map.map[1][player.x] == 0) {
    manager->ChangeScene(std::make_unique<MainMenu>());
    return;
  }

  if (old_score % 1000000 > player.score % 1000000) {
    PlaySound(milestone1m);
  } else if (old_score % 100000 > player.score % 100000) {
    PlaySound(milestone100k);
  } else if (old_score % 10000 > player.score % 10000) {
    PlaySound(milestone10k);
  } else if (old_score % 1000 > player.score % 1000) {
    PlaySound(milestone1k);
  }

  float frame_time = GetFrameTime();
  timer += frame_time;
  tick_timer += GetFrameTime();
  if (tick_timer >= 1 / game_speed) {
    map.generateNextSegment();
    tick_timer = 0.0f;
  }
  old_score = player.score;

  primary = manager->primary_color;
  bg = manager->bg_color;
}

void GameScene::Draw() {
  DrawText("Implement game scene draw soon", 50, 50, 20, primary);
  for (int x = 0; x <= 6; x++) {
    for (int y = 0; y < 12; y++) {
      if (map.map[y][x] == 0) continue;
      DrawRectangle(x * 30, y * 30, 30, 30, primary);
    }
  }
  DrawRectangle(player.x * 30 + 5, 35, 20, 20, RED);
}

void GameScene::Teardown() {
  UnloadSound(milestone1k);
  UnloadSound(milestone10k);
  UnloadSound(milestone100k);
  UnloadSound(milestone1m);
}
