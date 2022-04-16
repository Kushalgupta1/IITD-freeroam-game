using namespace std;   


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "functions.h"


functions :: functions(){}

//first argument is the player , second argument is the wall/object 
bool functions :: checkCollision( SDL_Rect a, SDL_Rect b , int type ){
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
if(type==0) // for camera collision with wall 
{
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;}

    else{
        if(rightA-5<=leftB) return false;
        if(leftA+5>=rightB) return false;
        if(bottomA-5<=topB) return false;
        if(bottomA-5>=bottomB)return false;
        return true;

    }
}

