/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "timer.h"
#include "tile.h"
#include "functions.h"
#include <iostream>


//Screen dimension constants
#include <sstream>
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//The dimensions of the level
const int LEVEL_WIDTH = 4096;
const int LEVEL_HEIGHT = 4096;

//Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 16384;
// const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
// const int TILE_RED = 0;
// const int TILE_GREEN = 1;
// const int TILE_BLUE = 2;
// const int TILE_CENTER = 3;
// const int TILE_TOP = 4;
// const int TILE_TOPRIGHT = 5;
// const int TILE_RIGHT = 6;
// const int TILE_BOTTOMRIGHT = 7;
// const int TILE_BOTTOM = 8;
// const int TILE_BOTTOMLEFT = 9;
// const int TILE_LEFT = 10;
// const int TILE_TOPLEFT = 11;

//Texture wrapper class


//The tile


//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move( Tile *tiles[] );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		void render( SDL_Rect& camera );

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( Tile* tiles[] );

//Frees media and shuts down SDL
void close( Tile* tiles[] );

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//Sets tiles from tile map
bool setTiles( Tile *tiles[] );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;

// SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];




SDL_Rect Tile::getBox()
{
    return mBox;
}

Dot::Dot()
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move( Tile *tiles[] )
{
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setCamera( SDL_Rect& camera )
{
	//Center the camera over the dot
	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::render( SDL_Rect& camera )
{
    //Show the dot
	gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia( Tile* tiles[] )
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "dot.bmp" ,gRenderer) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "tileset.png" ,gRenderer) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	return success;
}

void close( Tile* tiles[] )
{
	//Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] != NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;
	char trash ; 

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "victorian-preview-array.txt" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; i++ )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;
			map>>trash ;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				std::cout<<"Error loading map: Unexpected end of file!\n "<<i ;
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) ) //originally there was tileType< TotalTileSPrites 
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		
		//Clip the sprite sheet
		// if( tilesLoaded )
		// {
		// 	gTileClips[ TILE_RED ].x = 0;
		// 	gTileClips[ TILE_RED ].y = 0;
		// 	gTileClips[ TILE_RED ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_RED ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_GREEN ].x = 0;
		// 	gTileClips[ TILE_GREEN ].y = 80;
		// 	gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_BLUE ].x = 0;
		// 	gTileClips[ TILE_BLUE ].y = 160;
		// 	gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_TOPLEFT ].x = 80;
		// 	gTileClips[ TILE_TOPLEFT ].y = 0;
		// 	gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_LEFT ].x = 80;
		// 	gTileClips[ TILE_LEFT ].y = 80;
		// 	gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_BOTTOMLEFT ].x = 80;
		// 	gTileClips[ TILE_BOTTOMLEFT ].y = 160;
		// 	gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_TOP ].x = 160;
		// 	gTileClips[ TILE_TOP ].y = 0;
		// 	gTileClips[ TILE_TOP ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_CENTER ].x = 160;
		// 	gTileClips[ TILE_CENTER ].y = 80;
		// 	gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_BOTTOM ].x = 160;
		// 	gTileClips[ TILE_BOTTOM ].y = 160;
		// 	gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_TOPRIGHT ].x = 240;
		// 	gTileClips[ TILE_TOPRIGHT ].y = 0;
		// 	gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_RIGHT ].x = 240;
		// 	gTileClips[ TILE_RIGHT ].y = 80;
		// 	gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

		// 	gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
		// 	gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
		// 	gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
		// 	gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		// }
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] ) // this is totally wrong as of now  , I just return a plain true , 
{
	return false; 
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
		//    if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        if( ( tiles[ i ]->getType() >= 12 ) && ( tiles[ i ]->getType() <= 12 ) )
		//isse upar wali line thi pehle check karne ke liye ab ye kardi faltu mein 
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//The level tiles
		Tile* tileSet[ TOTAL_TILES ];

		//Load media
		if( !loadMedia( tileSet ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//The frames per second timer
			LTimer fpsTimer;

			//The frames per second cap timer
			LTimer capTimer;

			//In memory text stream
			std::stringstream timeText;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();

			//While application is running
			while( !quit )
			{
				//Start cap timer
				capTimer.start();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot
				dot.move( tileSet );
				dot.setCamera( camera );

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x1F, 0x88, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( gRenderer, camera, &gTileTexture );
				}

				//Render dot
				dot.render( camera );

				//Update screen
				SDL_RenderPresent( gRenderer );

				 ++countedFrames;

				//If frame finished early
				// int frameTicks = capTimer.getTicks();
				// if( frameTicks < SCREEN_TICK_PER_FRAME )
				// {
				// 	//Wait remaining time
				// 	SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				// }

			}
		}
		
		//Free resources and close SDL
		close( tileSet );
	}

	return 0;
}