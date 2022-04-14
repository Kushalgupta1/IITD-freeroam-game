#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "timer.h"
#include "tile.h"
#include "functions.h"



Tile::Tile( int x, int y, int tileType )
// x and y tell you the pixel location of the tile on the map , render accordingly , 
// while tyle type tells you the location of the tile on the spritesheet
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Renderer* gRenderer ,  SDL_Rect& camera ,LTexture *gTileTexture)
{
    //If the tile is on screen
    int tyleX = 32*(mType%65) ; 
    int tyleY = (mType/65) ; 
     tyleY = 32*tyleY;

    SDL_Rect renderQuad = { tyleX, tyleY, mBox.w, mBox.h };
    if( myfunctions.checkCollision( camera, mBox ) )
    {
        //Show the tile
        (*gTileTexture).render( gRenderer,mBox.x - camera.x, mBox.y - camera.y, &renderQuad );
    }
}

int Tile::getType()
{
    return mType;
}