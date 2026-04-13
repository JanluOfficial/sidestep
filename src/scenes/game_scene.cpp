#include "scene_manager.h"
#include "scenes/game_scene.h"
#include "scenes/main_menu.h"
#include "player.h"
#include "map.h"
#include "input.h"
#include <raylib.h>
#include <string>

#define L_BUFFERABLE map.map[2] != 0 && tick_timer > 1 / game_speed * 0.7

bool penalty(Player *player) {
  if (player->rempen_cooldown > 0) return false;
  player->rempen -= 1;
  player->rempen_cooldown = 1.5;
  return true;
}

void GameScene::Setup(SceneManager *manager) {
  milestone1k = LoadSound("resources/sounds/milestone_t.ogg");
  milestone10k = LoadSound("resources/sounds/milestone_tt.ogg");
  milestone100k = LoadSound("resources/sounds/milestone_ht.ogg");
  milestone1m = LoadSound("resources/sounds/milestone_m.ogg");

  move = LoadSound("resources/sounds/menu_tick.ogg");
  dmg = LoadSound("resources/sounds/fail.ogg");
}

void GameScene::Update(SceneManager *manager) { 
  float frame_time = GetFrameTime();
  
  if (!paused && !dead){
    if (IsInputLeftPressed() || l_buffered && tick_timer < 0.1) {
      if (player.x > 0 && map.map[1][player.x - 1] != 0) {
        for (int ix = player.x - 1; ix >= 0; ix--) {
          if (map.map[1][ix] == 0) break;
          player.x = ix;
          player.score += 10;
          moved++;
        }
      } else {
        if (penalty(&player)) PlaySound(dmg);
      }
    }

    if (IsInputRightPressed() || r_buffered && tick_timer < 0.1) {
      if (player.x < 6 && map.map[1][player.x + 1] != 0) {
        for (int ix = player.x + 1; ix <= 6; ix++) {
          if (map.map[1][ix] == 0) break;
          player.x = ix;
          player.score += 10;
          moved--;
        }
      } else {
        if (penalty(&player)) PlaySound(dmg);
      }
    }

    if (map.map[1][player.x] == 0 || player.rempen == 0) {
      //manager->ChangeScene(std::make_unique<MainMenu>());
      dead = true;
      death_timer = 0;
      return;
    }

    if (tick_timer >= 1 / game_speed) {
      map.generateNextSegment();
      player.score += 5;
      tick_timer = 0.0f;
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

    timer += frame_time;
    tick_timer += frame_time;
    if (player.rempen_cooldown > 0) {
      player.rempen_cooldown -= frame_time;
    }
    old_score = player.score;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    paused = !paused;
  }

  if (dead) {
    if (manager->IsMusicPlaying()) {
      manager->StopMusic();
    }
    death_timer += frame_time;
    if (death_timer > 3.0f) {
      manager->ChangeScene(std::make_unique<MainMenu>());
      return;
    }
  }

  primary = manager->primary_color;
  bg = manager->bg_color;
}

void GameScene::Draw() {
  Color tint_bg = ColorAlpha(bg, 0.5);
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
  DrawRectangle(
    (player.x-3) * rectSize + rectSize/6 + cx - rectSize/2,
    sh - 2*rectSize+rectSize/6,
    rectSize-rectSize/3,
    rectSize-rectSize/3, bg
  );

  // UI
  DrawText("Score", 20, 20, 20, primary);
  DrawText(TextFormat("%d", player.score), 20, 40, 40, primary);

  for (int i = 0; i < player.rempen; i++) {
    DrawRectangle(20 + 60 * i, 100, 50, 20, primary);
  }

  // Paused
  if (paused) {
    DrawRectangle(0, 0, sw, sh, tint_bg);
    DrawText("PAUSED", cx - 100, cy - 20, 40, primary);
  }

  // Dead
  if (dead) {
    DrawRectangle(0, 0, sw, sh, tint_bg);
    DrawText("Game Over", cx - 100, cy - 20, 40, primary);
  }
}

void GameScene::Teardown() {
  UnloadSound(milestone1k);
  UnloadSound(milestone10k);
  UnloadSound(milestone100k);
  UnloadSound(milestone1m);

  UnloadSound(move);
  UnloadSound(dmg);
}
