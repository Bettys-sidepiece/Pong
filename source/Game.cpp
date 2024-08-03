#include "Game.h"
#include <iostream>

Game::Game() : m_window(nullptr), m_renderer(nullptr), m_running(false),
               m_windowWidth(1000), 
               m_windowHeight(1000), 
               m_gameAreaWidth(m_windowWidth - UI_AREA_WIDTH),
               m_player1(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, m_windowHeight - 100, RECT_WIDTH, RECT_HEIGHT),
               m_player2(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, 100, RECT_WIDTH, RECT_HEIGHT) {}

Game::~Game() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: SDL Initialization " << SDL_GetError() << "\n";
        return false;
    }

    m_window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_windowWidth, m_windowHeight, SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        std::cout << "Error: SDL Window Creation " << SDL_GetError() << "\n";
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cout << "Error: SDL Renderer Creation " << SDL_GetError() << "\n";
        return false;
    }

    m_running = true;
    return true;
}

void Game::run() {
    while (m_running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // Approximately 60 frames per second
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_running = false;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            m_windowWidth = event.window.data1;
            m_windowHeight = event.window.data2;
            m_gameAreaWidth = m_windowWidth - UI_AREA_WIDTH;
        }
        m_player1.handleEvent(event);
    }
}

/**
 * @brief Updates the game state.
 *
 * This function is responsible for updating the game state by calling the update method of the player objects.
 * The update method for each player is responsible for handling player movement and any other game-related logic.
 *
 * @return void
 */
void Game::update() {
    m_player1.update(m_gameAreaWidth, UI_AREA_WIDTH);
    m_player2.update(m_gameAreaWidth, UI_AREA_WIDTH);
}


/**
 * @brief Renders the game to the screen.
 *
 * This function is responsible for drawing the game to the screen. It clears the screen, draws the game area,
 * player objects, and the left UI area, and then presents the rendered image to the screen.
 *
 * @return void
 */void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    SDL_Rect gameArea = {UI_AREA_WIDTH, 0, m_gameAreaWidth, m_windowHeight};
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &gameArea);

    m_player1.render(m_renderer);
    m_player2.render(m_renderer);

    SDL_Rect leftUIArea = {0, 0, UI_AREA_WIDTH, m_windowHeight};
    SDL_SetRenderDrawColor(m_renderer, 15, 15, 15, 255);
    SDL_RenderFillRect(m_renderer, &leftUIArea);

    SDL_RenderPresent(m_renderer);
}