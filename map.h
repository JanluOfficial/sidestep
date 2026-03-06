#ifndef PROCEDURAL_MAP_H
#define PROCEDURAL_MAP_H

#define MAP_WIDTH 7
#define MAP_HEIGHT 12
#define INITIAL_PATH_X 3

#define GEN_PATH 0
#define GEN_STRUCTURE 1

#define STRUCT_NONE -1
#define STRUCT_SPS 0 // Simple Path Split
#define STRUCT_WIDEOPEN 1 // Wide Open area to hop in between

#define STRUCT_HEIGHT 14



class ProceduralMap {
public:
    int map[MAP_HEIGHT][MAP_WIDTH];
    
    int genType = GEN_PATH;
    float likelihoodOfPathChange = 0.0f;
    float likelihoodOfStructure = 0.0f;

    ProceduralMap();
    void generateNextSegment();
    
private:
    int ix, iy;
    int pathX = INITIAL_PATH_X;

    int structureToGen = STRUCT_NONE;
    int structGenY = 0;

    int map_structures[2][STRUCT_HEIGHT][7] = {
    {
        {0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0}
    },
    {
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
    }
};
};

#endif // PROCEDURAL_MAP_H