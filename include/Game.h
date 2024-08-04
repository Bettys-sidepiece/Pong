#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "GameModifiers.h"
#include "Ball.h"
#include "PongUI.h"

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
    TTF_Font* m_font;

    bool m_running;
    int m_windowWidth;
    int m_windowHeight;
    int m_gameAreaWidth;
    static constexpr int UI_AREA_WIDTH = 400;
    static constexpr int RECT_WIDTH = 100;
    static constexpr int RECT_HEIGHT = 20;
    
    Player m_player1;
    Player m_player2;
    Ball m_ball;
    bool m_ballAttachedToPlayer1;
    bool m_ballAttachedToPlayer2;

    UI m_gui;
    
    GameModifierManager m_modManager;
    float m_dT;
    Uint32 m_lT;

    int m_gamestate;


};

#endif // GAME_H
