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

        const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;
		const int LEVEL_WIDTH = 4096;
		const int LEVEL_HEIGHT = 4096;
		const int TILE_WIDTH = 32;
		const int TILE_HEIGHT = 32;
		const int TOTAL_TILES = 16384;

		//Initializes position and type
		Tile( int x, int y, int tileType );


		//Shows the tile
		void render( SDL_Renderer* gRenderer ,SDL_Rect& camera, LTexture *gTileTexture );

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

        functions myfunctions  ;
    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

#endif
