#include "Button.h"
#include "texture.h"
#include <SDL2/SDL_mixer.h>

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	 Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
				


	// this is teh sprite associated with the buttion 
}

//this is called whenver window is resized. 
void LButton :: UpdateParameters(int width  , int height){
    WindowWidth =width ; 
    WindowHeight =height ; 
    mPosition.x = (int)((double)width * x);
    mPosition.y = (int)((double)height * y);
	ButtonWidth = (int)((double)WindowWidth * w);
    ButtonHeight= (int)((double)WindowHeight * h);
	
	// printf("Button Width is %d , the window width was %d , while the h was %f", ButtonWidth , WindowWidth ,w );

}

void LButton::handleEvent( SDL_Event* e  ,int NewState) 
// on every mouse event every button gets this function invoked and it checks 
{
	//If mouse event happened
	if( e->type == SDL_MOUSEBUTTONDOWN  && myState==0)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + ButtonWidth )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + ButtonHeight )
		{
			inside = false;
		}

		//Mouse is outside button
		if( inside )
		{
			myState =1 ;
			Mix_PlayChannel( -1, mySound, 0 );
		} 
		//Mouse is inside button
		
	}
	


	else if( e->type == SDL_MOUSEBUTTONUP && myState==1)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + ButtonWidth )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + ButtonHeight )
		{
			inside = false;
		}

		//Mouse is outside button
		if( inside )
		{
			*gameState=NewState;
			myState=0;
			gameTimer->start();
		}
		//Mouse is inside button
		
	}
}
	
void LButton::render()
{
	//Show current button sprite
    if(myState==0){
		// SDL_RenderClear(gRenderer);
		// printf("%d",myState);
		// printf("x is %d , y is %d , w is %d , h is %d\n",x,y,w,h);
		// printf("my x Position %d , my y Position %d ,myWidth  %d ,myHeight %d" , mPosition.x ,mPosition.y , ButtonWidth,ButtonHeight);
        MyTexture1.render(gRenderer ,mPosition.x,mPosition.y,ButtonWidth,ButtonHeight );
			// MyTexture1.render(gRenderer,mPosition.x,mPosition.y);

			// printf("My x position is %d , my y position is %d")
	}
    else{
		 MyTexture2.render(gRenderer ,mPosition.x,mPosition.y,ButtonWidth,ButtonHeight );
	}
	// gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y,  );
}


void LButton :: setTimer(LTimer * timer){
    gameTimer = timer; 
}

bool LButton :: InitialiseButton(int type ,LTimer* timer, int* State , double a, double b , double c, double d , std::string audiofile , SDL_Renderer* Renderer , std::string PrimaryImage, std::string SecondaryImage){

	// printf("Did this");
    ButtonType = type; 
    gameTimer = timer;
    gameState = State ;
    x=a; 
    y=b;
    w=c;
    h=d;
    audio=audiofile; 
    gRenderer=Renderer;
    ButtonWidth = (int)((double)WindowWidth * w);
    ButtonHeight= (int)((double)WindowHeight * h);
	// printf("Button Width is %d , the window width was %d , while the h was %f", ButtonWidth , WindowWidth ,w );

    if(!MyTexture1.loadFromFile(PrimaryImage,gRenderer)) {
        printf("Problem in loading button texture from image file %s \n",PrimaryImage);
        return false;
    }
    if(SecondaryImage.compare("")!=0){

        if(!MyTexture2.loadFromFile(SecondaryImage,gRenderer)) {
        printf("Problem in loading button texture from image file %s \n",SecondaryImage);
        return false;
    }}

	mySound = Mix_LoadWAV( audiofile.c_str() );
	if( mySound == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}

    
    return true;


    


}
	void LButton :: close(){
			MyTexture1.free();
			MyTexture2.free();
			Mix_FreeChunk(mySound);
			mySound=NULL;
	} 
