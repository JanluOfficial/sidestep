#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <raylib.h>
#include <algorithm>
using namespace std;

int framerates[] = {30, 60, 120, 240, 9999};

int commonWidths[] = {1280, 1366, 1440, 1600, 1920, 2560, 3440, 3840, 5120, 7680};
int commonWidthCount = (int)sizeof(commonWidths) / (int)sizeof(commonWidths[0]);

int aspectRatios[][2] = {
    {0, 0}, // Placeholder for user screen aspect ratio
    {4, 3},
    {16, 10},
    {16, 9},
    {21, 9},
};

int gcd(int a, int b) {
    int res = min(a, b);

    while (res > 1) {
        if (a % res == 0 && b % res == 0)
            break;
        res--;
    }
	return res;
}

void initUserAR(int screenWidth, int screenHeight) {
    int g = gcd(screenWidth, screenHeight);
    aspectRatios[0][0] = screenWidth / g;
    aspectRatios[0][1] = screenHeight / g;
}

#endif // GRAPHICS_H