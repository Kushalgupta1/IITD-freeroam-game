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
#include <set>
#include <map>
#include<cstdlib>

// #include "camera.h"


//The Player that will move around on the screen
class Player
{
    public:
		  std::map<int,std::pair<std::string,int>> tasks = 
		 { {1,{ "Take Ball from SAC and Drop at Main Ground",30}} ,
		 { 2 , {"Take book from LHC and return to main library",30}} , 
		 { 3 ,{"Take part in chess tournament in Karakoram" ,30}} ,
		 { 4 ,{"Go to SIT and do a research project" ,50}} ,
		 { 5 ,{"Do Quizzing in Nilgiri" ,30}} ,
		 { 6 ,{"Do dramtics in Jwalamuhkhi" ,30}} , 
		 { 7 , {"Take part in Literature Quiz in Shivalik", 30}} , 
		 { 8 ,{"Work on a Startup Plan in Zanskar",50}} ,
		 {9,{"Take Part in Singing Competition in Kailsah",30}} ,
		 {10,{"Take part in Dance Competition in Girnar",30}},
		 {11 , {"Do Gymming in Aravali",30}} , 
		 {12,{"Do Photo Editing in Udaigiri",30}} , 
		 {13,{"Do Competitive Programming in Satpura",30}} , 
		 {14,{"Do Debating in Himadri",40}} , 
		 {15,{"Take Medicines from Apollo and give to Friend in Vindhyachal",30} },
		 {16,{"Take Groceries from store and give to Friend in Kumaon",30}}
		 }; 
		  

		
		
		TTF_Font *myFont = NULL ; 

		std::set<int> myPendingTasks ; 

		void displayTextBox(int textBoxX, int textBoxY, int textBoxWidth, int textBoxHeight , Uint8 alpha);

		void displayMyText( std::string sentence,  int sentenceX, int sentenceY);

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
		

		string mySpriteSheet;
		int myCycleX;
		int myCycleY;
		int mySpriteWidth;
		int mySpriteHeight;
		int myRenderWidth;
		int myRenderHeight;
		int myBestState;

		void showPauseStateChart();
		// void pauseStateChartOther();
		
		string myName ;

		void Constructor(SDL_Renderer* myRenderer , int* width , int* height , std::string* GameMessage , int CycleX , int CycleY , int PlayerSpriteWidth , int PlayerSpriteHeight , int PlayerRenderHeight , int PlayerRenderWidth , string SpriteSheet , int bestState , std::string myname , string myFontFile);
		//Maximum axis velocity of the Player
		int Player_VEL =10;

		

        functions myfunctions  ;

		SDL_Color textColor = { 0, 0, 0 }; // setting color to black , you may even keep different colors for Happiness and health 
		

		void setProcess(bool *p);
		LTexture  PlayerBodyTexture  ; 
		//Initializes the variables
		//This is the constructor of the player, takes in a renderer and sets that


		LTexture PlayerHealthTexture; 
		LTexture PlayerHappinessTexture; 
		LTexture PlayerMoneyTexture;
		LTexture PlayerNameTexture;

		bool hasGroceries=false;
		bool hasMedicines=false;
		bool hasYulu=false;
		bool hasAssignment =false;
		bool hasBook =false;
		bool hasBall = false;

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

		void setName(std::string inputName);
		void updateParams( Tile *tileset2[] , Tile *tileset3[] , SDL_Rect player2 );

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
		void NetworkUpdate(int myStateFirst, int myStateSecond, int myXcoord , int myYcoord ,int Health , int Happiness, int Money);

		void renderOtherPlayer( SDL_Rect &camera);
		void close();

		SDL_Rect mBox;
		int getMyStateFirst();
		int getMyStateSecond();
		double myHealth=120; 
		double myHappiness=120;
		double myMoney = 120 ; 

		pair<int,int> myState ={2,4}; 
    private:
		//Collision box of the Player


		
		void EatFood(int type); 

		void GetYulu();
		void DropYulu();
		void Play();
		void PlayChess();
		void Sleep();
		void Dance();
		void Quizzing();
		void GiveGroceries();
		void GiveMeds();
		void DoCP();
		void DoSinging();
		void DoDrama();
		void DoDebating();
		void DoDance();
		void DoRest();
		void DoProject();
		void DoGymming();
		void DoStartup();
		void PickBook();
		void DropBook();
		void DoLitQuiz();
		void PickMeds();
		void DropMeds();
		void PickBall();
		void DropBall();
		void BuyGroceries();
		void Hospital();


		std::string *message ; 


		LTimer* gameTimer ; 


		SDL_Rect HealthBar = { 500 , 5, (int)(myHealth), 20 };
		SDL_Rect HappinessBar= {500 ,45, (int)(myHappiness) , 20};
		SDL_Rect MoneyBar= {500 ,85, (int)(myMoney) , 20};
		
		SDL_Renderer* gRenderer ; 
		//The velocity of the Player
		int mVelX, mVelY;
 
		// the first part tells whether moving up or left or right or down , 
		//the second part tells that which phase you are in right now
		//state is initialised to 2,4
};

#endif 