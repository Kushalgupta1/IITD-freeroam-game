// #include "Button.h"

// LButton::LButton()
// {
// 	mPosition.x = 0;
// 	mPosition.y = 0;

// 	// this is teh sprite associated with the buttion 
// }

// void LButton :: UpdateParameters(int x  , int y){
//     WindowWidth =x ; 
//     WindowHeight =y ; 

// }
// void LButton::setPosition( int x, int y )
// {
// 	mPosition.x = x;
// 	mPosition.y = y;
// }

// void LButton::handleEvent( SDL_Event* e ) 
// // on every mouse event every button gets this function invoked and it checks 
// {
// 	//If mouse event happened
// 	if( e->type == SDL_MOUSEBUTTONDOWN )
// 	{
// 		//Get mouse position
// 		int x, y;
// 		SDL_GetMouseState( &x, &y );

// 		//Check if mouse is in button
// 		bool inside = true;

// 		//Mouse is left of the button
// 		if( x < mPosition.x )
// 		{
// 			inside = false;
// 		}
// 		//Mouse is right of the button
// 		else if( x > mPosition.x + BUTTON_WIDTH )
// 		{
// 			inside = false;
// 		}
// 		//Mouse above the button
// 		else if( y < mPosition.y )
// 		{
// 			inside = false;
// 		}
// 		//Mouse below the button
// 		else if( y > mPosition.y + BUTTON_HEIGHT )
// 		{
// 			inside = false;
// 		}

// 		//Mouse is outside button
// 		if( inside )
// 		{
// 			*GameState =1 ;
// 		}
// 		//Mouse is inside button
		
// 		}
// 	}
// }
	
// void LButton::render()
// {
// 	//Show current button sprite
// 	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
// }
