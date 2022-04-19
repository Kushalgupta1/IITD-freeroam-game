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
#include<cstdlib>
#include <time.h>


void Player::Constructor(SDL_Renderer* myRenderer , int* width , int* height , std::string* GameMessage , int CycleX , int CycleY , int PlayerSpriteWidth , int PlayerSpriteHeight , int PlayerRenderHeight , int PlayerRenderWidth , string SpriteSheet , int bestState , string myname , string myFontFile)
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = PlayerRenderWidth;
	mBox.h = PlayerRenderHeight;
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
    message=GameMessage;
    myCycleX=CycleX;
    myCycleY = CycleY;
    mySpriteWidth = PlayerSpriteWidth;
    mySpriteHeight = PlayerSpriteHeight;
    myRenderHeight = PlayerRenderHeight;
    myRenderWidth = PlayerRenderWidth ; 
    myBestState = bestState ; 
    mySpriteSheet = SpriteSheet;
    gRenderer = myRenderer ; 
    myName=myname;
    myFont = TTF_OpenFont(myFontFile.c_str(), 35);
    if (myFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }


    int numtasks = 0 ; 
    int x;
    //initialise the tasks the player tasks to be performed . 
    srand(time(0));
    while(numtasks<5){
        x=1 + (rand()%16);
        if(myPendingTasks.find(x)!=myPendingTasks.end()){}
        else{myPendingTasks.insert(x);numtasks++;}

    }

    if (TTF_Init()==-1){
        printf("Failed to Initialise Player");
    } 



    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

//used to set name of the player 
void Player :: setName(std::string inputName ){
    myName=inputName;
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
    if( ! (PlayerBodyTexture.loadFromFile( mySpriteSheet ,gRenderer)) )
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
            case SDLK_DOWN : if(myState.first == 0) myState.second+=1 ;else myState={0,myBestState};break;

            case SDLK_LEFT : if(myState.first == 1) myState.second+=1;else myState={1,myBestState};break;

            case SDLK_RIGHT : if(myState.first == 2) myState.second+=1;else myState={2,myBestState};break;
            
            case SDLK_UP : if(myState.first ==3) myState.second+=1;else myState={3,myBestState};break;

            
        }
        if (myState.second <0 ) myState.second+=myCycleX;
        if(myState.second>=myCycleX) myState.second-=myCycleX;
        

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

void Player::updateParams( Tile *tileset2[] , Tile *tileset3[] , SDL_Rect player2)
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
    if( ( mBox.x < 0 ) || ( mBox.x + myRenderWidth > LEVEL_WIDTH ) || touchesWall( tileset2 ) || myfunctions.checkCollision(mBox,player2,0))
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the Player up or down
    mBox.y += mVelY;

    //If the Player went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + myRenderHeight> LEVEL_HEIGHT ) || touchesWall( tileset2 ) )
    {
        //move back
        mBox.y -= mVelY;
    }

    *message ="FreeRoam";
    if(!(*processGoingOn) && onSpecialSquare(tileset3)){
        switch(onSpecialSquare(tileset3)){
            case 1 : *message = "UDAIGIRI : Press 1 to do Photo Editing , Press 2 to have food"; break;
            case 2 : if(!hasYulu){*message="YULU STAND :Press 1 to get Yulu";} else {*message="YULU STAND :: Press 1 to Leave Yulu";} break;
            case 3 : if(hasGroceries){*message = "KUMAON : Press 1 to give groceries";} else{*message="KUMAON";}  break ;
            case 4 : if(hasMedicines){*message ="VINDHYACHAL : Press 1 to give medicines";} else {*message="VINDHYACHAL";} break ;
            case 5 : *message="MECHANICAL DEPARTMENT";  break ;
            case 6 : *message = "SATPURA : Press 1 to take Part in Competitive Programming Contest";  break ;
            case 7 : *message="KAILASH : Press 1 to take Part in singing competition";  break ;
            case 8 : *message  = "ELECTRICAL DEPARTMENT"; break ;
            case 9 : *message   = "JWALAMUKHI : Press 1 to do Dramatics" ;break ;
            case 10 : *message = "HIMADRI : Press 1 to do Debating"; break ;
            case 11 : *message  = "GIRNAR : Press 1 to take part in Dance Competition"; break ;
            case 12 : *message  = "BIOTECH DEPARTMENT" ;break ;
            case 13 : *message  = "BIOTECH LAWN : Press 1 to Rest and Relax"; break ;
            case 14 : *message   = "CAFE COFFEE DAY : Press 1 to have Coffee "; break ;
            case 15 : *message   = "IIT-D MAIN BUILDING" ; break ;
            case 16 : *message  = "TEXTILE DEPARTMENT" ;  break ;
            case 17 : *message   = "SATPURA NIGHT MESS : Press 1 to have food" ; break ;
            case 18 : *message  = "NESCAFE : Press 1 to have Coffee and Snacks" ; break ;
            case 19 : *message  = "DEPARTMENT OF DESIGN"; break ;
            case 20 : *message   = "SIT : Press 1 to do a Research Project here" ;break ;
            case 21 : *message   = "DELHI 16 : Press 1 to have food  ";break ;
            case 22 : *message  = "ARAVALI : Press 1 to do gymming. " ;break ;
            case 23 : *message   = "ZANSKAR : Press 1 to work on a startup plan. ";break ;
            case 24 : *message  = "ARAVALI NIGHT MESS : Press 1 to have juice/shake.";break ;
            case 25 : if(!hasBook){*message  = "LECTURE HALL COMPLEX : Press 1 to take book";} else{*message="LECTURE HALL COMPLEX";}break ;
            case 26 : *message = "VOLLEYBALL COURT: Press 1 to play";  break ;
            case 27 : if(hasBook){*message = "LIBRARY : Press 1 to return book"; } else{*message="LIBRARY";}break ;
            case 28 : *message = "LAWN TENNIS COURT : Press 1 to play Tennis"; break ;
            case 29 : *message = "RAJDHANI : Press 1 to have food";  break ;
            case 30 : *message ="SHIVALIK : Press 1 to take part in Literature Quiz"; break ;
            case 31 : *message ="BHARTI BUILDING"; break ;
            case 32 : *message  = "MASALA MIX : Press 1 to have snacks" ; break ;
            case 33 : if(!hasMedicines){*message  = "APOLLO Pharma: Press 1 to pickup medicines for friend";}else{*message = "APOLLO Pharma";}break ;
            case 34 : *message = "HOSPITAL : Press 1 to recharge health";  break ;
            case 35 : *message = "KARAKORAM : Press 1 to play chess";  break ;
            case 36 : *message = "STAFF CANTEEN : Press 1 to have food"; break ;
            case 37 : *message  = "NALANDA GROUND : Press 1 to relax here";break ;
            case 38 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 39 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 40 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 41 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 42 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 43 : *message = "NILGIRI : Press 1 to do Quizzing , Press 2 to sleep"; break ;
            case 44 : if(hasBall) {*message  ="MAIN GROUND : Press 1 to give ball.";} else{"MAIN GROUND";}break ;
            case 45 : *message= "NILGIRI NIGHT MESS : Press 1 to have food"  ;break ;
            case 46 : if(!hasBall){*message = "SAC : Press 1 to take ball";} else{ "STUDENT ACTIVITY CENTER";} break ;
            case 47 : *message = "Department of Management Studies";  break ;
            case 48 : if(!hasGroceries){*message  = "GROCERY SHOP : Press 1 to buy groceries for friend";} else {"GROCERY SHOP";}break ;

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
	(camera).x = ( mBox.x + myRenderWidth / 2 ) - *SCREEN_WIDTH / 2;
	(camera).y = ( mBox.y + myRenderHeight / 2 ) - *SCREEN_HEIGHT / 2;
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

int Player::getMyStateFirst(){
    return myState.first;
}
int Player::getMyStateSecond(){
    return myState.second;
}

void Player::render( SDL_Rect &camera  )
{
    SDL_Rect myClip ={ mySpriteWidth*myState.second , mySpriteHeight*myState.first, mySpriteWidth,mySpriteHeight};
    //Show the Player

    if( !PlayerNameTexture.loadFromRenderedText( myName, textColor ,myFont, gRenderer) )
	{
		printf( "Failed to load Play er texture!\n" );
	}

    displayTextBox(*SCREEN_WIDTH-240,0,240,250,10);
	(PlayerBodyTexture).render(gRenderer, mBox.x - (camera).x, mBox.y - (camera).y ,myRenderWidth,myRenderHeight , &myClip);
    PlayerNameTexture.render(gRenderer , *SCREEN_WIDTH -150 ,0);
    PlayerHealthTexture.render(gRenderer , *SCREEN_WIDTH-180 , 40 ) ; 
    PlayerHappinessTexture.render(gRenderer, *SCREEN_WIDTH-204, 80);
    PlayerMoneyTexture.render(gRenderer,*SCREEN_WIDTH-180 , 140) ; 


     HealthBar = { *SCREEN_WIDTH-104 , 45, (int)(myHealth), 20 };
    HappinessBar= {*SCREEN_WIDTH-104 ,85, (int)(myHappiness) , 20};
	MoneyBar= {*SCREEN_WIDTH-104 ,145, (int)(myMoney) , 20};
    
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HealthBar );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HappinessBar );

     SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &MoneyBar );

    string taskdisplay = string("Tasks Pending : ") + to_string(myPendingTasks.size()) ; 

    displayMyText(taskdisplay,*SCREEN_WIDTH - 180,180);
}

void Player::NetworkUpdate(int myStateFirst, int myStateSecond, int myXcoord , int myYcoord ,int Health , int Happiness, int Money){
    myState.first=myStateFirst;
    myState.second = myStateSecond;
    mBox.x = myXcoord ; 
    mBox.y = myYcoord ; 
    myHealth=Health;
    myHappiness =Happiness;
    myMoney = Money;
}

//pass player 1 or window screen width here. 
void Player::renderOtherPlayer( SDL_Rect &camera   )
{
    SDL_Rect myClip ={ mySpriteWidth*myState.second , mySpriteHeight*myState.first, mySpriteWidth,mySpriteHeight};
    //Show the Player
    if( !PlayerNameTexture.loadFromRenderedText( myName, textColor ,myFont, gRenderer) )
	{
		printf( "Failed to load Play er texture!\n" );
	}
    displayTextBox(0,0,240,250,10);
	(PlayerBodyTexture).render(gRenderer, mBox.x - (camera).x, mBox.y - (camera).y ,myRenderWidth,myRenderHeight , &myClip);
    PlayerNameTexture.render(gRenderer , 34 , 0 );
    PlayerHealthTexture.render(gRenderer , 4 , 40 ) ; 
    PlayerHappinessTexture.render(gRenderer, 4, 80);
    PlayerMoneyTexture.render(gRenderer,4 , 140) ; 


     HealthBar = { 80, 45, (int)(myHealth), 20 };
    HappinessBar= {100,85, (int)(myHappiness) , 20};
	MoneyBar= {90 ,145, (int)(myMoney) , 20};
    
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HealthBar );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
    SDL_RenderFillRect( gRenderer, &HappinessBar );

     SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
    SDL_RenderFillRect( gRenderer, &MoneyBar );

    string taskdisplay = string("Tasks Pending : ") + to_string(myPendingTasks.size()) ; 

    displayMyText(taskdisplay,4,180);
}

void Player::showPauseStateChart(){

        displayTextBox(0 , 0 ,200 ,300 , 10);
        string name = string("Player Name : ") + myName;
        displayMyText(name,5 ,5);
        string healthval = string("Player Health : ")+ to_string((int)myHealth);
        displayMyText(healthval , 5 ,40) ; 

        string happinessval = string("Player Happiness : ")+ to_string((int)myHappiness);
        displayMyText(happinessval , 5 ,80) ; 

        string moneyval = string("Player Money : ")+ to_string((int)myMoney);
        displayMyText(moneyval , 5 ,120) ; 

        string playerspeed = string("Player Speed : ")+to_string(Player_VEL);
        displayMyText(playerspeed , 5 ,160);\

        displayTextBox(*SCREEN_WIDTH-200,0,200,300,21);


        displayMyText("Tasks Pending :",*SCREEN_WIDTH-180 , 6);

        int i=1;
        for(auto it=myPendingTasks.begin() ; it!=myPendingTasks.end();it++){
        displayMyText((tasks[(*it)]).first,*SCREEN_WIDTH-300,40*i);i++;}

}

void Player:: displayTextBox(int textBoxX, int textBoxY, int textBoxWidth, int textBoxHeight ,Uint8 alpha){

    // SDL_Rect textbox = {(int)(((double)(*SCREEN_WIDTH)) * textBoxX), (int)(((double)(*SCREEN_HEIGHT)) * textBoxY), (int)(((double)(*SCREEN_WIDTH)) * textBoxWidth), (int)(((double)(*SCREEN_HEIGHT)) * textBoxHeight)};
    SDL_Rect textbox = {textBoxX , textBoxY , textBoxWidth , textBoxHeight};

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, alpha);
    SDL_RenderFillRect(gRenderer, &textbox);
}

void Player::displayMyText( std::string sentence,  int sentenceX, int sentenceY)
{
    LTexture myTexture;
    SDL_Color textColor = {0, 0, 0};
    if (!myTexture.loadFromRenderedText(sentence, textColor, myFont, gRenderer))
    {
        printf("Error in loading texture for string %s", sentence);
    }

    

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    // myTexture.render(gRenderer,WindowWidth/3 , WindowHeight*9/10, WindowWidth/3 , WindowHeight/11 );

    // myTexture.render(gRenderer, (int)(((double)(*SCREEN_WIDTH)) * sentenceX), (int)(((double)(*SCREEN_HEIGHT)) * sentenceY));
    

     myTexture.render(gRenderer, sentenceX,sentenceY);

    myTexture.free();
}
void Player::close(){
    PlayerBodyTexture.free();
    PlayerHealthTexture.free();
    PlayerHappinessTexture.free();
    PlayerNameTexture.free();
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
    mSpendRate = -0.01;
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
    mHappyRate=0.04;
    mRechargeRate=-0.04;
}

		
void Player::PlayChess(){
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+5000 ; 
    mHappyRate = 0.03 ; 
    mRechargeRate = -0.03 ;

}
void Player::Sleep(){
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+10000 ; 
	mRechargeRate = 0.1;
		}

void Player::Dance(){
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+5000 ; 
	mRechargeRate = -0.03;
    mHappyRate = 0.06;

		}
void Player::Quizzing(){
    *processGoingOn=true;
    targetTime=(gameTimer->getTicks())+10000 ; 
	mRechargeRate = 0.1;
    
}

		// void SubmitAssignment(){
		// 	if(Assignments>0){Assignment-=1;}
			
		// }
		// void TakeBall(){
		// 	NumBalls+=1;
		// }
		// void GiveBall(){
		// 	if(NumBalls>0){NumBalls-=1;}
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