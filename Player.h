#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "functions.h"
#include "tile.h"
#include <utility>


//The Player that will move around on the screen
class Player
{
    public:


        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 480;
        const int LEVEL_WIDTH = 4096;
        const int LEVEL_HEIGHT = 4096;
        const int TILE_WIDTH = 32;
        const int TILE_HEIGHT = 32;
        const int TOTAL_TILES = 16384;
		//The dimensions of the Player
		static const int Player_WIDTH = 95;
		static const int Player_HEIGHT = 159;

		//Maximum axis velocity of the Player
		static const int Player_VEL = 10;

        functions myfunctions  ;

		SDL_Color textColor = { 0, 0, 0 }; // setting color to black , you may even keep different colors for energy and health 
		TTF_Font *myFont = NULL ;


		LTexture  PlayerBodyTexture  ; 
		//Initializes the variables
		Player( SDL_Renderer* myRenderer);
		//This is the constructor of the player, takes in a renderer and sets that


		LTexture PlayerHealthTexture; 
		LTexture PlayerEnergyTexture; 


		SDL_Renderer* gRenderer ; 

		bool loadPlayer(); 

		//Takes key presses and adjusts the Player's velocity
		void handleEvent( SDL_Event& e );

		//Moves the Player and check collision against tiles
		void move( Tile *tiles[] );

		//Centers the camera over the Player
		void setCamera( SDL_Rect& camera );

		//Shows the Player on the screen
		void render( SDL_Rect& camera );

        bool touchesWall(Tile* tiles[] ) ; 

		void close();

    private:
		//Collision box of the Player
		SDL_Rect mBox;


		double myHealth=100; 
		double myHunger=0;
		
		//The velocity of the Player
		int mVelX, mVelY;

		pair<int,int> myState ={2,4}; 
		// the first part tells whether moving up or left or right or down , 
		//the second part tells that which phase you are in right now
		//state is initialised to 2,4
};
#endif 