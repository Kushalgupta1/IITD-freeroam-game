#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;   


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"



class functions{


    public : 

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int LEVEL_WIDTH = 12800;
    const int LEVEL_HEIGHT = 6400;
    const int TILE_WIDTH = 32;
    const int TILE_HEIGHT = 32;
    const int LAYER1_TOTAL_TILES = 80000;
    const int LAYER2_TOTAL_TILES = 80000;

    functions();
bool checkCollision( SDL_Rect a, SDL_Rect b ,int type);
};
#endif