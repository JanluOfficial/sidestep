#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <time.h>
#include <raylib.h>

// Custom headers
using namespace std;
#include "map.h"
#include "menu.h"
#include "player.h"
#include "graphics.h"

#define TITLE_Y screenHeight / 2 - 100

#ifndef PLATFORM_WEB
bool on_web = false;
#else
bool on_web = true;
#endif

int key[2] = { KEY_G, KEY_H };
Color color[3] = { BLACK, GREEN, WHITE };

bool debug = false;

#define SCREEN_AUDIO_DISCLAIMER -3
#define SCREEN_JANLU_LOGO -2
#define SCREEN_DEFAULT_KEYBINDS -1
#define SCREEN_MAIN 0
#define SCREEN_OPTIONS 1
#define SCREEN_GRAPHICS 2
#define SCREEN_KEYBINDS 3
#define SCREEN_GAME 4
#define SCREEN_SCORE 5
int screen = SCREEN_JANLU_LOGO;

// Reused across screens
int menuSel = 0;
int menuSelOptCount[] = {2, 4, 5, 0, 0, 0}; // Number of selectable options per screen

void setScreen(int s) {
    screen = s;
    menuSel = 0;
}

int targetFPS = 1;
const char* fpsText;
const char* aspectRatioText;

void PlaySoundResource(Sound sound);
void StopSoundResource(Sound sound);

#ifndef PLATFORM_WEB
void PlaySoundResource(Sound sound) {
    PlaySound(sound);
    return;
}

void StopSoundResource(Sound sound) {
    StopSound(sound);
    return;
}
#endif

Player player;
ProceduralMap proceduralMap;

/*
Ideas
-----------
Player moving up a scrolling level constantly
Player moving left/right to avoid running into walls
Player choosing split paths now and then, one harder but with extra rewards, one easier but no extra rewards
G/H to move left/right
Score increases the longer you survive
*/

int ix = 0, iy = 0;
float timer = 0.0f;

int main() {
    srand(time(0));
    InitWindow(1280, 720, "Sidestep");
    
    #ifndef PLATFORM_WEB
    InitAudioDevice();
    #endif

    #ifdef PLATFORM_WEB
    screen = SCREEN_AUDIO_DISCLAIMER;
    #endif

    // Initialize resolutions array
    int displayWidth = GetMonitorWidth(0), displayHeight = GetMonitorHeight(0);
    #ifdef PLATFORM_WEB
    displayWidth = 1920;
    displayHeight = 1920;
    #endif

    // Generate resolutions based on the moniotor's aspect ratio
    initUserAR(displayWidth, displayHeight);
    int aspectRatioCount = (int)sizeof(aspectRatios) / (int)sizeof(aspectRatios[0]);
    
    //int commonWidths[] = {1280, 1366, 1440, 1600, 1920, 2560, 3440, 3840, 5120, 7680};
    //int commonWidthCount = (int)sizeof(commonWidths) / (int)sizeof(commonWidths[0]);
    
    int resolutions[aspectRatioCount * commonWidthCount][2];
    int resCount[5] = {0, 0, 0, 0, 0};
    for (int ar = 0; ar < aspectRatioCount; ar++) {
        int arX = aspectRatios[ar][0];
        int arY = aspectRatios[ar][1];
        for (int cw = 0; cw < commonWidthCount; cw++) {
            int width = commonWidths[cw];
            int height = (width * arY) / arX;
            if (height <= displayHeight) {
                resolutions[ar * commonWidthCount + resCount[ar]][0] = width;
                resolutions[ar * commonWidthCount + resCount[ar]][1] = height;
                resCount[ar]++;
            }
        }
    }

    int aspectRatioIndex = 2; // Default to 16:9
    int resolutionIndex = 0; // Default to 1280x720
    int currentResolutionIndex = aspectRatioIndex * commonWidthCount + resolutionIndex; // Default to 1280xWhatever

    int screenWidth = resolutions[currentResolutionIndex][0];
    int screenHeight = resolutions[currentResolutionIndex][1];

    int tileSize = screenHeight / 12;
    int gameSpeed = 5; // Higher is faster
    int sideMargin = (screenWidth - 7 * tileSize) / 2;
    SetWindowSize(screenWidth, screenHeight);
    TraceLog(LOG_INFO, TextFormat("Set initial resolution to %dx%d", screenWidth, screenHeight));
    SetTargetFPS(framerates[targetFPS]);

    // Sounds
    Sound failSound = LoadSound("resources/sounds/fail.ogg");
    Sound menuSelectSound = LoadSound("resources/sounds/menu_select.ogg");
    Sound menuConfirmSound = LoadSound("resources/sounds/menu_confirm.ogg");
    Sound menuBackSound = LoadSound("resources/sounds/menu_back.ogg");
    Sound menuTickSound = LoadSound("resources/sounds/menu_tick.ogg");

    Sound milestone1k = LoadSound("resources/sounds/milestone_t.ogg");
    Sound milestone10k = LoadSound("resources/sounds/milestone_t.ogg");
    Sound milestone100k = LoadSound("resources/sounds/milestone_t.ogg");
    Sound milestone1m = LoadSound("resources/sounds/milestone_t.ogg");

    Sound janlu_jingle = LoadSound("resources/sounds/janlu_jingle.ogg");
    Sound janlu_jingle_buildup = LoadSound("resources/sounds/janlu_jingle_buildup.ogg");

    Sound menu_music = LoadSound("resources/sounds/sidestep_menu.ogg");

    // Textures
    Texture2D jpixA = LoadTexture("resources/textures/jpix0.png");
    Texture2D jpixB = LoadTexture("resources/textures/jpix1.png");
    Texture2D jpixC = LoadTexture("resources/textures/jpix2.png");
    Texture2D jpixD = LoadTexture("resources/textures/jpix3.png");
    Texture2D JanluLogo[] = {jpixA, jpixB, jpixC, jpixD};

    bool blink_score = false;
    float blink_tmr = 0.0;
    float tmr = -1;

    int old_score = 0;
    int moved = 0;
    int id = 0;
    bool buildup_played = false, jingle_played = false;
    
    // Menus
    Texture2D logo = LoadTexture("resources/textures/game_logo.png");
    Menu mainMenu("");
    mainMenu.addItem("Start Game", [&]() {
        player = Player();
        proceduralMap = ProceduralMap();
        old_score = 0;
        setScreen(SCREEN_GAME);
        PlaySoundResource(menuConfirmSound);
    });
    mainMenu.addItem("Options", [&]() {
        setScreen(SCREEN_OPTIONS);
        PlaySoundResource(menuConfirmSound);
    });
    #ifndef PLATFORM_WEB
    mainMenu.addItem("Exit", [&]() {
        CloseWindow();
    });
    #endif

    Menu optionsMenu("Options");
    optionsMenu.addItem("Debug Overlay", [&]() {
        debug = !debug;
        PlaySoundResource(menuConfirmSound);
    });
    optionsMenu.addItem("Graphics", [&]() {
        setScreen(SCREEN_GRAPHICS);
        PlaySoundResource(menuConfirmSound);
    });
    optionsMenu.addItem("Keybinds", [&]() {
        setScreen(SCREEN_KEYBINDS);
        PlaySoundResource(menuConfirmSound);
    });
    optionsMenu.addItem("Back", [&]() {
        setScreen(SCREEN_MAIN);
        PlaySoundResource(menuBackSound);
    });

    Menu graphicsMenu("Graphics Options");
    #ifndef PLATFORM_WEB
    graphicsMenu.addItem("Fullscreen", [&]() {
        ToggleFullscreen();
        PlaySoundResource(menuConfirmSound);
    });
    #endif
    graphicsMenu.addItem("Aspect Ratio", [&]() {
        aspectRatioIndex = (aspectRatioIndex + 1) % (sizeof(aspectRatios) / sizeof(aspectRatios[0]));
        // Adjust resolution to match new aspect ratio while keeping width the same
        {
            currentResolutionIndex = aspectRatioIndex * commonWidthCount + resolutionIndex;
            screenWidth = resolutions[currentResolutionIndex][0];
            screenHeight = resolutions[currentResolutionIndex][1];
            SetWindowSize(screenWidth, screenHeight);
            tileSize = screenHeight / 12;
            sideMargin = (screenWidth - 7 * tileSize) / 2;
        }
        PlaySoundResource(menuTickSound);
    });
    graphicsMenu.addItem("Resolution", [&]() {
        // Change Resolution in the current aspect ratio;
        // Update screen size
        resolutionIndex = (resolutionIndex + 1) % resCount[aspectRatioIndex];
        {
            currentResolutionIndex = aspectRatioIndex * commonWidthCount + resolutionIndex;
            screenWidth = resolutions[currentResolutionIndex][0];
            screenHeight = resolutions[currentResolutionIndex][1];
            SetWindowSize(screenWidth, screenHeight);
            tileSize = screenHeight / 12;
            sideMargin = (screenWidth - 7 * tileSize) / 2;
        }

        PlaySoundResource(menuTickSound);
    });
    graphicsMenu.addItem("Framerate", [&]() {
        targetFPS = (targetFPS + 1) % (sizeof(framerates) / sizeof(framerates[0]));
        SetTargetFPS(framerates[targetFPS]);
        PlaySoundResource(menuTickSound);
    });
    graphicsMenu.addItem("Back", [&]() {
        setScreen(SCREEN_OPTIONS);
        PlaySoundResource(menuBackSound);
    });

    int keybind_index = 0;
    int key_pressed = 0;
    while (!WindowShouldClose()) {
        // Logic
        if (IsKeyPressed(key[0])) {
            if (screen != SCREEN_GAME && menuSelOptCount[screen] != 0) {
                PlaySoundResource(menuSelectSound);
            }
        }

        switch (screen) {
            case SCREEN_AUDIO_DISCLAIMER:
                if (tmr > 5 || IsKeyPressed(key[0])) {
                    tmr = 0;
                    setScreen(SCREEN_JANLU_LOGO);
                    break;
                }
                tmr += GetFrameTime();
                break;
            case SCREEN_JANLU_LOGO:
                if (!IsSoundPlaying(janlu_jingle_buildup) && !buildup_played) {
                    PlaySound(janlu_jingle_buildup);
                    buildup_played = true;
                }
                tmr += GetFrameTime();
                id = floor(min(tmr*3, 3.0f));
                if (tmr > 1.5 && !jingle_played) {
                    PlaySound(janlu_jingle);
                    jingle_played = true;
                }
                if (tmr > 15 || (jingle_played && !IsSoundPlaying(janlu_jingle) && tmr > 3)) {
                    setScreen(SCREEN_DEFAULT_KEYBINDS);
                    break;
                }
                break;
            case SCREEN_DEFAULT_KEYBINDS:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                if (IsKeyPressed(key[1])) {
                    setScreen(SCREEN_MAIN);
                    PlaySoundResource(menuConfirmSound);
                    break;
                }
            case SCREEN_MAIN:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                if (IsKeyPressed(key[0])) {
                    mainMenu.incrementSelected();
                }
                if (IsKeyPressed(key[1])) {
                    mainMenu.executeSelected();
                }
                break;
            case SCREEN_OPTIONS:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                if (IsKeyPressed(key[0])) {
                    optionsMenu.incrementSelected();
                }
                if (IsKeyPressed(key[1])) {
                    optionsMenu.executeSelected();
                }
                break;
            case SCREEN_GRAPHICS:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                if (IsKeyPressed(key[0])) {
                    graphicsMenu.incrementSelected();
                }
                if (IsKeyPressed(key[1])) {
                    graphicsMenu.executeSelected();
                }
                break;
            case SCREEN_KEYBINDS:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                // Await any key input to rebind
                key_pressed = GetKeyPressed();
                if (key_pressed != 0) {
                    key[keybind_index] = key_pressed;
                    keybind_index++;
                    PlaySoundResource(menuTickSound);
                    if (keybind_index >= 2) {
                        // Finished keybinding
                        keybind_index = 0;
                        setScreen(SCREEN_OPTIONS);
                        PlaySoundResource(menuConfirmSound);
                    }
                }
                break;
            
            case SCREEN_GAME:
                if (IsSoundPlaying(menu_music)) StopSoundResource(menu_music);
                // Actual Game Logic
                if (IsKeyPressed(key[0])) {
                    // Move player left as far as possible on proceduralMap.map[1]
                    if (player.x > 0 && proceduralMap.map[1][player.x - 1] == 1) {
                        // Calculate how far left we can go
                        for (ix = player.x - 1; ix >= 0; ix--) {
                            if (proceduralMap.map[1][ix] == 1) {
                                player.x = ix;
                                player.score += 10;
                                moved++;
                            } else {
                                break;
                            }
                        }
                    }
                }

                if (IsKeyPressed(key[1])) {
                    // Move player right as far as possible on proceduralMap.map[1]
                    if (player.x < 6 && proceduralMap.map[1][player.x + 1] == 1) {
                        // Calculate how far right we can go
                        for (ix = player.x + 1; ix <= 6; ix++) {
                            if (proceduralMap.map[1][ix] == 1) {
                                player.x = ix;
                                player.score += 10;
                                moved--;
                            } else {
                                break;
                            }
                        }
                    }
                }

                if (proceduralMap.map[1][player.x] == 0) {
                    // Player has gone off path, play fail sound and go back to main menu
                    PlaySoundResource(failSound);
                    setScreen(SCREEN_SCORE);
                }

                if (moved != 0) {
                    PlaySoundResource(menuTickSound);
                    moved = 0;
                }

                if (old_score % 1000 > player.score % 1000) blink_score = true;
                if (old_score % 1000000 > player.score % 1000000) {
                    PlaySoundResource(milestone1m);
                } else if (old_score % 100000 > player.score % 100000) {
                    PlaySoundResource(milestone100k);
                } else if (old_score % 10000 > player.score % 10000) {
                    PlaySoundResource(milestone10k);
                } else if (old_score % 1000 > player.score % 1000) {
                    PlaySoundResource(milestone1k);
                }

                // Advance procedural map based on game speed
                timer += GetFrameTime();
                if (timer >= 1 / (float)gameSpeed) {
                    proceduralMap.generateNextSegment();
                    timer = 0.0f;
                }

                old_score = player.score;

                break;
            case SCREEN_SCORE:
                if (!IsSoundPlaying(menu_music)) PlaySoundResource(menu_music);
                if (IsKeyPressed(key[1])) {
                    PlaySoundResource(menuBackSound);
                    setScreen(SCREEN_MAIN);
                }
                break;
            default:
                setScreen(SCREEN_MAIN);
                break;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(color[0]);
        
        switch (screen) {
            case SCREEN_AUDIO_DISCLAIMER:
                DrawText("Disclaimer", 300, TITLE_Y, 30, color[1]);

                DrawText("Sound due to technical limitations. If you", 300, TITLE_Y + 100, 30, color[2]);
                DrawText("want to play this game with sound, you must", 300, TITLE_Y + 150, 30, color[2]);
                DrawText("download a native build. Sorry for the", 300, TITLE_Y + 200, 30, color[2]);
                DrawText("inconvenience :(", 300, TITLE_Y + 250, 30, color[2]);

                DrawText("The game will continue in 5 seconds", 300, TITLE_Y + 300, 30, color[1]);
                break;
            case SCREEN_JANLU_LOGO:
                DrawTexture(JanluLogo[id], screenWidth / 2 - JanluLogo[id].width / 2, screenHeight / 2 - JanluLogo[3].height / 2, WHITE);
                break;
            case SCREEN_DEFAULT_KEYBINDS:
                DrawText("Default Keybinds:", 250, TITLE_Y, 40, color[2]);
                DrawText("SELECT / MOVE LEFT: G", 250, TITLE_Y + 100, 30, color[1]);
                DrawText("CONFIRM / MOVE RIGHT: H", 250, TITLE_Y + 150, 30, color[1]);
                DrawText("You can change these in the Options menu later.", 250, TITLE_Y + 200, 30, color[2]);

                DrawText("Press CONFIRM to continue", 250, TITLE_Y + 300, 30, color[1]);
                break;
            case SCREEN_MAIN:
                // Logo and Menu
                DrawTexture(logo, 150, TITLE_Y - logo.height, WHITE);
                DrawMenu(mainMenu);
                break;
            case SCREEN_OPTIONS:
                optionsMenu.update_option_text(0, debug ? "ON" : "OFF");
                DrawMenu(optionsMenu);
                break;
            case SCREEN_GRAPHICS:
                if (!on_web) graphicsMenu.update_option_text(0, IsWindowFullscreen() ? "ON" : "OFF");
                aspectRatioText = TextFormat("%d:%d", aspectRatios[aspectRatioIndex][0], aspectRatios[aspectRatioIndex][1]);
                graphicsMenu.update_option_text(on_web ? 0 : 1, aspectRatioText);
                graphicsMenu.update_option_text(on_web ? 1 : 2, TextFormat("%dx%d", screenWidth, screenHeight));
                fpsText = (framerates[targetFPS] == 9999) ? "UNLIMITED" : TextFormat("%d", framerates[targetFPS]);
                graphicsMenu.update_option_text(on_web ? 2 : 3, fpsText);
                
                DrawMenu(graphicsMenu);
                break;
            case SCREEN_KEYBINDS:
                DrawText("Press the key you want to use for:", 250, TITLE_Y, 30, color[2]);
                DrawText(TextFormat("%s", (keybind_index == 0) ? "SELECT/MOVE LEFT" : "CONFIRM/MOVE RIGHT"), 250, TITLE_Y + 100, 40, color[1]);
                break;

            case SCREEN_GAME:
                if (debug) {
                    for (iy = 0; iy < 12; iy++) {
                        for (ix = 0; ix < 7; ix++) {
                            DrawRectangle(ix * 10 + 20, iy * 10 + screenHeight - 140, 10, 10, (proceduralMap.map[iy][ix] == 1) ? RED : DARKGRAY);
                        }
                    }
                    DrawRectangle(player.x * 10 + 20, 2 * 10 + screenHeight - 140, 10, 10, YELLOW);
                }
                
                for (iy = 0; iy < 12; iy++) {
                    for (ix = 0; ix < 7; ix++) {
                        if (proceduralMap.map[11 - iy][ix] == 1) {
                            DrawRectangle(ix * tileSize + (screenWidth - 7 * tileSize) / 2, iy * tileSize, tileSize, tileSize, color[1]);
                        }
                    }
                }
                DrawRectangle(player.x * tileSize + (screenWidth - 7 * tileSize) / 2 + (tileSize / 10), 10 * tileSize + (tileSize / 10), tileSize * 0.8, tileSize * 0.8, color[0]);
                DrawLine(sideMargin - tileSize, 0, sideMargin - tileSize, screenHeight, color[1]);
                DrawLine(screenWidth - sideMargin + tileSize, 0, screenWidth - sideMargin + tileSize, screenHeight, color[1]);

                // Draw score in the top left
                DrawText("Score", 20, 30, 20, color[1]);

                if (blink_score) {
                    if (blink_tmr > 1.0) {
                        blink_score = false;
                        blink_tmr = 0.0;
                    } else {
                        blink_tmr += GetFrameTime();
                    }
                }
                DrawText(TextFormat("%d", player.score), 20, 60, 60, (blink_score && (fmod(blink_tmr, 0.5) >= 0.25)) ? color[0] : color[1]);
                break;
            case SCREEN_SCORE:
                DrawText("Your Score", 20, 30, 30, color[1]);
                DrawText(TextFormat("%d", player.score), 20, 70, 120, color[1]);

                DrawText("Press CONFIRM to go back to the menu", 20, screenHeight - 60, 30, color[1]);
            default:
                break;
        }

        if (debug) {
            DrawText("DEBUG OVERLAY", 10, 10, 20, RED);
            DrawText(TextFormat("FPS: %d (Target: %d)", GetFPS(), framerates[targetFPS]), 10, 40, 20, RED);
            DrawText(TextFormat("Screen: %d", screen), 10, 70, 20, RED);
            //DrawText(TextFormat("Menu Selection: %d", menuSel), 10, 100, 20, RED);

            DrawText(TextFormat("Resolution: %dx%d", screenWidth, screenHeight), 10, 130, 20, RED);
            DrawText(TextFormat("AR Index: %d, RES Index: %d, cRES Index: %d", aspectRatioIndex, resolutionIndex, currentResolutionIndex), 10, 160, 20, RED);
            
            if (screen == SCREEN_GAME) {
                DrawText(TextFormat("GT: %d, LPC: %f, LS: %f", proceduralMap.genType, proceduralMap.likelihoodOfPathChange, proceduralMap.likelihoodOfStructure), 10, 250, 20, RED);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// emcc -o C:\xampp\htdocs\game\index.html main.cpp -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -I. -I C:/raylib/raylib/src -I C:/raylib/raylib/src/external -L. -L C:/raylib/raylib/src -s USE_GLFW=3 -s ASYNCIFY -s INITIAL_MEMORY=67108864 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file resources/ --shell-file C:/raylib/raylib/src/shell.html C:/raylib/raylib/src/web/libraylib.a -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall
