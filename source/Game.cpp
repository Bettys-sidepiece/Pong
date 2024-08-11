#include "Game.h"
#include <iostream>

Game* Game::g_game = nullptr;

Game::Game() 
    : m_window(nullptr), m_renderer(nullptr), m_running(false),
      m_windowWidth(1000), m_windowHeight(1000), 
      m_gameAreaWidth(m_windowWidth - UI_AREA_WIDTH),
      m_player1(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH), m_windowHeight - 225, RECT_WIDTH, RECT_HEIGHT, 1),
      m_player2(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH), 50, RECT_WIDTH, RECT_HEIGHT, -1),
      m_ball(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH), (m_windowHeight - 200)-10, 10),
      m_ballAttachedToPlayer1(true), m_ballAttachedToPlayer2(false),
      m_dT(0.0f), m_lT(0), m_gui(nullptr, 0, 0, nullptr),
      m_AI(Difficulty::NORMAL),
      m_musicActive(true),m_currentDifficulty(Difficulty::NORMAL),
      m_clickSound(nullptr), m_hoverSound(nullptr),m_paddleHitSound(nullptr)
{
    m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
    e_gamestate = GameState::MENU;
    e_gamemode = GameMode::SINGLE_PLAYER;
    g_game = this;
}

Game::~Game() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    if (m_font) TTF_CloseFont(m_font);
    if (m_gui.getTitleFont()) TTF_CloseFont(m_gui.getTitleFont());

    if (m_clickSound) Mix_FreeChunk(m_clickSound);
    if (m_hoverSound) Mix_FreeChunk(m_hoverSound);
    if (m_paddleHitSound) Mix_FreeChunk(m_paddleHitSound);

    Mix_Quit();

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
        resetGameState();
    }
}

void Game::pauseGame() {
    if (e_gamestate == GameState::RUNNING) {
        prevgamestate = GameState::RUNNING;
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
    if (e_gamestate == GameState::MENU || e_gamestate == GameState::PAUSED) {
        e_gamestate = GameState::SETTINGS;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }
}

void Game::setMusic(){
    if(e_gamestate == GameState::SETTINGS) {
        m_gui.setGameState(static_cast<int>(e_gamestate));
        if(m_musicActive == true){
            disableAudio();
        }else{
            enableAudio();
        }
    }
}

void Game::setDifficulty(){
    if(e_gamestate == GameState::SETTINGS) {
        e_gamestate = GameState::DIFFICULTYSEL;
        prevgamestate = GameState::SETTINGS;
        m_gui.setGameState(static_cast<int>(e_gamestate));
    }

}

void Game::backToMenu(){
    if(e_gamestate == GameState::SETTINGS || e_gamestate == GameState::GAMESEL || 
        e_gamestate == GameState::PAUSED) 
        {   
            if(prevgamestate == GameState::PAUSED && prevgamestate == GameState::SETTINGS){
                e_gamestate = GameState::PAUSED;
                m_gui.setGameState(static_cast<int>(e_gamestate));
                resetGameState();

            }else if(prevgamestate == GameState::RUNNING){
                e_gamestate = GameState::MENU;
                m_gui.setGameState(static_cast<int>(e_gamestate));
            }else{
                e_gamestate = GameState::MENU;
                m_gui.setGameState(static_cast<int>(e_gamestate));
            }
        }
}

void Game::playMode() {
    if (e_gamestate == GameState::MENU) {
        prevgamestate = GameState::MENU;
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
        //e_gamemode = GameMode::MULTI_PLAYER;
        //m_gui.setGameState(static_cast<int>(e_gamestate));
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
    
    // Get the primary display
    SDL_DisplayMode DM;
    if (SDL_GetCurrentDisplayMode(0, &DM) != 0) {
        std::cout << "Error: SDL_GetCurrentDisplayMode " << SDL_GetError() << "\n";
        return false;
    }
    // Set the window size to the display size
    m_windowWidth = DM.w;
    m_windowHeight = DM.h;
    m_gameAreaWidth = m_windowWidth - UI_AREA_WIDTH;

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

    m_font = TTF_OpenFont("Assets/PixelCaps.ttf", 25);
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
        return false;
    }

    m_clickSound = Mix_LoadWAV("Assets/clicksound3.wav");
    m_hoverSound = Mix_LoadWAV("Assets/clicksound1.wav");
    m_paddleHitSound = Mix_LoadWAV("Assets/Ping.wav");

    if (!m_clickSound || !m_hoverSound || !m_paddleHitSound) {
        std::cout << "Failed to load sound effects! SDL_mixer Error: " << Mix_GetError() << "\n";
        return false;
    }
    
    m_gui = UI(m_renderer, UI_AREA_WIDTH, m_windowHeight, m_font);
    m_gui.setTitleFont(titlefont);
    m_gui.setSubTitleFont(subtitlefont);
    m_gui.setGameState(e_gamestate);

    m_gui.setScreenWidth(m_gameAreaWidth);
    m_gui.setScreenHeight(m_windowHeight);

    m_running = true;
    m_gui.initButtons(exitCallback, startGameCallback, pauseGameCallback, 
                      resumeCallback, gameSettingsCallback, playModeCallback,
                      returnToMenu, changeDifficulty, toggleMusic, selectSinglePlayer,
                      selectMultiplayer,m_clickSound,m_hoverSound);
    
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    while (m_running) {
        Uint32 currentTime = SDL_GetTicks();
        m_dT = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;

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
            if (m_ballAttachedToPlayer1) {
                m_ball.launch();
                m_ballAttachedToPlayer1 = false;
                m_ballAttachedToPlayer2 = false;
            }
        }

        if (e_gamestate == GameState::RUNNING) {
            m_player1.handleEvent(event);
            if (m_ballAttachedToPlayer2 && m_ball.getBallRect().y > m_player2.getRect().y) {
                m_ball.launch();
                m_ballAttachedToPlayer1 = false;
                m_ballAttachedToPlayer2 = false;
            }
        }

        if (e_gamestate == GameState::DIFFICULTYSEL) {
            if (event.type == SDL_KEYDOWN) {
                if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        selectDifficulty(-1);
                        if (m_clickSound) {
                            Mix_PlayChannel(-1, m_clickSound, 0);
                        }
                        break;
                    
                    case SDLK_RIGHT:
                        selectDifficulty(1);
                        if (m_clickSound) {
                            Mix_PlayChannel(-1, m_clickSound, 0);
                        }
                        break;

                    case SDLK_RETURN:
                        e_gamestate = GameState::SETTINGS;
                        prevgamestate = GameState::DIFFICULTYSEL;
                        m_gui.setGameState(static_cast<int>(e_gamestate));
                        if (m_clickSound) {
                            Mix_PlayChannel(-1, m_clickSound, 0);
                        }
                        break;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int centerX = UI_AREA_WIDTH + m_gameAreaWidth / 2;
                int radius = 150;
                int dialWidth = radius * 2;
                
                if (mouseX < centerX - radius / 2) {
                    m_currentDifficulty = Difficulty::HARD;
                } else if (mouseX < centerX + radius / 2) {
                    m_currentDifficulty = Difficulty::EASY;
                } else {
                    m_currentDifficulty = Difficulty::NORMAL;
                }
                m_AI.setDifficulty(m_currentDifficulty);
            }
            }
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
            if (e_gamemode == GameMode::SINGLE_PLAYER) {
                m_AI.update(m_ball, m_player2, m_dT, m_gameAreaWidth, UI_AREA_WIDTH);
            }   
            m_player1.update(m_gameAreaWidth, UI_AREA_WIDTH);
            m_player2.update(m_gameAreaWidth, UI_AREA_WIDTH);

            if (m_ballAttachedToPlayer1) {
                m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score, m_paddleHitSound);
            } else if (m_ballAttachedToPlayer2) {
                m_ball.attachTo(&m_player2.p_getRect(), m_player2.m_id);
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score,m_paddleHitSound);
            } else {
                m_ball.update(m_gameAreaWidth, UI_AREA_WIDTH, score,m_paddleHitSound);
                m_ball.collisionDetection(m_player1.getRect(), m_player2.getRect(),m_paddleHitSound);
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

            m_gui.updateScore(m_player1.m_score, m_player2.m_score);

            break;

        case GameState::PAUSED:{
            // Do nothing or update pause menu
            break;
            }
        case GameState::SETTINGS:{
            // Update settings menu
            break;
            }
        case GameState::GAMESEL:{
            // Update game selection menu
            break;
            }
        case GameState::DIFFICULTYSEL:{
            break;
            }
        case GameState::TOGGLEMUSIC:{
            break;
        }
    }
    m_gui.update();
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    SDL_Rect gameArea = {UI_AREA_WIDTH, 0, m_gameAreaWidth, m_windowHeight};
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &gameArea);

    SDL_Rect pauseArea = {UI_AREA_WIDTH,0,m_gameAreaWidth,m_windowHeight};
    SDL_Rect leftGuard = {(395), 0, 10, m_windowHeight};
    SDL_Rect rightGuard = {(m_windowWidth-5), 0, 25, m_windowHeight};
    SDL_Rect centerLine = {(UI_AREA_WIDTH+5),(m_windowHeight/2), (m_gameAreaWidth-10),25};

    SDL_Rect outline_sp = {370,310,20,20};
    SDL_Rect outline_mp = {370,375,20,20};

    SDL_Rect outline_audio = {330, 312, 20, 20};

    m_gui.render(static_cast<int>(e_gamestate));

    switch (e_gamestate) {
        case GameState::MENU:{
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer,200,200,200,150);
            SDL_RenderFillRect(m_renderer, &leftGuard);
            
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,150);
            SDL_RenderFillRect(m_renderer, &rightGuard);
            
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,150);
            SDL_RenderFillRect(m_renderer, &centerLine);

            m_player1.render(m_renderer);
            m_player2.render(m_renderer);
            // Render menu
            break;
        }
        case GameState::RUNNING:{
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer,200,200,200,175);
            SDL_RenderFillRect(m_renderer, &leftGuard);
            
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,175);
            SDL_RenderFillRect(m_renderer, &rightGuard);

            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,175);
            SDL_RenderFillRect(m_renderer, &centerLine);

            m_player1.render(m_renderer);
            m_player2.render(m_renderer);
            m_ball.render(m_renderer);
            break;
    }
        case GameState::PAUSED:{
            // Render paused game and pause menu
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer,200,200,200,175);
            SDL_RenderFillRect(m_renderer, &leftGuard);
            
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,175);
            SDL_RenderFillRect(m_renderer, &rightGuard);

            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 200,200,200,175);
            SDL_RenderFillRect(m_renderer, &centerLine);

            m_player1.render(m_renderer);
            m_player2.render(m_renderer);
            m_ball.render(m_renderer);

            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(m_renderer, 125, 125, 125, 50);
            SDL_RenderFillRect(m_renderer, &pauseArea);
            break;
        }
        case GameState::SETTINGS:{
            // Render settings menu
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
            if(m_musicActive == true){
                SDL_RenderFillRect(m_renderer, &outline_audio);
            }else{
                SDL_RenderDrawRect(m_renderer, &outline_audio);
            }
            break;
        }
        case GameState::GAMESEL:{
            // Render game selection menu
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

            if(e_gamemode == GameMode::MULTI_PLAYER){
                SDL_RenderFillRect(m_renderer, &outline_mp);
                SDL_RenderDrawRect(m_renderer, &outline_sp);
            }
            else{
                SDL_RenderFillRect(m_renderer, &outline_sp);
                SDL_RenderDrawRect(m_renderer, &outline_mp);
            }
            break;
        }
        case GameState::DIFFICULTYSEL:{           
            std::cout << "Rendering difficulty selection screen" << std::endl;

            int centerX = UI_AREA_WIDTH / 2;
            int centerY = m_windowHeight / 2;

            // Draw the selected difficulty
            SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
            std::string difficultyText;
            switch (m_currentDifficulty) {
                case Difficulty::EASY:
                    difficultyText = "EASY";
                    break;
                case Difficulty::NORMAL:
                    difficultyText = "NORMAL";
                    break;
                case Difficulty::HARD:
                    difficultyText = "HARD";
                    break;
            }
            m_gui.drawText(true, difficultyText, centerX, centerY - 30, m_font);

            // Draw navigation instructions
            SDL_SetRenderDrawColor(m_renderer, 150, 150, 150, 255);

            m_gui.drawText(true, "Difficulty", centerX, 200,m_font);
            m_gui.drawText(true, "< LEFT | RIGHT >", centerX, centerY + 70, m_font);
            m_gui.drawText(true, "ENTER to confirm", centerX, centerY + 140, m_font);

            std::cout << "Current difficulty: " << static_cast<int>(m_currentDifficulty) << std::endl;
            std::cout << "Finished rendering difficulty selection screen" << std::endl;
            break;
        }
        case GameState::TOGGLEMUSIC:{
            break;
        }
    } 
    
    SDL_RenderPresent(m_renderer);
}

void Game::setAIDifficulty(Difficulty difficulty) {
    m_AI.setDifficulty(difficulty);
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

void Game::resetGameState() {
    // Reset player 1 position
    m_player1.setPosition(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, m_windowHeight - 100
    
    );
    // Reset player 2 position
    m_player2.setPosition(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, 50);
    // Reset ball position
    m_ball.setPosition(UI_AREA_WIDTH + (m_gameAreaWidth - RECT_WIDTH) / 2, (m_windowHeight - 200) - 10);
    // Attach ball to player 1
    m_ball.attachTo(&m_player1.p_getRect(), m_player1.m_id);
    m_ballAttachedToPlayer1 = true;
    m_ballAttachedToPlayer2 = false;

    // Reset scores
    m_player1.m_score = 0;
    m_player2.m_score = 0;

    // Update the GUI with reset scores
    m_gui.updateScore(m_player1.m_score, m_player2.m_score);
}

void Game::enableAudio() {
    if (!m_musicActive) {
        m_clickSound->volume = 128;
        m_hoverSound->volume = 128;
        m_paddleHitSound->volume = 128;
        m_musicActive = true;
    }
}

void Game::disableAudio() {
    if (m_musicActive) {
        m_clickSound->volume = 0;
        m_hoverSound->volume = 0;
        m_paddleHitSound->volume = 0;
        m_musicActive = false;
    }
}

void Game::selectDifficulty(int direction) {
    int difficultyLevel = static_cast<int>(m_currentDifficulty);
    difficultyLevel = (difficultyLevel + direction + 3) % 3;
    m_currentDifficulty = static_cast<Difficulty>(difficultyLevel);
    m_AI.setDifficulty(m_currentDifficulty);
    std::cout << "Difficulty changed to: ";
    switch (m_currentDifficulty) {
        case Difficulty::HARD: std::cout << "Hard"; break;
        case Difficulty::EASY: std::cout << "Easy"; break;
        case Difficulty::NORMAL: std::cout << "Normal"; break;
    }
    std::cout << "\n";
}