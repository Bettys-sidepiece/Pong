#include "PongUI.h"
#include <iostream>

// Constructor
TextButton::TextButton(int x, int y, const std::string& text, TTF_Font* font, SDL_Color color, SDL_Color hoverColor, CallbackFunction callback, Mix_Chunk* clickSound, Mix_Chunk* hoverSound)
    : m_rect{x, y, 300, 50}, m_text(text), m_color(color), m_hoverColor(hoverColor), m_currentColor(color), m_font(font), m_isHovered(false), m_onClick(callback), m_clickSound(clickSound), m_hoverSound(hoverSound){
}

// Destructor
TextButton::~TextButton() {
    // No need to clean up font or other resources here
}

// Update the button color based on hover state
void TextButton::updateColor() {
    m_currentColor = m_isHovered ? m_hoverColor : m_color;
}

// Render the button and its text
void TextButton::render(SDL_Renderer* renderer) const {
    // Render the button background
    SDL_SetRenderDrawColor(renderer, m_currentColor.r, m_currentColor.g, m_currentColor.b, m_currentColor.a);

    // Render the text
    SDL_Surface* surface = TTF_RenderText_Blended(m_font, m_text.c_str(), m_currentColor);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Create a destination rectangle that matches the text size
    SDL_Rect destRect = {m_rect.x, m_rect.y, surface->w, surface->h};

    // Center the text within the button rectangle
    destRect.x = m_rect.x + (m_rect.w - surface->w) / 2;
    destRect.y = m_rect.y + (m_rect.h - surface->h) / 2;

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Handle events related to the button
void TextButton::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        bool wasHovered = m_isHovered;
        int x = event.motion.x;
        int y = event.motion.y;
        m_isHovered = (x >= m_rect.x && x <= m_rect.x + m_rect.w && y >= m_rect.y && y <= m_rect.y + m_rect.h);
        if (m_isHovered && !wasHovered && m_hoverSound) {
            Mix_PlayChannel(-1, m_hoverSound, 0);
        }
        updateColor();
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (isClicked(event) && m_onClick != nullptr) {
            if (m_clickSound) {
                Mix_PlayChannel(-1, m_clickSound, 0);
            }
            m_onClick();
        }
    }
}

// Check if the button is clicked
bool TextButton::isClicked(const SDL_Event& event) const {
    return (event.button.x >= m_rect.x && event.button.x <= m_rect.x + m_rect.w &&
            event.button.y >= m_rect.y && event.button.y <= m_rect.y + m_rect.h);
}

// Constructor
UI::UI(SDL_Renderer* renderer, int uiAreaWidth, int windowHeight, TTF_Font* font)
    : m_renderer(renderer), m_uiAreaWidth(uiAreaWidth), m_windowHeight(windowHeight),
      m_player1Score(0), m_player2Score(0), m_font(font) {
    
}

// Destructor
UI::~UI() {
    // Destructor where you might need to free resources if necessary
}

// Render the UI based on the state
void UI::render(int state) {
    if (state < 0 || state > static_cast<int>(GuiState::TOGGLEMUSIC)) {
        std::cerr << "Invalid state in UI::render: " << state << std::endl;
        return;
    }
    drawUIBackground();
    switch (state) {
        case static_cast<int>(GuiState::MAIN_GUI):
            drawText(true,"PONG", 25, 80, m_titlefont);
            for (const TextButton& button : m_buttons[state]) {
                button.render(m_renderer);
            }
            break;
        case static_cast<int>(GuiState::SETTINGS_GUI):
            drawText(true,"Settings", 25, 80, m_subtitlefont);
            for (const TextButton& button : m_buttons[state]) {
                button.render(m_renderer);
            }
            break;
        case static_cast<int>(GuiState::GAME_PAUSED):
            drawText(false, std::to_string(m_player1Score), 450, (m_screenheight/2)+90, m_subtitlefont);
            drawText(false, std::to_string(m_player2Score), 450, (m_screenheight/2)-150, m_subtitlefont);
            drawText(true,"Paused", 25, 80, m_subtitlefont);
            for (const TextButton& button : m_buttons[state]) {
                button.render(m_renderer);
            }
            break;
        case static_cast<int>(GuiState::GAME_RUNNING):
            drawText(false, std::to_string(m_player1Score), 450, (m_screenheight/2)+90, m_subtitlefont);
            drawText(false, std::to_string(m_player2Score), 450, (m_screenheight/2)-150, m_subtitlefont);
            for (const TextButton& button : m_buttons[state]) {
                button.render(m_renderer);
            }
            break;
        case static_cast<int>(GuiState::GAMESEL):
            drawText(true,"Game Mode", 25, 80, m_subtitlefont);
            for (const TextButton& button : m_buttons[state]) {
                button.render(m_renderer);
            }
            break;
        default:
            break;
    }
}

// Update the scores
void UI::updateScore(int player1Score, int player2Score) {
    m_player1Score = player1Score;
    m_player2Score = player2Score;
}

// Handle UI events
void UI::handleEvent(SDL_Event& event) {
    if (m_gamestate >= static_cast<int>(GuiState::MAIN_GUI) && m_gamestate <= static_cast<int>(GuiState::GAMESEL)){
        for (auto& button : m_buttons[m_gamestate]) {
            button.handleEvent(event);
        }
    }
}

// Draw text on the UI
void UI::drawText(bool header ,const std::string& text, int x, int y, TTF_Font* font) {
    if (!font || !m_renderer) {
        std::cerr << "Font or renderer is null in drawText" << std::endl;
        return;
    }

    SDL_Color color = {175, 175, 175, 255}; // White color

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Calculate the x position to center the text
    if(header) x = (m_uiAreaWidth - surface->w) / 2;

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Draw the UI background
void UI::drawUIBackground() {
    if (!m_renderer) {
        std::cerr << "Renderer is null in drawUIBackground" << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(m_renderer, 15, 15, 15, 255); // Black background
    SDL_Rect uiRect = {0, 0, m_uiAreaWidth, m_windowHeight};
    SDL_RenderFillRect(m_renderer, &uiRect);
}

// Update UI state
void UI::update() {
    // Placeholder for future updates
}

// Initialize buttons with their respective callbacks
void UI::initButtons(CallbackFunction exitCallback, CallbackFunction startCallback, 
                    CallbackFunction pauseCallback, CallbackFunction resumeCallback,
                     CallbackFunction settingsCallback, CallbackFunction playModeCallback, 
                     CallbackFunction returnToMenuCallback, CallbackFunction changeDifficultyCallback, 
                     CallbackFunction toggleMusicCallback, CallbackFunction selectSinglePlayerCallback, 
                     CallbackFunction selectMultiplayerCallback,
                     Mix_Chunk* clickSound,Mix_Chunk* hoverSound){
    // Set callbacks for buttons
    m_exitCallback = exitCallback;
    m_startGameCallback = startCallback;
    m_pauseCallback = pauseCallback;
    m_resumeCallback = resumeCallback;
    m_settingsCallback = settingsCallback;
    m_playModeCallback = playModeCallback;
    m_returnToMenuCallback = returnToMenuCallback;
    m_changeDifficultyCallback = changeDifficultyCallback;
    m_toggleMusicCallback = toggleMusicCallback;
    m_selectSinglePlayerCallback = selectSinglePlayerCallback;
    m_selectMultiplayerCallback = selectMultiplayerCallback;
    
    SDL_Color defaultColor = {125, 125, 125, 255}; // White color
    SDL_Color hoverColor = {200, 200, 200, 255};  // Light blue color

    // Example button initializations (with their own render logic)
    for( auto&button : m_buttons) 
    {
        button.clear();
    }

    // Menu Buttons
    m_buttons[0].emplace_back(50, 300, "Start Game", m_font, defaultColor, hoverColor, m_startGameCallback, clickSound, hoverSound);
    m_buttons[0].emplace_back(50, 360, "Play Mode", m_font, defaultColor, hoverColor, m_playModeCallback, clickSound, hoverSound);
    m_buttons[0].emplace_back(50, 420, "Settings", m_font, defaultColor, hoverColor, m_settingsCallback, clickSound, hoverSound);
    m_buttons[0].emplace_back(50, 480, "Exit Game", m_font, defaultColor, hoverColor, m_exitCallback, clickSound, hoverSound);

    // Settings Buttons
    m_buttons[1].emplace_back(50, 300, "Audio", m_font, defaultColor, hoverColor, m_toggleMusicCallback, clickSound, hoverSound);
    m_buttons[1].emplace_back(50, 360, "Difficulty", m_font, defaultColor, hoverColor, m_changeDifficultyCallback, clickSound, hoverSound);
    m_buttons[1].emplace_back(50, 420, "Back", m_font, defaultColor, hoverColor, m_returnToMenuCallback, clickSound, hoverSound);

    // Gameplay Paused
    m_buttons[2].emplace_back(50, 360, "Resume", m_font, defaultColor, hoverColor, m_resumeCallback, clickSound, hoverSound);
    m_buttons[2].emplace_back(50, 420, "Quit to Menu", m_font, defaultColor, hoverColor, m_returnToMenuCallback, clickSound, hoverSound);

    // Gameplay Active
    m_buttons[3].emplace_back(50, 300, "Pause", m_font, defaultColor, hoverColor, m_pauseCallback, clickSound, hoverSound);

    // Game Mode Selection Buttons
    m_buttons[4].emplace_back(50, 300, "Single Player", m_font, defaultColor, hoverColor, m_selectSinglePlayerCallback, clickSound, hoverSound);
    m_buttons[4].emplace_back(50, 360, "Multiplayer", m_font, defaultColor, hoverColor, m_selectMultiplayerCallback, clickSound, hoverSound);
    m_buttons[4].emplace_back(50, 420, "Back", m_font, defaultColor, hoverColor, m_returnToMenuCallback, clickSound, hoverSound);

}
