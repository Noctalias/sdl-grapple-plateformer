#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cmath>

//-----------------------HEADERS-------------------------------

#include "grapple.h"
#include "player.h"
#include "plateform.h"


/*TODO: 
- Vector class:
private : x y
public: operator+, operator-, operator*, operator/

Image class,

Player class

Try to make headers */

//-----------------------CONSTANTS-------------------------------
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

const unsigned int SIZE_BLOCK = 32;
const int MAP_SIZE_X = 16;
const int MAP_SIZE_Y = 16;
const float GROUND_DISTANCE = 0.1;

const unsigned int CHARACTER_SIZE = 16;
const float PLAYER_MOVEMENT_SPEED = 0.3;
const char CONTROLS[] = "";

//-----------------------VARIABLES-------------------------------

class Vector2D
{
private:
    double x, y;

public:
    // Constructor to initialize x and y
    Vector2D(double x, double y) : x(x), y(y) {}

    // Method to add another vector to this vector
    Vector2D add(const Vector2D &other) const
    {
        return Vector2D(x + other.x, y + other.y);
    }

};

 class Player
{ 
    public: // Initializing player's position, velocity, force and initialazing jumping to false
    Player(float posStartX, float posStartY, float velStartX, float velStartY) : position(posStartX, posStartY), velocity(velStartX, velStartY){};

    private:
    Vector2D position, velocity, force;
    bool jumping; // Check if player is jumping/in the air.


};

//-----------------------FUNCTIONS-------------------------------

int main(int argc, char *argv[])
{ // Main SDL2 syntax

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Creating SDL window
    SDL_Window *window = SDL_CreateWindow("SDLGAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    //Creating SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Event loop
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                // The user clicked the close button
                quit = true;
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
            }
        }

        // Your game logic, update and rendering code goes here

        // Clearing renderer
        SDL_RenderClear(renderer);

        // Updating screen
        SDL_RenderPresent(renderer);
    }

    //Cleaning up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Compilate command
// g++ -Wall -o sdlgame main.cpp -lSDL2

// lunch game
//  ./sdlgame

// Ctrl + Shift + I to indent