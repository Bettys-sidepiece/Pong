#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Player.h"
#include "GameModifiers.h"
#include "Ball.h"
#include "PongUI.h"
#include "ComputerAI.h"

enum GameState{
        MENU = 0,
        SETTINGS,
        PAUSED,
        RUNNING,
        GAMESEL,
        DIFFICULTYSEL,
        TOGGLEMUSIC,
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

    void resetGameState();
    void enableAudio();
    void disableAudio();
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
    void setAIDifficulty(Difficulty difficulty);
    void selectDifficulty(int direction);

private:
    void handleEvents();
    void update();
    void render();
    Difficulty m_currentDifficulty;

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
    bool m_musicActive;

    UI m_gui;
    
    GameModifierManager m_modManager;
    float m_dT;
    Uint32 m_lT;

    GameState e_gamestate;
    GameState prevgamestate;
    GameMode e_gamemode;

    ComputerAI m_AI;

    Mix_Chunk* m_clickSound;
    Mix_Chunk* m_hoverSound;
    Mix_Chunk* m_paddleHitSound;

};

extern Game* g_game;

#endif // GAME_H
