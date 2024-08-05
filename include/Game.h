#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "GameModifiers.h"
#include "Ball.h"
#include "PongUI.h"

enum GameState{
        MENU = 0,
        SETTINGS,
        PAUSED,
        RUNNING,
        GAMESEL
    };

enum class GameMode{
    NONE,
    SINGLE_PLAYER = 1,
    MULTI_PLAYER,
};

class Game {
public:
    static Game* g_game;

    Game();
    ~Game();
    bool initialize();
    void run();
    void startGame();
    void pauseGame();
    void resumeGame();
    void gameSettings();
    void playMode();
    void setMusic();
    void setDifficulty();
    void backToMenu();
    void toggleSinglePlayer();
    void toggleMultiPlayer();

    static void exitCallback();
    static void startGameCallback();
    static void pauseGameCallback();
    static void resumeCallback();
    static void gameSettingsCallback();
    static void playModeCallback();
    // Function prototypes for additional callbacks
    static void toggleMusic();
    static void changeDifficulty();
    static void returnToMenu();
    static void selectSinglePlayer();
    static void selectMultiplayer();

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

    GameState e_gamestate;
    GameMode e_gamemode;

};

extern Game* g_game;

#endif // GAME_H
