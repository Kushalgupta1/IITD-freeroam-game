/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "timer.h"
#include "tile.h"
#include "functions.h"
#include "Player.h"
#include <SDL2/SDL_ttf.h>
#include "Window.h"
#include <vector>
#include "Button.h"
#include <fcntl.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>
#include <stdlib.h>

#define PORT 9987
#include <sstream>>

// #include "camera.h"

// Screen dimension constants
const int SCREEN_FPS = 30;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The dimensions of the level
const int LEVEL_WIDTH = 12800;
const int LEVEL_HEIGHT = 6400;

// Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int LAYER1_TOTAL_TILES = 80000;
const int LAYER2_TOTAL_TILES = 80000;
const int LAYER3_TOTAL_TILES = 80000;

LWindow gWindow;

// This is the music that will play in the backgorund
Mix_Music *BackgroundMusic = NULL;
// Mix_Chunk *mySound = NULL;

std::string message = " Hello ";

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia(Tile *TilesLayer1[], Tile *TilesLayer2[], Tile *TilesLayer3[]);

bool nameTaken = false;
// Frees media and shuts down SDL
void close(Tile *tiles[]);

// Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

// Checks collision box against set of tiles
//  bool touchesWall( SDL_Rect box, Tile* tiles[] );
// functions mein daal dita isko

// Sets tiles from tile map
bool setTiles(Tile *TilesLayer1[], Tile *TilesLayer2[], Tile *TilesLayer3[]);

int gameState = 0; // State =0  is for not started, state=1 is for started .
int player2gameState = 0;

string player1name = "Enter Player Name";
string player2name = "Enter Player Name";

vector<LTexture> Textures(2);
vector<std::string> Images{"SmallImage.png", "BigImage.png"};

// //The window we'll be rendering to
// SDL_Window* gWindow = NULL ;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Scene textures

Player player1;
Player player2;
LTexture gTileTexture1;
LTexture gTileTexture2;
LTexture gInputTextTexture;
LTexture StartTextureSmall;
LTexture StartTextureLarge;

// SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

TTF_Font *gFont = NULL;

enum State
{
    Intro,
    Started
};

//  Client stuff

struct Info
{
    int stateFirst;
    int stateSecond;
    int myXcoord;
    int myYcoord;
    int myState;
    int health;
    int happiness;
    int money;
};

void toNetwork(char *buffer, struct Info *mydata)
{
    // 32 byte buffer required
    // test bit
    buffer[0] = '*';
    // storing X of player
    std::sprintf(buffer + 1, "%02d", mydata->stateFirst);
    std::sprintf(buffer + 4, "%02d", mydata->stateSecond);
    std::sprintf(buffer + 7, "%04d", mydata->myXcoord);
    std::sprintf(buffer + 12, "%04d", mydata->myYcoord);
    std::sprintf(buffer + 17, "%01d", mydata->myState);
    std::sprintf(buffer + 19, "%03d", mydata->health);
    std::sprintf(buffer + 23, "%03d", mydata->happiness);
    std::sprintf(buffer + 27, "%03d", mydata->money);
    buffer[31] = '#';
}
bool iWonTheGame=true;

bool fromNetwork(char *buffer, struct Info *indata)
{
    if (buffer[0] != '*' || buffer[31] != '#')
        return false;
    indata->stateFirst = atoi(buffer + 1);
    indata->stateSecond = atoi(buffer + 4);
    indata->myXcoord = atoi(buffer + 7);
    indata->myYcoord = atoi(buffer + 12);
    indata->myState = atoi(buffer + 17);
    indata->health = atoi(buffer + 19);
    indata->happiness = atoi(buffer + 23);
    indata->money = atoi(buffer + 27);
    return true;
}

void print_data(struct Info *indata)
{

    cout << indata->stateFirst << "\n";
    cout << indata->stateSecond << "\n";
    cout << indata->myXcoord << "\n";
    cout << indata->myYcoord << "\n";
    cout << indata->myState << "\n";
    cout << indata->health << "\n";
    cout << indata->happiness << "\n";
    cout << indata->money << "\n";
}
//  Client function end
/////////////////////////////

bool init()
{
    // Initialization flag
    bool success = true;

    gameState = 0;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        if (!gWindow.init())
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = gWindow.createRenderer();
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 16384) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }

                player1.Constructor(gRenderer, &gWindow.mWidth, &gWindow.mHeight, &message, 12, 4, 95, 159, 80, 48, "SpritePlayer1.png", 4, " ","EvilEmpire-4BBVK.ttf");
                player2.Constructor(gRenderer, &gWindow.mWidth, &gWindow.mHeight, &message, 9, 4, 64, 64, 64, 64, "player2.png", 4, " ","EvilEmpire-4BBVK.ttf");
                player1.mBox.x=1500;
                player1.mBox.y=100;
                
                player2.mBox.x = 1600;

                player2.mBox.y = 100;
                SDL_StartTextInput();
            }
        }
    }

    return success;
}

TTF_Font *textFont = NULL;

bool loadMedia(Tile *TilesLayer1[], Tile *TilesLayer2[], Tile *TilesLayer3[])
{
    // Loading success flag
    bool success = true;

    textFont = TTF_OpenFont("EvilEmpire-4BBVK.ttf", 35);
    if (textFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    BackgroundMusic = Mix_LoadMUS("Fluffing-a-Duck.wav");
    if (BackgroundMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // mySound = Mix_LoadWAV( "mixkit-quick-win-video-game-notification-269.wav" );
    // if( mySound == NULL )
    // {
    // 	printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    // 	return false;
    // }

    if (!player1.loadPlayer())
    {
        printf("Failed to load Pla yer !\n");
        success = false;
    }

    if (!player2.loadPlayer())
    {
        printf("Failed to load Pla yer !\n");
        success = false;
    }
    SDL_Color Black = {0, 0, 0, 0xFF};

    // Load tile texture
    if (!gTileTexture1.loadFromFile("campus_layer1&2.png", gRenderer))
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }
    if (!gTileTexture2.loadFromFile("TILESET_campus_layer3.png", gRenderer))
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    for (int i = 0; i < 2; i++)
    {
        if (!Textures[i].loadFromFile(Images[i], gRenderer))
        {
            printf("Faield to load Texture from Image %s", Images[i]);
            success = false;
        }
    }

    // Load tile map
    if (!setTiles(TilesLayer1, TilesLayer2, TilesLayer3))
    {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void close(Tile *TilesLayer1[], Tile *TilesLayer2[])
{
    // Deallocate tiles
    for (int i = 0; i < LAYER1_TOTAL_TILES; ++i)
    {
        if (TilesLayer1[i] != NULL)
        {
            delete TilesLayer1[i];
            TilesLayer1[i] = NULL;
        }
    }

    // Deallocate tiles
    for (int i = 0; i < LAYER2_TOTAL_TILES; ++i)
    {
        if (TilesLayer2[i] != NULL)
        {
            delete TilesLayer2[i];
            TilesLayer2[i] = NULL;
        }
    }

    // Disable text input
    SDL_StopTextInput();
    player1.close();
    player2.close();
    gTileTexture1.free();
    gTileTexture2.free();

    // Free loaded images
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    gWindow.free();

    // this is to close the background music
    Mix_FreeMusic(BackgroundMusic);
    // Mix_FreeChunk(mySound);

    BackgroundMusic = NULL;
    // mySound=NULL;

    gRenderer = NULL;

    for (int i = 0; i < 2; i++)
    {
        Textures[i].free();
    }
    // Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool setTiles(Tile *TilesLayer1[], Tile *TilesLayer2[], Tile *TilesLayer3[])
{
    // Success flag
    bool tilesLoaded = true;
    char trash;

    // The tile offsets
    int x = 0, y = 0;

    // Open the map
    std::ifstream map1("campus_layer1&2.txt");

    // If the map couldn't be loaded
    if (map1.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        // int *(int,int)
        // Initialize the tiles
        for (int i = 0; i < LAYER1_TOTAL_TILES; i++)
        {
            // Determines what kind of tile will be made
            int tileType = -1;

            // Read tile from map file
            map1 >> tileType;
            map1 >> trash;

            // If the was a problem in reading the map
            if (map1.fail())
            {
                // Stop loading map
                std::cout << "Error loading map: Unexpected end of file!\n " << i;
                tilesLoaded = false;
                break;
            }

            // If the number is a valid tile number
            if ((tileType >= 0)) // originally there was tileType< TotalTileSPrites
            {
                TilesLayer1[i] = new Tile(x, y, tileType);
            }
            // If we don't recognize the tile type
            else
            {
                // Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            // Move to next tile spot
            x += TILE_WIDTH;

            // If we've gone too far
            if (x >= LEVEL_WIDTH)
            {
                // Move back
                x = 0;

                // Move to the next row
                y += TILE_HEIGHT;
            }
        }
    }

    // Close the file
    map1.close();

    // The tile offsets
    x = 0;
    y = 0;

    // Open the map
    std::ifstream map2("TILEMAP_campus_layer3.txt");

    // If the map couldn't be loaded
    if (map2.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        // Initialize the tiles
        for (int i = 0; i < LAYER2_TOTAL_TILES; i++)
        {
            // Determines what kind of tile will be made
            int tileType = -1;

            // Read tile from map file
            map2 >> tileType;
            map2 >> trash;

            // If the was a problem in reading the map
            if (map2.fail())
            {
                // Stop loading map
                std::cout << "Error loading map: Unexpected end of file!\n " << i;
                tilesLoaded = false;
                break;
            }

            // If the number is a valid tile number
            if ((tileType >= 0)) // originally there was tileType< TotalTileSPrites
            {
                TilesLayer2[i] = new Tile(x, y, tileType);
            }
            // If we don't recognize the tile type
            else
            {
                // Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            // Move to next tile spot
            x += TILE_WIDTH;

            // If we've gone too far
            if (x >= LEVEL_WIDTH)
            {
                // Move back
                x = 0;

                // Move to the next row
                y += TILE_HEIGHT;
            }
        }
    }

    // Close the file
    map2.close();

    // The tile offsets
    x = 0;
    y = 0;

    // Open the map
    std::ifstream map3("campus_layer4final.txt");

    // If the map couldn't be loaded
    if (map3.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        // Initialize the tiles
        for (int i = 0; i < LAYER3_TOTAL_TILES; i++)
        {
            // Determines what kind of tile will be made
            int tileType = -1;

            // Read tile from map file
            map3 >> tileType;
            map3 >> trash;

            // If the was a problem in reading the map
            if (map3.fail())
            {
                // Stop loading map
                std::cout << "Error loading map: Unexpected end of file!\n " << i;
                tilesLoaded = false;
                break;
            }

            // If the number is a valid tile number
            if ((tileType >= 0)) // originally there was tileType< TotalTileSPrites
            {
                TilesLayer3[i] = new Tile(x, y, tileType);
            }
            // If we don't recognize the tile type
            else
            {
                // Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            // Move to next tile spot
            x += TILE_WIDTH;

            // If we've gone too far
            if (x >= LEVEL_WIDTH)
            {
                // Move back
                x = 0;

                // Move to the next row
                y += TILE_HEIGHT;
            }
        }
        map3.close();

        // If the map was loaded fine
        return tilesLoaded;
    }
}

void displayText(SDL_Renderer *gRenderer, std::string sentence, int WindowWidth, int WindowHeight, double textBoxX, double textBoxY, double textBoxWidth, double textBoxHeight, double sentenceX, double sentenceY)
{
    LTexture myTexture;
    SDL_Color textColor = {0, 0, 0};
    if (!myTexture.loadFromRenderedText(sentence, textColor, textFont, gRenderer))
    {
        printf("Error in loading texture for string %s", sentence);
    }

    SDL_Rect textbox = {(int)(((double)(WindowWidth)) * textBoxX), (int)(((double)(WindowHeight)) * textBoxY), (int)(((double)(WindowWidth)) * textBoxWidth), (int)(((double)(WindowHeight)) * textBoxHeight)};

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &textbox);

    // myTexture.render(gRenderer,WindowWidth/3 , WindowHeight*9/10, WindowWidth/3 , WindowHeight/11 );
    myTexture.render(gRenderer, (int)(((double)(WindowWidth)) * sentenceX), (int)(((double)(WindowHeight)) * sentenceY));
    myTexture.free();
}

int main(int argc, char *args[])
{
    int serv_fd, newserv_fd, bytes_sent, bytes_recvd;
    int port_no = PORT;
    char in_buffer[32], out_buffer[32], sname[64], cname[64];
    struct Info indata;
    struct Info mydata = {42, 5, 6, 7, 8, 9, 1, 4};
    bool validate_data;

    char cli_ip[INET_ADDRSTRLEN];
    // IP address of server
    char serv_ip[INET_ADDRSTRLEN] = "192.168.43.53";

    struct sockaddr_in serv_addr, cli_addr;

    // creating sever side socket
    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < -1)
    {
        perror("Server side listening Socket could not be created!");
        return 1;
    }

    int opt = 1;

    if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);

    // serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Convert IPv4 addresses from text to binary form
    if (inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    // binding socket
    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Failed to bind!");
        return 1;
    }

    if (listen(serv_fd, 5) == -1)
    {
        perror("Failed to listen!");
        return 1;
    }

    memset(&cli_addr, 0, sizeof(cli_addr));

    socklen_t cli_size = sizeof(cli_addr);

    if ((newserv_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &cli_size)) == -1)
    {
        perror("Failed to accept from client!");
        return 1;
    }
    //fcntl(newserv_fd, F_SETFL, fcntl(newserv_fd, F_GETFL, 0) | O_NONBLOCK);

    inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
    cout << "Server received connections from " << cli_ip << "\n";

    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // The level tiles
        Tile *tileset1[LAYER1_TOTAL_TILES];
        Tile *tileset2[LAYER2_TOTAL_TILES];
        Tile *tileset3[LAYER3_TOTAL_TILES];

        // Load media
        if (!loadMedia(tileset1, tileset2, tileset3))
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // Level camera
            SDL_Rect camera = {0, 0, gWindow.mWidth, gWindow.mHeight};

            // The frames per second timer
            bool processGoingOn = false;
            player1.setProcess(&processGoingOn);
            player2.setProcess(&processGoingOn);
            LTimer fpsTimer;

            // The frames per second cap timer
            LTimer capTimer;

            LTimer gameTimer;
            player1.setTimer(&gameTimer);

            bool bgplaying = false;
            bool renderText = true;

            LButton gameStartButton; 
			gameStartButton.InitialiseButton(1,&gameTimer,&gameState,0.4,0.35,0.2,0.3,"mixkit-quick-win-video-game-notification-269.wav",gRenderer,"start-up.png","start-down.png");

			LButton gameInfoButton;
			gameInfoButton.InitialiseButton(1,&gameTimer,&gameState,0.05,0.92,0.1,0.05,"mixkit-quick-win-video-game-notification-269.wav",gRenderer,"Status-up.png","Status-Down.png");
	 
			LButton gameResumeButton; 
			gameResumeButton.InitialiseButton(1,&gameTimer,&gameState,0.41,0.2,0.18,0.09,"mixkit-quick-win-video-game-notification-269.wav",gRenderer,"Resume-up.png","Resume-down.png");

			LButton gameVolumeOnButton; 
			gameVolumeOnButton.InitialiseButton(1,&gameTimer,&gameState,0.92,0.92,0.05,0.05,"mixkit-quick-win-video-game-notification-269.wav",gRenderer,"volume-on-up.png","volume-on-down.png");

			LButton gameVolumeDownButton; 
			gameVolumeDownButton.InitialiseButton(1,&gameTimer,&gameState,0.85,0.92,0.05,0.05,"mixkit-quick-win-video-game-notification-269.wav",gRenderer,"volume-off-up.png","volume-off-down.png");

            player1.updateScreen(&gWindow.mWidth, &gWindow.mHeight);
            player2.updateScreen(&gWindow.mWidth, &gWindow.mHeight);

            // In memory text stream
            std::stringstream timeText;

            // Start counting frames per second
            int countedFrames = 0;
            fpsTimer.start();

            // While application is running
            while (!quit)
            {
                // Start cap timer
                capTimer.start();
                // Handle events on queue
                
                if (gameState == 0)
                {
                    while (SDL_PollEvent(&e) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }
                        else if (e.type == SDL_KEYDOWN)
                        {
                            // Handle backspace
                            if (e.key.keysym.sym == SDLK_BACKSPACE && player1name.length() > 0)
                            {
                                // lop off character
                                player1name.pop_back();
                                renderText = true;
                            }
                            // Handle copy
                            else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                            {
                                SDL_SetClipboardText(player1name.c_str());
                            }
                            // Handle paste
                            else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                            {
                                player1name = SDL_GetClipboardText();
                                renderText = true;
                            }
                        }
                        else if (e.type == SDL_TEXTINPUT)
                        {
                            // Not copy or pasting
                            if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                            {
                                // Append character
                                player1name += e.text.text;
                                renderText = true;
                            }
                        }

                        // Handle input for the Player
                        gWindow.handleEvent( e );
					gameStartButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameInfoButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameResumeButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeOnButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeDownButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameStartButton.handleEvent(&e , 1);
                    }

                    if (!gWindow.isMinimized())
                    {

                        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);
                        if (gWindow.mWidth == 640 && gWindow.mHeight == 480)
                            Textures[0].render(gRenderer, 0, 0, gWindow.mWidth, gWindow.mHeight);
                        else
                            Textures[1].render(gRenderer, 0, 0, gWindow.mWidth, gWindow.mHeight);
                        gameStartButton.render();

                        // Rerender text if needed
                        if (renderText)
                        {
                            // Text is not empty
                            if (player1name != "")
                            {
                                // Render new text
                        displayText(gRenderer,player1name,gWindow.mWidth,gWindow.mHeight, 0.25,0.15,0.5,0.10,0.32,0.17);
                            }
                            // Text is empty
                            else
                            {
                                // Render space texture
                        displayText(gRenderer," ",gWindow.mWidth,gWindow.mHeight, 0.25,0.15,0.5,0.10,0.32,0.17);
                            }
                        }

                        SDL_RenderPresent(gRenderer);
                    }
                }

                // this is the condition when game has started
                else if (gameState == 1)
                {
                    if (!nameTaken)
                    {
                        strcpy(sname, player1name.c_str());
                        do
                        {
                            static int flag = 0;
                            memset(&cname, 0, sizeof(cname));
                            bytes_recvd = recv(newserv_fd, &cname, sizeof(cname), 0);
                            if (bytes_recvd == -1 && flag == 0)
                            {
                                memset(&cname, 0, sizeof(cname));
                                cout << "Could not ACQUIRE Player Information!"
                                     << "\n"
                                     << "Trying again..."
                                     << "\n";
                                continue;
                            }
                            else
                            {
                                flag = 1;
                                bytes_sent = send(newserv_fd, &sname, sizeof(sname), 0);
                                if (bytes_sent == -1)
                                    cout << "Could not SEND Player Data!"
                                         << "Trying Again..."
                                         << "\n";
                                else
                                    cout << cname << " has joined the game."
                                         << "\n";
                                player2name = cname;
                            }
                        } while (bytes_recvd == -1 || bytes_sent == -1);
                        nameTaken = true;
                        fcntl(newserv_fd, F_SETFL, fcntl(newserv_fd, F_GETFL, 0) | O_NONBLOCK);
                    }

                    player1.setName(player1name);
                    player2.setName(player2name);
                    if (!bgplaying)
                    {
                        Mix_PlayMusic(BackgroundMusic, -1);
                        bgplaying = true;
                        gameStartButton.close();
                    }
                    if(bgplaying && Mix_PausedMusic()==1){Mix_ResumeMusic();}
                                    


                    // Mix_PlayChannel( -1, mySound, 1 );

                    while (SDL_PollEvent(&e) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                            Mix_HaltMusic(); // not sure if this is required
                            Mix_HaltMusic();
                            Mix_Quit();
                        }

                        // Handle input for the Player
                        gWindow.handleEvent(e);
                        if (!gWindow.isMinimized() && !processGoingOn)
                        {
                            player1.handleEvent(e, tileset3);
                            if(player1.myPendingTasks.size()==0){gameState=3;}
                        }
                        gameStartButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameInfoButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameResumeButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeOnButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeDownButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);

                    int store = gameState;
					gameVolumeOnButton.handleEvent(&e,8);
					if(gameState==8) {gameState=1 ; if(Mix_PausedMusic()==1) Mix_ResumeMusic(); }
					gameVolumeDownButton.handleEvent(&e,8);
					if(gameState==8) {printf("sizmo");gameState=1 ; if(Mix_PausedMusic()!=1) Mix_PauseMusic(); }
					gameState=store;
					gameInfoButton.handleEvent(&e , 2); 
                    }

                    if (!gWindow.isMinimized())
                    {
                        // Move the dot
                        player1.updateParams(tileset2, tileset3, player2.mBox); // movement will revert only on base of tileset2
                        player1.setCamera(camera);

                        // Clear screen
                        SDL_SetRenderDrawColor(gRenderer, 0x1F, 0x88, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);

                        // Render level
                        for (int i = 0; i < LAYER1_TOTAL_TILES; ++i)
                        {
                            tileset1[i]->render(gRenderer, camera, &gTileTexture1);
                        }

                        // Render level
                        for (int i = 0; i < LAYER2_TOTAL_TILES; ++i)
                        {
                            tileset2[i]->render(gRenderer, camera, &gTileTexture2);
                        }

                        // Render dot

                        player1.render( camera  );
				player2.renderOtherPlayer(camera);
				displayText(gRenderer,message,gWindow.mWidth,gWindow.mHeight,0.00,0.90,1.00,0.10,0.33,0.923) ; 
				gameVolumeDownButton.render();
				gameVolumeOnButton.render();
				gameInfoButton.render();
				//Update screen
				SDL_RenderPresent( gRenderer );
                    }
                }

                else if (gameState == 2)
                {
                    if(Mix_PausedMusic()!=1){Mix_PauseMusic();}

                    while (SDL_PollEvent(&e) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }

                        // Handle input for the Player
                        gWindow.handleEvent( e );
					gameStartButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameInfoButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameResumeButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeOnButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					gameVolumeDownButton.UpdateParameters(gWindow.mWidth,gWindow.mHeight);
					
					gameResumeButton.handleEvent(&e , 1);
                    }

                    if (!gWindow.isMinimized())
                    {
                        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                        SDL_RenderClear(gRenderer);
                        if (gWindow.mWidth == 640 && gWindow.mHeight == 480)
                            Textures[0].render(gRenderer, 0, 0, gWindow.mWidth, gWindow.mHeight);
                        else
                            Textures[1].render(gRenderer, 0, 0, gWindow.mWidth, gWindow.mHeight);
                        gameResumeButton.render();
                        player1.showPauseStateChart();
                        SDL_RenderPresent(gRenderer);
                    }

                } // Pause music , show tasks pending , can go to state 1 by pressing resume button
                else if (gameState == 3)
                {
                    Mix_HaltMusic();
                    if(!iWonTheGame){
                        string displayText = player2name+string(" Won the Game \n.Better Luck Next Time.") ; 

                    player1.displayMyText(displayText,500,300);
                    SDL_RenderPresent(gRenderer);}
                    else{
                         string displayText = string(" Hurray , you won the game") ; 
                        player1.displayMyText(displayText,500,300);
                        SDL_RenderPresent(gRenderer);
                    }

                } // Game over , display winner.

                ++countedFrames;

                if ((gameState != 0) && (countedFrames % 3 < 10))
                {
                    // receiving
                    bytes_recvd = recv(newserv_fd, &in_buffer, sizeof(in_buffer), 0);
                    if (bytes_recvd == -1);
                        // cout << "Frame data not received!"
                        //      << "\n";

                    else if (bytes_recvd != 32)
                        cout << "complete data not received, what is going on!!!\n";

                    else
                    {
                        validate_data = fromNetwork(in_buffer, &indata);
                        if (!validate_data)
                            cout << "Wrong data received\n";

                        else
                        {
                            player2.myState.first = indata.stateFirst;
                            player2.myState.second = indata.stateSecond;
                            player2.mBox.x = indata.myXcoord;
                            player2.mBox.y = indata.myYcoord;
                            player2gameState = indata.myState;
                            player2.myHealth = indata.health;
                            player2.myHappiness = indata.happiness;
                            player2.myMoney = indata.money;
                        }
                    }


                    // sending
                    mydata = {player1.getMyStateFirst(), player1.getMyStateSecond(), player1.mBox.x, player1.mBox.y, gameState, (int)player1.myHealth, (int)player1.myHappiness, (int)player1.myMoney};

                    toNetwork(out_buffer, &mydata);
                    bytes_sent = send(newserv_fd, &out_buffer, sizeof(out_buffer), 0);
                    if (bytes_sent == -1)
                        cout << "Frame data not sent" << "\n"; 
                    else if (bytes_sent != 32)
                        cout << "complete data not sent, what is going on???????\n";
                }

                    if(player2gameState==3){gameState=3;iWonTheGame=false;}

                    // If frame finished early
                    int frameTicks = capTimer.getTicks();
                    if (frameTicks < SCREEN_TICK_PER_FRAME)
                    {
                        // Wait remaining time
                        SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
                    }
                    //printf("Width is %d , and height is %d \n", gWindow.mWidth, gWindow.mHeight);
            }

            // Free resources and close SDL
        }
            close(tileset1, tileset2);
    }
        close(serv_fd);
        close(newserv_fd);
        return 0;
}