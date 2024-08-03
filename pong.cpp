#include <SDL2/SDL.h>
#include <iostream>


const int UI_AREA_WIDTH = 400;
const int RECT_WIDTH = 100;
const int RECT_HEIGHT = 20;

int windowWidth = 1000;
int windowHeight = 1000;
int gameAreaWidth = windowWidth - UI_AREA_WIDTH;
int p_rect_x = UI_AREA_WIDTH + (gameAreaWidth - RECT_WIDTH) / 2;// Initial x position of the rectangle 1
int p_rect_y = UI_AREA_WIDTH + (windowWidth - 100);
int s_rect_x = UI_AREA_WIDTH + (gameAreaWidth - RECT_WIDTH) / 2;
int s_rect_y = 100;


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

void handleEvent(SDL_Event &event, bool &running, int &rect_x);
bool createRenderer(SDL_Renderer*& render, SDL_Window* win);
bool createRuntime(SDL_Window*& win);
void runGameLoop(SDL_Event &evnt, SDL_Renderer* render, bool &running);
void endGameLoop(SDL_Renderer* render, SDL_Window* win);

enum GameDiff{
    EASY = 0, NORMAL, HARD
};

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
            rect_x -= (event.motion.x - RECT_WIDTH /2); // Move left
        } else if (event.key.keysym.sym == SDLK_d) {
            rect_x += (event.motion.x + RECT_WIDTH /2); // Move right
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        rect_x = event.motion.x - RECT_WIDTH / 2; // Adjust x position within game area
    }

    // Clamp rectangle within window bounds
    if (rect_x < UI_AREA_WIDTH) {
        rect_x = UI_AREA_WIDTH;
    } else if (rect_x > (gameAreaWidth + UI_AREA_WIDTH) - RECT_WIDTH) {
        rect_x = (gameAreaWidth + UI_AREA_WIDTH) - RECT_WIDTH;
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
    win = SDL_CreateWindow("PONG",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           windowWidth,windowHeight,
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
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                windowWidth = event.window.data1;
                windowHeight = event.window.data2;
                gameAreaWidth = windowWidth - UI_AREA_WIDTH;
                p_rect_x = UI_AREA_WIDTH + (gameAreaWidth - RECT_WIDTH) / 2;
                s_rect_x = UI_AREA_WIDTH + (gameAreaWidth - RECT_WIDTH) / 2;
                p_rect_y = (windowHeight - 100);
                s_rect_y = 100;
            }
            handleEvent(evnt, running, p_rect_x);
        }

        // Clear the screen
        SDL_SetRenderDrawColor(render, 25, 25, 25, 255); // Gray
        SDL_RenderClear(render);

        // Draw the game area background
        SDL_Rect gameArea = {UI_AREA_WIDTH, 0, gameAreaWidth, windowHeight}; // Game area starting after UI area
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black
        SDL_RenderFillRect(render, &gameArea);

        // Draw the filled rectangle
        SDL_Rect rect = {p_rect_x, p_rect_y, RECT_WIDTH, RECT_HEIGHT};
        SDL_SetRenderDrawColor(render, 100, 100, 100, 255); // Grey
        SDL_RenderFillRect(render, &rect);

        // Draw the filled rectangle
        SDL_Rect rect2 = {s_rect_x, s_rect_y, RECT_WIDTH, RECT_HEIGHT};
        SDL_SetRenderDrawColor(render, 150, 150, 150, 255); // Grey
        SDL_RenderFillRect(render, &rect2);

        // Draw UI areas (optional)
        SDL_Rect leftUIArea = {0,0, UI_AREA_WIDTH, windowHeight};
        SDL_SetRenderDrawColor(render, 15, 15, 15, 255); // Light gray
        SDL_RenderFillRect(render, &leftUIArea);

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
