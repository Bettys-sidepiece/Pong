#include "Game.h"
#include <iostream>

Game::Game() : m_window(nullptr), m_renderer(nullptr), m_running(false),
               m_windowWidth(1000), 
               m_windowHeight(1000), 
               m_gameAreaWidth(m_windowWidth - UI_AREA_WIDTH),
               m_player1(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, m_windowHeight - 225, RECT_WIDTH, RECT_HEIGHT, 1),
               m_player2(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, 50, RECT_WIDTH, RECT_HEIGHT, -1),
               m_ball(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, (m_windowHeight - 200)-10, 10),//<<< changed speed from 5 to 0
               m_ballAttachedToPlayer1(true), m_ballAttachedToPlayer2(false),
               m_dT(0.0f), m_lT(0),
               m_gui(nullptr, 0, 0, nullptr)

            {
                m_ball.attachTo(&m_player1.p_getRect(),m_player1.m_id);
                m_gamestate = 0;
            }

Game::~Game() {
    if(m_renderer) SDL_DestroyRenderer(m_renderer);
    if(m_window) SDL_DestroyWindow(m_window);
    if (m_font) {
        TTF_CloseFont(m_font);
    }
    if(m_gui.getTitleFont()){
        TTF_CloseFont(m_gui.getTitleFont());
    }
    TTF_Quit();
    SDL_Quit();
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

    m_font = TTF_OpenFont("Roboto/Roboto-Medium.ttf", 25); // Load a font
    if (!m_font) {
        std::cout << "Error: SDL_ttf Font Loading " << TTF_GetError() << "\n";
        return false;
    }

    TTF_Font* titlefont = TTF_OpenFont("Roboto/Roboto-Bold.ttf", 60); // Load a font
    if (!titlefont) {
        std::cout << "Error: SDL_ttf Font Loading " << TTF_GetError() << "\n";
    }
    
    m_gui = UI(m_renderer, UI_AREA_WIDTH, m_windowHeight, m_font); // Initialize UI
    m_gui.setTitleFont(titlefont);
    
    m_running = true;
    m_lT = SDL_GetTicks();
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

        m_player1.handleEvent(event);
        m_player2.handleEvent(event);
        m_gui.handleEvent(event);
    }
}

void Game::update() {
    int score = 0;
    m_player1.update(m_gameAreaWidth, UI_AREA_WIDTH);
    m_player2.update(m_gameAreaWidth, UI_AREA_WIDTH);

    if(m_ballAttachedToPlayer1){
        m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
        m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH,score);
    }else if(m_ballAttachedToPlayer2){
        m_ball.attachTo(&m_player2.p_getRect(),m_player2.m_id);
        m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH,score);
    }else{
        m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH,score);
        m_ball.collisionDetection(m_player1.getRect(), m_player2.getRect());
    }
    if(score > 0){
        m_player1.m_score++;
        m_ball.attachTo(&m_player2.p_getRect(),m_player2.m_id);
        m_ballAttachedToPlayer2 = true;
    }else if(score < 0){
        m_player2.m_score++;
        m_ball.attachTo(&m_player1.p_getRect(),m_player1.m_id);
        m_ballAttachedToPlayer1 = true;
    }
    m_gui.update();
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    SDL_Rect gameArea = {UI_AREA_WIDTH, 0, m_gameAreaWidth, m_windowHeight};
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &gameArea);

    m_player1.render(m_renderer);
    m_player2.render(m_renderer);
    m_ball.render(m_renderer);

    //m_modManager.render(m_renderer);
    m_gui.render(m_gamestate);

    SDL_RenderPresent(m_renderer);
}
