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
#include <map>
// #include "camera.h"


//The Player that will move around on the screen
class Player
{
    public:




		bool* processGoingOn=NULL;
        int* SCREEN_WIDTH ;
        int* SCREEN_HEIGHT;
        const int LEVEL_WIDTH = 12800;
    	const int LEVEL_HEIGHT = 6400;
        const int TILE_WIDTH = 32;
        const int TILE_HEIGHT = 32;
        const int LAYER1_TOTAL_TILES = 80000;
		const int LAYER2_TOTAL_TILES = 80000;
		const int LAYER3_TOTAL_TILES= 80000;
		//The dimensions of the Player
		static const int Player_WIDTH = 48;
		static const int Player_HEIGHT = 80;

		//Maximum axis velocity of the Player
		int Player_VEL =10;

        functions myfunctions  ;

		SDL_Color textColor = { 0, 0, 0 }; // setting color to black , you may even keep different colors for Happiness and health 
		TTF_Font *myFont = NULL ;

		void setProcess(bool *p);
		LTexture  PlayerBodyTexture  ; 
		//Initializes the variables
		void Constructor(SDL_Renderer* myRenderer , int* width , int* height , std::string* GameMessage);
		//This is the constructor of the player, takes in a renderer and sets that


		LTexture PlayerHealthTexture; 
		LTexture PlayerHappinessTexture; 
		LTexture PlayerMoneyTexture;

		bool hasYulu=false;

		bool moving =false; 
		bool spending =false ; 
		bool happying=false;
		bool recharging =false; 
		double mSpendRate ; 
		double mRechargeRate;
		double mHappyRate;

		bool loadPlayer(); 
		int targetTime;

		//Takes key presses and adjusts the Player's velocity
		void handleEvent(  SDL_Event& e , Tile *tiles[]);

		//Updates the Player position and check collision against tiles, also update player health according to health velocity or decrease money according to money velocity 

		void updateParams( Tile *tileset2[] , Tile *tileset3[] );

		//Centers the camera over the Player
		void setCamera( SDL_Rect &camera );

		//Shows the Player on the screen
		void render( SDL_Rect& camera );

        bool touchesWall(Tile* tiles[] ) ; 

		int onSpecialSquare(Tile* tiles[] );

		int Assignments ; 
		int Money;

		void setTimer (LTimer* gameTimer);
		void updateScreen(int* widhh, int* height);
		void close();

    private:
		//Collision box of the Player
		SDL_Rect mBox;

		double myHealth=30; 
		double myHappiness=50;
		double myMoney = 100 ; 

		
		void EatFood(int type); 

		void GetYulu();
		void DropYulu();
		void Play();
		void PlayChess();
		void Sleep();
		void Dance();

		std::string *message ; 


		LTimer* gameTimer ; 


		SDL_Rect HealthBar = { 500 , 5, (int)(myHealth), 20 };
		SDL_Rect HappinessBar= {500 ,45, (int)(myHappiness) , 20};
		SDL_Rect MoneyBar= {500 ,85, (int)(myMoney) , 20};
		
		SDL_Renderer* gRenderer ; 
		//The velocity of the Player
		int mVelX, mVelY;
 
		pair<int,int> myState ={2,4}; 
		// the first part tells whether moving up or left or right or down , 
		//the second part tells that which phase you are in right now
		//state is initialised to 2,4
};

#endif 