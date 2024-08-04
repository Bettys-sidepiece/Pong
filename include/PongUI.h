#ifndef PONGUI_H
#define PONGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class TextButton
{
private:
    SDL_Rect m_rect;
    std::string m_text;
    SDL_Color m_color;
    SDL_Color m_hoverColor;
    SDL_Color m_currentColor;
    TTF_Font* m_font;
    bool m_isHovered;

    void updateColor();

public:
    TextButton(int x, int y, const std::string& text, TTF_Font* font, SDL_Color color, SDL_Color hoverColor);
    ~TextButton();

    void render(SDL_Renderer* renderer) const;
    void handleEvent(const SDL_Event& event);
    bool isClicked(const SDL_Event& event) const;
    SDL_Rect getRect() const { return m_rect; };
    std::string getText() const { return m_text; };

};

class UI {
public:
    enum class GUISTATE{
        MAIN_GUI = 0,
        SETTINGS_GUI,
        GAME_PAUSED,
        GAME_RUNNING
    };

    UI(SDL_Renderer* renderer, int uiAreaWidth, int windowHeight, TTF_Font* font);
    ~UI();

    void render(int state);
    void updateScore(int player1Score, int player2Score);
    void update(); 
    void setTitleFont(TTF_Font* font){m_titlefont = font;};
    TTF_Font* getTitleFont() const {return m_titlefont;};
    void handleEvent(SDL_Event& event);
    void initMainGui();
    void initSettingsGui();

    void initGamePausedGui();

    void initGameRunningGui();

private:
    SDL_Renderer* m_renderer; // Renderer to draw the UI elements
    int m_uiAreaWidth; // Width of the UI area
    int m_windowHeight; // Height of the window

    int m_player1Score; // Player 1 score
    int m_player2Score; // Player 2 score

    TTF_Font* m_font;
    TTF_Font* m_titlefont;
    std::vector<TextButton> m_buttons;

    // Function to draw text (you'll need SDL_ttf library for this)
    void drawText(const std::string& text, int x, int y, TTF_Font* font);
    
    // Function to draw the UI background
    void drawUIBackground();
};

#endif // PONGUI_H
