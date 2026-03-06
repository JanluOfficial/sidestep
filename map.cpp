#include <cmath>
#include <algorithm>
using namespace std;
#include "map.h"

#define PATH_INC 0.1
#define STRUCT_INC 0.0005

ProceduralMap::ProceduralMap() {
    for (iy = 0; iy < MAP_HEIGHT; iy++) {
        for (ix = 0; ix < MAP_WIDTH; ix++) {
            map[iy][ix] = (ix == INITIAL_PATH_X) ? 1 : 0;
        }
    }
}

// TODO: Implement path splits

void ProceduralMap::generateNextSegment() {
    // Find current path position
    bool pathChanged = ((float)rand() / RAND_MAX) < likelihoodOfPathChange;
    bool generateStructure = ((float)rand() / RAND_MAX) < likelihoodOfStructure;

    if (genType == GEN_PATH) {
        int newX = 0;
        if (generateStructure) {
            genType = GEN_STRUCTURE;
            likelihoodOfStructure = 0;
            structureToGen = rand() % 2;

            if (structureToGen == STRUCT_SPS) {
                newX = 3;
            } else if (structureToGen == STRUCT_WIDEOPEN) {
                newX = 6;
            } 

            structGenY = 0;
            pathChanged = true;
        }
        if (pathChanged) {
            // Calculate new path position
            int direction = (pathX == 6) ? -1 : (pathX == 0) ? 1 : (rand() % 2 == 0) ? -1 : 1;
            if (!generateStructure) {
                newX = min(max(pathX + direction * (rand() % 3 + 1), 0), 6);
            }

            // Shift map up
            for (iy = 0; iy < 11; iy++) {
                for (ix = 0; ix < 7; ix++) {
                    map[iy][ix] = map[iy + 1][ix];
                }
            }

            for (ix = 0; ix < 7; ix++) {
                map[11][ix] = ((ix <= pathX && ix >= newX) || (ix >= pathX && ix <= newX)) ? 1 : 0; 
                //map[11][ix] = (ix == newX) ? 1 : 0;
            }

            likelihoodOfPathChange = 0.0f;
            likelihoodOfStructure += STRUCT_INC;
            pathX = newX;
        } else {
            // Shift map up without changing path
            for (iy = 0; iy < 11; iy++) {
                for (ix = 0; ix < 7; ix++) {
                    map[iy][ix] = map[iy + 1][ix];
                }
            }

            for (ix = 0; ix < 7; ix++) {
                map[11][ix] = (ix == pathX) ? 1 : 0;
            }

            likelihoodOfPathChange += PATH_INC;
        }
    } else if (genType == GEN_STRUCTURE) {
        if (structGenY < STRUCT_HEIGHT) {
            for (iy = 0; iy < 11; iy++) {
                for (ix = 0; ix < 7; ix++) {
                    map[iy][ix] = map[iy + 1][ix];
                }
            }
            for (int i = 0; i < 7; i++) {
                map[11][i] = map_structures[structureToGen][structGenY][i];
            }
            structGenY++;
            if (structGenY >= STRUCT_HEIGHT) {
                structureToGen = STRUCT_NONE;
                structGenY = 0;
                genType = GEN_PATH;
            }
        }
    }
}