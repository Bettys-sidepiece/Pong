#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Player.h"

class Game {
public:
    Game();
    ~Game();
    bool initialize();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_running;
    int m_windowWidth;
    int m_windowHeight;
    int m_gameAreaWidth;
    const int UI_AREA_WIDTH;
    const int RECT_WIDTH;
    const int RECT_HEIGHT;

    Player m_player1;
    Player m_player2;
};

#endif // GAME_H