#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "functions.h"
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include <utility>


void Player::Constructor(SDL_Renderer* myRenderer , int* width , int* height , std::string *GameMessage)
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = Player_WIDTH;
	mBox.h = Player_HEIGHT;
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
    message=GameMessage;

    gRenderer = myRenderer ; 

    if (TTF_Init()==-1){
        printf("Failed to Initialise Player");
    } 



    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

bool Player :: loadPlayer()
{

    myFont = TTF_OpenFont( "EvilEmpire-4BBVK.ttf", 24 );
	if( myFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}

    printf("Was here"); 
    if( ! (PlayerBodyTexture.loadFromFile( "SpritePlayer1.png" ,gRenderer)) )
	{
		printf( "Failed to load Playe r texture!\n" );
		return false;
	}

    if( !PlayerHealthTexture.loadFromRenderedText( "Health" , textColor ,myFont, gRenderer) )
	{
		printf( "Failed to load Play er texture!\n" );
		return false;
	}

    if( !PlayerHappinessTexture.loadFromRenderedText( "Happiness" , textColor ,myFont, gRenderer) )
	{
		printf( "Failed to load Playe r texture!\n" );
		return false;
	}

    if( !PlayerMoneyTexture.loadFromRenderedText( "Money" , textColor ,myFont, gRenderer) )
	{
		printf( "Failed to load Playe r texture!\n" );
		return false;
	}
    return true;



}

bool Player ::touchesWall(Tile* tiles[] )
{
	
    //Go through the tiles
    for( int i = 0; i < LAYER2_TOTAL_TILES; ++i )
    { 
        //If the tile is a wall type tile 
		 //    if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        if( ( tiles[ i ]->getType() > 0 )  )
		//isse upar wali line thi pehle check karne ke liye ab ye kardi faltu mein 
        {
            //If the collision box touches  the wall tile
            if( myfunctions.checkCollision( mBox, tiles[ i ]->getBox() ,1) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

int Player ::onSpecialSquare(Tile* tiles[] )
{
	
    //Go through the tiles
    for( int i = 0; i < LAYER3_TOTAL_TILES; ++i )
    { 
        //If the tile is a wall type tile 
		 //    if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        if( ( tiles[ i ]->getType() > 0 )  )
		//isse upar wali line thi pehle check karne ke liye ab ye kardi faltu mein 
        {
            //If the collision box touches  the wall tile
            if( myfunctions.checkCollision( mBox, tiles[ i ]->getBox() ,1) )
            {
                return tiles[i]->getType();
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

void Player::handleEvent( SDL_Event& e , Tile *tiles[])
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN)
    {

        switch(e.key.keysym.sym)
        {   
            case SDLK_DOWN : if(myState.first == 0) myState.second+=1 ;else myState={0,4};break;
            case SDLK_LEFT : if(myState.first == 1) myState.second+=1;else myState={1,4};break;
            case SDLK_RIGHT : if(myState.first == 2) myState.second+=1;else myState={2,4};break;
            case SDLK_UP : if(myState.first ==3) myState.second+=1;else myState={3,4};break;

            
        }
        if (myState.second <0 ) myState.second+=12;
        if(myState.second>=12) myState.second-=12;
        

        //Adjust the velocity

        if(e.key.repeat==0){
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= Player_VEL; break;
            case SDLK_DOWN: mVelY += Player_VEL; break;
            case SDLK_LEFT: mVelX -= Player_VEL; break;
            case SDLK_RIGHT: mVelX += Player_VEL; break;
        }}
    }
    //If a key was released
    else if( e.type == SDL_KEYUP )
    {
        //Adjust the velocity

        // switch(e.key.keysym.sym)
        // {
        //     case SDLK_DOWN : myState={0,4};break;
        //     case SDLK_LEFT : myState={1,4};break;
        //     case SDLK_RIGHT : myState={2,4};break;
        //     case SDLK_UP : myState={3,4};break;
        // }

        // if (myState.second <0 ) myState.second+=12; 
        // if(myState.second>12) myState.second-=12;

        if(e.key.repeat==0){
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += Player_VEL; break;
            case SDLK_DOWN: mVelY -= Player_VEL; break;
            case SDLK_LEFT: mVelX += Player_VEL; break;
            case SDLK_RIGHT: mVelX -= Player_VEL; break;
        }

        }
    }

if(onSpecialSquare(tiles)){


    if((e.type==SDL_KEYDOWN) && (e.key.keysym.sym==SDLK_1)){
        switch(onSpecialSquare(tiles))
        {
            case 1 : if(!hasYulu)GetYulu(); else DropYulu(); break;
            case 2 : EatFood(0);break;
            
        }
    }
    }
}

void Player::updateParams( Tile *tileset2[] , Tile *tileset3[] )
{   
    //targetTime is the time set when a process starts
    if(*processGoingOn && gameTimer->getTicks()>=targetTime){
        spending=false;happying=false;recharging=false;
        mSpendRate=0;mHappyRate=0;mRechargeRate=0;
        *processGoingOn=false;
    }

    
    //Move the Player left or right
    mBox.x += mVelX;

    //If the Player went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + Player_WIDTH > LEVEL_WIDTH ) || touchesWall( tileset2 ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the Player up or down
    mBox.y += mVelY;

    //If the Player went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + Player_HEIGHT > LEVEL_HEIGHT ) || touchesWall( tileset2 ) )
    {
        //move back
        mBox.y -= mVelY;
    }

    *message ="FreeRoam";
    if(!(*processGoingOn) && onSpecialSquare(tileset3)){
        switch(onSpecialSquare(tileset3)){
            case 1 : if(!hasYulu){*message="Press 1 to get Yulu";} else {*message="Press 1 to Leave Yulu";} break;
            case 2 : *message = "Press 1 to eat food"; break;
        }
    }

    
    if(spending){
        myMoney+=mSpendRate;
    }
    if(happying){
        myHappiness+=mHappyRate;
    }
    if(recharging){
        myHealth+=mRechargeRate;
    }
}


void Player ::updateScreen(int* width, int* height){
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
}
void Player::setCamera( SDL_Rect &camera )
{
	//Center the camera over the Player
	(camera).x = ( mBox.x + Player_WIDTH / 2 ) - *SCREEN_WIDTH / 2;
	(camera).y = ( mBox.y + Player_HEIGHT / 2 ) - *SCREEN_HEIGHT / 2;
    (camera).w = *SCREEN_WIDTH  ;
    (camera).h = *SCREEN_HEIGHT  ;
	//Keep the camera in bounds
	if( (camera).x < 0 )
	{ 
		(camera).x = 0;
	}
	if( (camera).y < 0 )
	{
		(camera).y = 0;
	}
	if( (camera).x > LEVEL_WIDTH - (camera).w) 
	{
		(camera).x = LEVEL_WIDTH - ((camera).w);
	}
	if( (camera).y > LEVEL_HEIGHT - ((camera).h ))
	{
		(camera).y = LEVEL_HEIGHT - ((camera).h);
	}
}

void Player::render( SDL_Rect &camera  )
{
    SDL_Rect myClip ={ 95*myState.second , 159*myState.first, 95,159};
    //Show the Player
	(PlayerBodyTexture).render(gRenderer, mBox.x - (camera).x, mBox.y - (camera).y ,48,80 , &myClip);
    PlayerHealthTexture.render(gRenderer , 400 , 0 ) ; 
    PlayerHappinessTexture.render(gRenderer, 400 , 50);
    PlayerMoneyTexture.render(gRenderer,400 , 100) ; 


     HealthBar = { 500 , 5, (int)(myHealth), 20 };
    HappinessBar= {500 ,45, (int)(myHappiness) , 20};
	MoneyBar= {500 ,85, (int)(myMoney) , 20};
    
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HealthBar );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HappinessBar );

     SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &MoneyBar );
}


void Player::close(){
    PlayerBodyTexture.free();
    PlayerHealthTexture.free();
    PlayerHappinessTexture.free();
}

void Player :: EatFood(int type){
    //Hostel Food for type 0 , Shop food for type 1 
    
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+5000 ; 
    recharging=true;
    mRechargeRate=1 ; 
    if(type==1) mSpendRate=-1;
	if(myHealth>100)myHealth=100;
    
}

void Player :: GetYulu(){
    if(!hasYulu){
    Player_VEL +=15;
    spending=true;
    mSpendRate = -0.02;
    hasYulu =true;}
}
void Player :: DropYulu(){
    if(hasYulu){
        Player_VEL-=15;
        spending=false;
        mSpendRate=0;
        hasYulu=false;
    }
}

void Player::Play(){
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+5000 ; 
    mHappyRate=5;
    mRechargeRate=-2;
}

// void GetYulu(){
		// 	mySpeed=20;
		// 	hasYulu =true;
		// 	moneyDecreaserate=-1;
		// }

		// void LeaveYulu(){
		// 	mySpeed=10;
		// 	hasYulu=false;
		// 	moneyDecreaserate=0;
		// }
		// void SubmitAssignment(){
		// 	if(Assignments>0){Assignment-=1;}
			
		// }
		// void TakeBall(){
		// 	NumBalls+=1;
		// }
		// void GiveBall(){
		// 	if(NumBalls>0){NumBalls-=1;}
		// }
		// void sleep(){
		// 	myHealth=100;
		// }
		// void playChess(){

		// }
		// void Dance(){

		// }
		// void Quizzing(){

		// }
		// void Play(){
		// 	happiness++
		// 	energy--
		// }
		// void study(){
		// 	energy--;
		// 	happiness++
		// }

void Player :: setTimer(LTimer * timer){
    gameTimer=timer;
}

void Player :: setProcess(bool *p){
        processGoingOn=p;
}