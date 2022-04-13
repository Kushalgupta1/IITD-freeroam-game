#ifndef FUNCTIONS_H
#define FUNCTIONS_H

using namespace std;   


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"

class Functions{
    public : 
    Functions();
bool checkCollision( SDL_Rect a, SDL_Rect b );
};
#endif