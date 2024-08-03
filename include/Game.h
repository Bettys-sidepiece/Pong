#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Player.h"

class Game {
public:
    /**
     * @brief Default constructor for the Game class.
     *
     * Initializes the game window, renderer, running flag, window dimensions, game area width,
     * UI area width, rectangle dimensions, and player objects.
     *
     * @param m_windowWidth The width of the game window.
     * @param m_windowHeight The height of the game window.
     * @param m_gameAreaWidth The width of the game area.
     * @param UI_AREA_WIDTH The width of the UI area.
     * @param RECT_WIDTH The width of the rectangle.
     * @param RECT_HEIGHT The height of the rectangle.
     * @param m_player1 The first player object.
     * @param m_player2 The second player object.
     */
    Game();
        /**
     * @brief Destructor for the Game class.
     *
     * Destroys the SDL window, renderer, and quits SDL.
     *
     * @return void
     */
    ~Game();
    /**
     * @brief Initializes the game window, renderer, and other essential components.
     *
     * This function sets up the SDL window, renderer, running flag, window dimensions, game area width,
     * UI area width, rectangle dimensions, and player objects. It also initializes the SDL library.
     *
     * @return bool Returns true if the initialization is successful, false otherwise.
     */
    bool initialize();
    /**
     * @brief Runs the main game loop.
     *
     * This function continuously handles events, updates the game state, and renders the game.
     * It calls the handleEvents(), update(), and render() functions in a loop until the game is stopped.
     *
     * @return void
     */
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
    static constexpr int UI_AREA_WIDTH = 400;
    static constexpr int RECT_WIDTH = 100;
    static constexpr int RECT_HEIGHT = 20;
    
    Player m_player1;
    Player m_player2;
};

#endif // GAME_H