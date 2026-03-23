#include "scene_manager.h"
#include "scenes/game_scene.h"
#include "scenes/main_menu.h"
#include "player.h"
#include "map.h"
#include <raylib.h>
#include <string>

void penalty(Player *player) {
  player->rempen -= 1;
  player->rempen_cooldown = 1.5;
}

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
    } else if (map.map[1][player.x + 1] == 0 && !(player.rempen_cooldown > 0)) {
      penalty(&player);
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
    } else if (map.map[1][player.x - 1] == 0 && !(player.rempen_cooldown > 0)) {
      penalty(&player);
    }
  }

  if (map.map[1][player.x] == 0 || player.rempen == 0) {
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
  if (player.rempen_cooldown > 0) {
    player.rempen_cooldown -= frame_time;
  }
  if (tick_timer >= 1 / game_speed) {
    map.generateNextSegment();
    tick_timer = 0.0f;
  }
  old_score = player.score;

  primary = manager->primary_color;
  bg = manager->bg_color;
}

void GameScene::Draw() {
  int sw = GetScreenWidth(), sh = GetScreenHeight();
  int cx = sw / 2, cy = sh / 2;

  int rectSize = sh / 12;

  // Game Area
  for (int x = 0; x <= 6; x++) {
    for (int y = 0; y < 12; y++) {
      int dy = 11 - y;
      if (map.map[y][x] == 0) continue;
      DrawRectangle((x - 3) * rectSize + cx - rectSize/2, dy * rectSize, rectSize, rectSize, primary);
    }
  }
  DrawRectangle((player.x-3) * rectSize + rectSize/6 + cx - rectSize/2, sh - 2*rectSize+rectSize/6, rectSize-rectSize/3, rectSize-rectSize/3, bg);

  // UI
  DrawText("Score", 20, 20, 20, primary);
  DrawText(TextFormat("%d", player.score), 20, 40, 40, primary);

  for (int i = 0; i < player.rempen; i++) {
    DrawRectangle(20 + 60 * i, 100, 50, 20, primary);
  }
}

void GameScene::Teardown() {
  UnloadSound(milestone1k);
  UnloadSound(milestone10k);
  UnloadSound(milestone100k);
  UnloadSound(milestone1m);
}
