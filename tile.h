#ifndef TILE_H
#define TILE_H 

using namespace std;   


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "timer.h"
#include "functions.h"



class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

        const int TILE_WIDTH = 80;
      const int TILE_HEIGHT = 80;

		//Shows the tile
		void render( SDL_Renderer* gRenderer ,SDL_Rect& camera, LTexture *gTileTexture );

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

        Functions myfunctions  ;
    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

#endif
