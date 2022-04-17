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
#include "Player.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Window.h"
#include <vector>
#include "Button.h"
#include "timer.h"
#include <SDL2/SDL_mixer.h>

// #include "camera.h"


//Screen dimension constants
#include <sstream>
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//The dimensions of the level
const int LEVEL_WIDTH = 12800;
const int LEVEL_HEIGHT = 6400;

//Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int LAYER1_TOTAL_TILES = 80000;
const int LAYER2_TOTAL_TILES = 80000;
const int LAYER3_TOTAL_TILES = 80000;

LWindow gWindow ; 

//This is the music that will play in the backgorund 
Mix_Music *BackgroundMusic = NULL;
// Mix_Chunk *mySound = NULL;


std::string message=" Hello " ; 

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( Tile* TilesLayer1[] , Tile* TilesLayer2[] ,Tile* TilesLayer3[]);

//Frees media and shuts down SDL
void close( Tile* tiles[] );

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
// bool touchesWall( SDL_Rect box, Tile* tiles[] );
//functions mein daal dita isko 

//Sets tiles from tile map
bool setTiles( Tile* TilesLayer1[] , Tile* TilesLayer2[] ,Tile* TilesLayer3[]  );

int gameState = 0 ; // State =0  is for not started, state=1 is for started . 




vector<LTexture> Textures(2) ; 
vector<std::string> Images{"SmallImage.png","BigImage.png"}; 

// //The window we'll be rendering to
// SDL_Window* gWindow = NULL ;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures

Player player1 ; 
LTexture gTileTexture1;
LTexture gTileTexture2;
LTexture StartTextureSmall;
LTexture StartTextureLarge;

// SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];


TTF_Font *gFont = NULL ; 

 enum State { Intro , Started} ; 

	


bool init()
{
	//Initialization flag
	bool success = true;

	gameState = 0 ; 
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
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
		if( !gWindow.init() )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{	

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				


				
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				

				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 16384) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

				player1.Constructor(gRenderer,&gWindow.mWidth,&gWindow.mHeight,&message);

			} 
		}
	}

	return success;
}

TTF_Font *textFont = NULL; 

bool loadMedia( Tile* TilesLayer1[] , Tile* TilesLayer2[] ,Tile* TilesLayer3[])
{
	//Loading success flag
	bool success = true;

	 textFont = TTF_OpenFont( "EvilEmpire-4BBVK.ttf", 28 );
	if( textFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

	BackgroundMusic = Mix_LoadMUS( "Fluffing-a-Duck.wav" );
	if( BackgroundMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	// mySound = Mix_LoadWAV( "mixkit-quick-win-video-game-notification-269.wav" );
	// if( mySound == NULL )
	// {
	// 	printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	return false;
	// }

	
	if( !player1.loadPlayer() )
	{
		printf( "Failed to load Pla yer !\n" );
		success = false;
	}

	//Load tile texture
	if( !gTileTexture1.loadFromFile( "TILESET_campus_layer1&2.png" ,gRenderer) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}
	if( !gTileTexture2.loadFromFile( "TILESET_campus_layer3.png" ,gRenderer) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}
	


	for(int i=0 ; i<2 ; i++){
		if(!Textures[i].loadFromFile(Images[i],gRenderer)){
			printf("Faield to load Texture from Image %s" , Images[i]);
			success=false;
		}
	}
	

	//Load tile map
	if( !setTiles(TilesLayer1 ,TilesLayer2 ,TilesLayer3) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	


	return success;
}

void close( Tile* TilesLayer1[] , Tile* TilesLayer2[]  )
{
	//Deallocate tiles
	for( int i = 0; i < LAYER1_TOTAL_TILES; ++i )
	{
		 if( TilesLayer1[ i ] != NULL )
		 {
			delete TilesLayer1[ i ];
			TilesLayer1[ i ] = NULL;
		 }
	}

	//Deallocate tiles
	for( int i = 0; i < LAYER2_TOTAL_TILES; ++i )
	{
		 if( TilesLayer2[ i ] != NULL )
		 {
			delete TilesLayer2[ i ];
			TilesLayer2[ i ] = NULL;
		 }
	}

	//Free loaded images
	player1.close();
	gTileTexture1.free();
	gTileTexture2.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	gWindow.free();

	// this is to close the background music 
	Mix_FreeMusic( BackgroundMusic );
	// Mix_FreeChunk(mySound);

	BackgroundMusic = NULL;
	// mySound=NULL;

	gRenderer = NULL;

	for(int i=0 ; i<2 ; i++){
		Textures[i].free();
	}
	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool setTiles( Tile* TilesLayer1[] , Tile* TilesLayer2[] ,Tile* TilesLayer3[] )
{
	//Success flag
	bool tilesLoaded = true;
	char trash ; 


    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map1( "TILEMAP_campus_layer1&2.txt" );

    //If the map couldn't be loaded
    if( map1.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		// int *(int,int) 
		//Initialize the tiles
		for( int i = 0; i < LAYER1_TOTAL_TILES; i++ )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map1 >> tileType;
			map1>>trash ; 

			//If the was a problem in reading the map
			if( map1.fail() )
			{
				//Stop loading map
				std::cout<<"Error loading map: Unexpected end of file!\n "<<i ;
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) ) //originally there was tileType< TotalTileSPrites 
			{
				TilesLayer1[ i ] = new Tile( x, y, tileType );
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
		
		
	}

    //Close the file
    map1.close();



	//The tile offsets
    x = 0;
	y = 0;

    //Open the map
    std::ifstream map2( "TILEMAP_campus_layer3.txt" );

    //If the map couldn't be loaded
    if( map2.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < LAYER2_TOTAL_TILES; i++ )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map2 >> tileType;
			map2>>trash ;

			//If the was a problem in reading the map
			if( map2.fail() )
			{
				//Stop loading map
				std::cout<<"Error loading map: Unexpected end of file!\n "<<i ;
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) ) //originally there was tileType< TotalTileSPrites 
			{
				TilesLayer2[ i ] = new Tile( x, y, tileType );
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
		






		
		
	}

    //Close the file
    map2.close();



	//The tile offsets
    x = 0;
	y = 0;

    //Open the map
    std::ifstream map3( "TILEMAP_campus_layer4.txt" );

    //If the map couldn't be loaded
    if( map3.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < LAYER3_TOTAL_TILES; i++ )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map3 >> tileType;
			map3>>trash ;

			//If the was a problem in reading the map
			if( map3.fail() )
			{
				//Stop loading map
				std::cout<<"Error loading map: Unexpected end of file!\n "<<i ;
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) ) //originally there was tileType< TotalTileSPrites 
			{
				TilesLayer3[ i ] = new Tile( x, y, tileType );
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
		map3.close();

    //If the map was loaded fine
    return tilesLoaded;
	}
}
 

void displayText(SDL_Renderer* gRenderer ,std::string sentence , int WindowWidth , int WindowHeight){
	LTexture myTexture;
	SDL_Color textColor={0,0,0};
	if(!myTexture.loadFromRenderedText(sentence ,textColor ,  textFont,gRenderer )){
		printf("Error in loading texture for string %s",sentence) ; 

	}

	SDL_Rect textbox = { WindowWidth / 5, WindowHeight*9/10, WindowWidth*2 / 3, WindowHeight/10};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &textbox );

	// myTexture.render(gRenderer,WindowWidth/3 , WindowHeight*9/10, WindowWidth/3 , WindowHeight/11 );
	myTexture.render(gRenderer ,WindowWidth/3 , WindowHeight*12/13 );
	myTexture.free();


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
		Tile* tileset1[ LAYER1_TOTAL_TILES ];
		Tile* tileset2[ LAYER2_TOTAL_TILES ];
		Tile* tileset3[LAYER3_TOTAL_TILES];

		//Load media
		if( !loadMedia( tileset1 ,tileset2,tileset3))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			

			//Level camera
			SDL_Rect camera ={0,0,gWindow.mWidth,gWindow.mHeight};
			
//The frames per second timer
			bool processGoingOn =false;
			player1.setProcess(&processGoingOn);
			LTimer fpsTimer;

			//The frames per second cap timer
			LTimer capTimer;

			LTimer gameTimer;
			player1.setTimer(&gameTimer);
			

			LButton gameStartButton; 
			gameStartButton.InitialiseButton(1,&gameTimer,&gameState,0.4,0.35,0.2,0.3,"audio",gRenderer,"start-up.png","start-down.png");

			LButton gamePauseButton;
	
			LButton gameResumeButton; 
			//In memory text stream
			std::stringstream timeText;

			//Start counting frames per second
			int countedFrames = 0;
				Mix_PlayMusic( BackgroundMusic, -1);
			fpsTimer.start();

			//While application is running
			while( !quit )
			{
				//Start cap timer
				capTimer.start();
				//Handle events on queue

			if(gameState==0){
				while( SDL_PollEvent( &e ) != 0)
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Player
					gWindow.handleEvent( e );
					gameStartButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameStartButton.handleEvent(&e , 1);

				}

				if( !gWindow.isMinimized() ){

					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );
					if(gWindow.mWidth==640 && gWindow.mHeight==480)
					Textures[0].render(gRenderer,0,0,gWindow.mWidth,gWindow.mHeight);
					else Textures[1].render(gRenderer,0,0,gWindow.mWidth,gWindow.mHeight);
					gameStartButton.render();
					SDL_RenderPresent(gRenderer);
				}

			}

			//this is the condition when game has started
			else{
				// Mix_PlayChannel( -1, mySound, 1 );
				gameStartButton.close();

				while( SDL_PollEvent( &e ) != 0  )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
						Mix_HaltMusic(); // not sure if this is required
						Mix_HaltMusic();
						Mix_Quit();
					}

					//Handle input for the Player
					gWindow.handleEvent( e );
					player1.updateScreen(&gWindow.mWidth , &gWindow.mHeight);
					


					if( !gWindow.isMinimized() && !processGoingOn ){player1.handleEvent( e ,tileset3 );}
				}


				if( !gWindow.isMinimized() ){
				//Move the dot
				player1.updateParams( tileset2 ,tileset3); //movement will revert only on base of tileset2
				player1.setCamera( camera);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x1F, 0x88, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render level
				for( int i = 0; i < LAYER1_TOTAL_TILES; ++i )
				{
					tileset1[ i ]->render( gRenderer, camera, &gTileTexture1 );
				}

				// Render level
				for( int i = 0; i < LAYER2_TOTAL_TILES; ++i )
				{
					tileset2[ i ]->render( gRenderer, camera, &gTileTexture2 );
				}

				//Render dot
				
				player1.render( camera  );

				//Update screen
				displayText(gRenderer,message,gWindow.mWidth,gWindow.mHeight) ; 
				SDL_RenderPresent( gRenderer );}
			}
				 ++countedFrames;

				// If frame finished early
				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
				// printf("Width is %d , and height is %d \n" ,gWindow.mWidth ,gWindow.mHeight );

			}
		}
		
		//Free resources and close SDL
		close( tileset1 , tileset2 );
	}

	return 0;
}