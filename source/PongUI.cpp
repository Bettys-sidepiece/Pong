#include "PongUI.h"
#include <iostream>

TextButton::TextButton(int x, int y, const std::string& text, TTF_Font* font, SDL_Color color, SDL_Color hoverColor)
    : m_rect{x, y, 200, 50}, m_text(text), m_color(color), m_hoverColor(hoverColor), m_currentColor(color), m_font(font), m_isHovered(false) {
}

TextButton::~TextButton() {
    // No need to clean up font or other resources here
}

void TextButton::updateColor() {
    m_currentColor = m_isHovered ? m_hoverColor : m_color;
}

void TextButton::render(SDL_Renderer* renderer) const {
    SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_currentColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &m_rect);
    SDL_DestroyTexture(texture);
}

void TextButton::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;
        m_isHovered = (x >= m_rect.x && x <= m_rect.x + m_rect.w && y >= m_rect.y && y <= m_rect.y + m_rect.h);
        updateColor();
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (isClicked(event)) {
            // Handle click action
        }
    }
}

bool TextButton::isClicked(const SDL_Event& event) const {
    return (event.button.x >= m_rect.x && event.button.x <= m_rect.x + m_rect.w &&
            event.button.y >= m_rect.y && event.button.y <= m_rect.y + m_rect.h);
}

UI::UI(SDL_Renderer* renderer, int uiAreaWidth, int windowHeight, TTF_Font* font)
    : m_renderer(renderer), m_uiAreaWidth(uiAreaWidth), m_windowHeight(windowHeight),
      m_player1Score(0), m_player2Score(0) {
    
    m_font = font;
    SDL_Color defaultColor = {175, 175, 175, 255}; // White color
    SDL_Color hoverColor = {0, 0, 200, 255};  // Light blue color

    // Create buttons with sample texts
    m_buttons.emplace_back(50, 300, "Start Game", font, defaultColor, hoverColor); // 0
    m_buttons.emplace_back(50, 330, "Multiplayer", font, defaultColor, hoverColor); // 1
    m_buttons.emplace_back(50, 360, "Singleplayer", font, defaultColor, hoverColor);// 2
    m_buttons.emplace_back(50, 390, "Settings", font, defaultColor, hoverColor);  // 3
    m_buttons.emplace_back(50,420, "Quit", font, defaultColor, hoverColor); // 4
}

UI::~UI() {
    // Destructor where you might need to free resources if necessary
}


void UI::render(int state) {
    if (state < 0 || state >= static_cast<int>(GUISTATE::GAME_RUNNING)) {
        std::cerr << "Invalid state in UI::update: " << state << std::endl;
        return;
    }
    drawUIBackground();
    switch (state)
    {
    case static_cast<int>(GUISTATE::MAIN_GUI):
        drawText("PONG", 100, 80, m_titlefont);
        for(const TextButton& button: m_buttons){
        drawText(button.getText(), button.getRect().x, button.getRect().y, m_font);
        }
        break;
    case static_cast<int>(GUISTATE::SETTINGS_GUI):
        break;

    case static_cast<int>(GUISTATE::GAME_PAUSED):
        break;

    case static_cast<int>(GUISTATE::GAME_RUNNING):    
    default:
        break;
    }
}

void UI::updateScore(int player1Score, int player2Score) {
    m_player1Score = player1Score;
    m_player2Score = player2Score;
}

void UI::handleEvent(SDL_Event& event) {
    for (auto& button : m_buttons) {
        button.handleEvent(event);
        if (button.isClicked(event)) {
            // Handle button click
        }
    }
}

void UI::drawText(const std::string& text, int x, int y, TTF_Font* font) {
    if (!font || !m_renderer) {
        std::cerr << "Font or renderer is null in drawText" << std::endl;
        return;
    }

    SDL_Color color = {175, 175, 175, 255}; // White color

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
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

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void UI::drawUIBackground() {
    if (!m_renderer) {
        std::cerr << "Font or renderer is null in drawText" << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(m_renderer, 15, 15, 15, 255); // Black background
    SDL_Rect uiRect = {0, 0, m_uiAreaWidth, m_windowHeight};
    SDL_RenderFillRect(m_renderer, &uiRect);
}

void UI::update(){
    
}

void UI::initMainGui(){

}
void UI::initSettingsGui(){

}

void UI::initGamePausedGui(){
    
}

void UI::initGameRunningGui(){

}