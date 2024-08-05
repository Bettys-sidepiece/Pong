#include "Game.h"
#include <iostream>

Game* Game::g_game = nullptr;

Game::Game() 
    : m_window(nullptr), m_renderer(nullptr), m_running(false),
      m_windowWidth(1000), m_windowHeight(1000), 
      m_gameAreaWidth(m_windowWidth - UI_AREA_WIDTH),
      m_player1(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, m_windowHeight - 225, RECT_WIDTH, RECT_HEIGHT, 1),
      m_player2(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, 50, RECT_WIDTH, RECT_HEIGHT, -1),
      m_ball(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, (m_windowHeight - 200)-10, 10),
      m_ballAttachedToPlayer1(true), m_ballAttachedToPlayer2(false),
      m_dT(0.0f), m_lT(0), m_gui(nullptr, 0, 0, nullptr)
{
    m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
    e_gamestate = GameState::MENU;
    e_gamemode = GameMode::NONE;
    g_game = this;
}

Game::~Game() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    if (m_font) TTF_CloseFont(m_font);
    if (m_gui.getTitleFont()) TTF_CloseFont(m_gui.getTitleFont());

    g_game = nullptr;
    TTF_Quit();
    SDL_Quit();
}

void Game::startGame() {
    if (e_gamemode != GameMode::NONE) {
        e_gamestate = GameState::RUNNING;
        m_gui.setGameState(static_cast<int>(e_gamestate));
        m_player1.m_score = 0;
        m_player2.m_score = 0;
    }
}

void Game::pauseGame() {
    if (e_gamestate == GameState::RUNNING) {
        e_gamestate = GameState::PAUSED;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::resumeGame() {
    if (e_gamestate == GameState::PAUSED) {
        e_gamestate = GameState::RUNNING;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::gameSettings() {
    if (e_gamemode == GameMode::NONE) {
        e_gamestate = GameState::SETTINGS;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::setMusic(){
    if(e_gamestate == GameState::SETTINGS) {
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }

}

void Game::setDifficulty(){
    if(e_gamestate == GameState::SETTINGS) {
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }

}

void Game::backToMenu(){
    if(e_gamestate == GameState::SETTINGS || 
        e_gamestate == GameState::GAMESEL || 
        e_gamestate == GameState::RUNNING) 
        {
        e_gamestate = GameState::MENU;
        m_gui.setGameState(static_cast<int>(e_gamestate));
        }
}

void Game::playMode() {
    if (e_gamestate == GameState::MENU) {
        e_gamestate = GameState::GAMESEL;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::toggleSinglePlayer(){
    if (e_gamestate == GameState::GAMESEL) {
        e_gamemode = GameMode::SINGLE_PLAYER;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::toggleMultiPlayer(){
    if (e_gamestate == GameState::GAMESEL) {
        e_gamemode = GameMode::MULTI_PLAYER;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: SDL Initialization " << SDL_GetError() << "\n";
        return false;
    }

    if (TTF_Init() != 0) {
        std::cout << "Error: SDL_ttf Initialization " << TTF_GetError() << "\n";
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

    m_font = TTF_OpenFont("Assets/PixelCaps.ttf", 35);
    if (!m_font) {
        std::cout << "Error: SDL_ttf Font Loading " << TTF_GetError() << "\n";
        return false;
    }

    TTF_Font* titlefont = TTF_OpenFont("Assets/Pixelmania.ttf", 55);
    if (!titlefont) {
        std::cout << "Error: SDL_ttf Font Loading " << TTF_GetError() << "\n";
    }
    
    TTF_Font* subtitlefont = TTF_OpenFont("Assets/PixelCaps.ttf", 40);
    if (!titlefont) {
        std::cout << "Error: SDL_ttf Font Loading " << TTF_GetError() << "\n";
    }
    
    m_gui = UI(m_renderer, UI_AREA_WIDTH, m_windowHeight, m_font);
    m_gui.setTitleFont(titlefont);
    m_gui.setSubTitleFont(subtitlefont);
    m_gui.setGameState(e_gamestate);

    m_running = true;
    m_gui.initButtons(exitCallback, startGameCallback, pauseGameCallback, 
                      resumeCallback, gameSettingsCallback, playModeCallback,
                      returnToMenu, changeDifficulty, toggleMusic, selectSinglePlayer,
                      selectMultiplayer);
    
    return true;
}

void Game::run() {
    while (m_running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
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
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            if (m_ballAttachedToPlayer1 || m_ballAttachedToPlayer2) {
                m_ball.launch();
                m_ballAttachedToPlayer1 = false;
                m_ballAttachedToPlayer2 = false;
            }
        }

        if (e_gamestate == GameState::RUNNING) {
            m_player1.handleEvent(event);
            m_player2.handleEvent(event);
        }

        m_gui.handleEvent(event);
    }
}

void Game::update() {
    int score = 0;
    
    switch (e_gamestate) {
        case GameState::MENU:
            // Update menu
            break;
        case GameState::RUNNING:
            m_player1.update(m_gameAreaWidth, UI_AREA_WIDTH);
            m_player2.update(m_gameAreaWidth, UI_AREA_WIDTH);

            if (m_ballAttachedToPlayer1) {
                m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score);
            } else if (m_ballAttachedToPlayer2) {
                m_ball.attachTo(&m_player2.p_getRect(), m_player2.m_id);
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score);
            } else {
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score);
                m_ball.collisionDetection(m_player1.getRect(), m_player2.getRect());
            }

            if (score > 0) {
                m_player1.m_score++;
                m_ball.attachTo(&m_player2.p_getRect(), m_player2.m_id);
                m_ballAttachedToPlayer2 = true;
            } else if (score < 0) {
                m_player2.m_score++;
                m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
                m_ballAttachedToPlayer1 = true;
            }
            break;

        case GameState::PAUSED:
            // Do nothing or update pause menu
            break;
        case GameState::SETTINGS:
            // Update settings menu
            break;
        case GameState::GAMESEL:
            // Update game selection menu
            break;
    }
    m_gui.update();
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    SDL_Rect gameArea = {UI_AREA_WIDTH, 0, m_gameAreaWidth, m_windowHeight};
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &gameArea);

    switch (e_gamestate) {
        case GameState::MENU:
            // Render menu
            break;
        case GameState::RUNNING:
            m_player1.render(m_renderer);
            m_player2.render(m_renderer);
            m_ball.render(m_renderer);
            break;

        case GameState::PAUSED:
            // Render paused game and pause menu
            break;
        case GameState::SETTINGS:
            // Render settings menu
            break;
        case GameState::GAMESEL:
            // Render game selection menu
            break;
    } 
    m_gui.render(static_cast<int>(e_gamestate));
    SDL_RenderPresent(m_renderer);
}

void Game::exitCallback() { if (g_game) g_game->m_running = false;}
void Game::startGameCallback() { if (g_game) g_game->startGame();}
void Game::pauseGameCallback() { if (g_game) g_game->pauseGame();}
void Game::resumeCallback() { if (g_game) g_game->resumeGame();}
void Game::gameSettingsCallback() { if (g_game) g_game->gameSettings();}
void Game::playModeCallback() { if (g_game) g_game->playMode();}
void Game::toggleMusic(){if(g_game) g_game->setMusic();}
void Game::changeDifficulty(){if(g_game) g_game->setDifficulty();}
void Game::returnToMenu(){if(g_game) g_game->backToMenu();}
void Game::selectSinglePlayer(){if (g_game) g_game->toggleSinglePlayer();}
void Game::selectMultiplayer(){if(g_game) g_game->toggleMultiPlayer();}