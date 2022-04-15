#ifndef TEXTURE_H
#define TEXTURE_H 

using namespace std;   


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <SDL2/SDL_ttf.h>


class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path ,  SDL_Renderer*  gRenderer);
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		// bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor , TTF_Font* gFont , SDL_Renderer* gRenderer );


		//Renders texture at given point
		void render(  SDL_Renderer*  gRenderer,int x, int y,int resizex=0,int resizey =0,SDL_Rect* clip = NULL , double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );



		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif 