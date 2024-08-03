#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
const int RECT_WIDTH = 100;
const int RECT_HEIGHT = 20;
int rect_x = (WINDOW_WIDTH) / 2; // Initial x position of the rectangle

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

void handleEvent(SDL_Event &event, bool &running, int &rect_x);
bool createRenderer(SDL_Renderer*& render, SDL_Window* win);
bool createRuntime(SDL_Window*& win);
void runGameLoop(SDL_Event &evnt, SDL_Renderer* render, bool &running);
void endGameLoop(SDL_Renderer* render, SDL_Window* win);

int main() {

    bool running = true;
    
    // Initialize SDL, Create a resizable SDL window
    if(!createRuntime(window)) {
        return 1;
    }

    // Create a renderer
    if(!createRenderer(renderer, window)) {
        return 1;
    }

    // Run the event loop
    runGameLoop(event, renderer, running);

    // Clean up
    endGameLoop(renderer, window);
   
    return 0;
}

void handleEvent(SDL_Event &event, bool &running, int &rect_x) 
{
    if (event.type == SDL_QUIT) {
        running = false;
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a) {
            rect_x -= (event.motion.x - 10); // Move left
        } else if (event.key.keysym.sym == SDLK_d) {
            rect_x += (event.motion.x + 10); // Move right
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        rect_x = event.motion.x - RECT_WIDTH / 2; // Center the rectangle on the mouse x position
    }

    // Clamp rectangle within window bounds
    if (rect_x < 0) {
        rect_x = 0;
    } else if (rect_x > WINDOW_WIDTH - RECT_WIDTH) {
        rect_x = WINDOW_WIDTH - RECT_WIDTH;
    }
}

bool createRuntime(SDL_Window*& win)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: SDL Initialization " << SDL_GetError() << "\n";
        return false;
    }

    // Create a resizable SDL window
    win = SDL_CreateWindow("PONG-2024",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           WINDOW_WIDTH, WINDOW_HEIGHT,
                           SDL_WINDOW_RESIZABLE);

    if (!win) {
        std::cout << "Error: SDL Window Creation " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    return true;
}

bool createRenderer(SDL_Renderer*& render, SDL_Window* win)
{
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!render) {
        std::cout << "Error: SDL Renderer Creation " << SDL_GetError() << "\n";
        SDL_DestroyWindow(win);
        SDL_Quit();
        return false;
    }

    return true;
}

void runGameLoop(SDL_Event& evnt, SDL_Renderer* render, bool &running)
{
    while (running) {
        while (SDL_PollEvent(&evnt)) {
            handleEvent(evnt, running, rect_x);
        }

        // Clear the screen
        SDL_SetRenderDrawColor(render, 100, 100, 100, 255); // Gray
        SDL_RenderClear(render);

        // Draw the filled rectangle
        SDL_Rect rect = {rect_x, (WINDOW_HEIGHT-RECT_HEIGHT)/2, RECT_WIDTH, RECT_HEIGHT};
        SDL_SetRenderDrawColor(render, 0, 195, 0, 255); // Red
        SDL_RenderFillRect(render, &rect);

        // Present the renderer
        SDL_RenderPresent(render);

        // Delay to control frame rate
        SDL_Delay(16); // Approximately 60 frames per second
    }
}

void endGameLoop(SDL_Renderer* render, SDL_Window* win)
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
