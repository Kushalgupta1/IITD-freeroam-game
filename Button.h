#ifndef BUTTON_H
#define BUTTON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "texture.h"
#include "timer.h"
#include <SDL2/SDL_mixer.h>

class LButton
{
	public:
		//Initializes internal variables
		LButton();
        LTexture MyTexture1 ; 
        LTexture MyTexture2 ; 

        bool loadButton();

        int WindowWidth = 640; 
        int WindowHeight = 480;
        int ButtonWidth ; 
        int ButtonHeight ; 

        void UpdateParameters(int x , int y );

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e  ,int NewState);
	
		//Shows button sprite
		void render();

        int* gameState ; 

        void setTimer(LTimer*);

        int ButtonType ; 
        double x,y,w,h;
        std::string audio ; // abhi isko kaafi kuchh karna hai , just wait


        bool InitialiseButton(int type ,LTimer* timer, int* State , double a, double b , double c, double d , std::string audiofile , SDL_Renderer* Renderer , std::string PrimaryImage, std::string SecondaryImage="");


        void close();
	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite, this is used to tell which sprtie it has that 
		//portion of the spritesheet will be shown on the portion assigned to this button
        int ButtonState ;
		
        SDL_Renderer* gRenderer;
        LTimer* gameTimer ; 
        int myState=0;
        Mix_Chunk *mySound = NULL;


};
#endif