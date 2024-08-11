#ifndef PONGUI_H
#define PONGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include <functional>

using CallbackFunction = std::function<void()>;

class TextButton {
private:
    Mix_Chunk* m_clickSound;
    Mix_Chunk* m_hoverSound;
    SDL_Rect m_rect;
    std::string m_text;
    SDL_Color m_color;
    SDL_Color m_hoverColor;
    SDL_Color m_currentColor;
    TTF_Font* m_font;
    bool m_isHovered;

    CallbackFunction m_onClick; // Callback for click event

    void updateColor();

public:
    TextButton(int x, int y, const std::string& text, TTF_Font* font, SDL_Color color, SDL_Color hoverColor, CallbackFunction callback, Mix_Chunk* clickSound, Mix_Chunk* hoverSound);
    ~TextButton();

    void render(SDL_Renderer* renderer) const;
    void handleEvent(const SDL_Event& event);
    bool isClicked(const SDL_Event& event) const;
    SDL_Rect getRect() const { return m_rect; }
    std::string getText() const { return m_text; }
};

enum class GuiState {
    MAIN_GUI = 0,
    SETTINGS_GUI,
    GAME_PAUSED,
    GAME_RUNNING,
    GAMESEL
};

class UI {
public:
    void setExitCallback(CallbackFunction callback) { m_exitCallback = callback; }
    void setStartGameCallback(CallbackFunction callback) { m_startGameCallback = callback; }
    void setPauseCallback(CallbackFunction callback) { m_pauseCallback = callback; }
    void setSettingsCallback(CallbackFunction callback) { m_settingsCallback = callback; }
    void setPlayModeCallback(CallbackFunction callback) { m_playModeCallback = callback; }
    void setResumeCallback(CallbackFunction callback) { m_resumeCallback = callback; }
    
    void initButtons(CallbackFunction exitCallback, CallbackFunction startCallback, 
                    CallbackFunction pauseCallback, CallbackFunction resumeCallback,
                    CallbackFunction settingsCallback, CallbackFunction playModeCallback, 
                    CallbackFunction returnToMenuCallback, CallbackFunction changeDifficultyCallback, 
                    CallbackFunction toggleMusicCallback, CallbackFunction selectSinglePlayerCallback, 
                    CallbackFunction selectMultiplayerCallback,
                    Mix_Chunk* clickSound,Mix_Chunk* hoverSound);

    UI(SDL_Renderer* renderer, int uiAreaWidth, int windowHeight, TTF_Font* font);
    ~UI();

    void render(int state);
    void updateScore(int player1Score, int player2Score);
    void update(); 
    void setTitleFont(TTF_Font* font) { m_titlefont = font; }
    TTF_Font* getTitleFont() const { return m_titlefont; }
    void setSubTitleFont(TTF_Font* font) { m_subtitlefont = font; }
    TTF_Font* getSubTitleFont() const { return m_subtitlefont; }
    void handleEvent(SDL_Event& event);

    void setGameState(int state){ m_gamestate = state;}
    int getGameState() const { return m_gamestate; }

    int getScreenWidth() const { return m_screenwidth; };
    void setScreenWidth(int width) { m_screenwidth = width;};

    int getScreenHeight() const { return m_screenheight; };
    void setScreenHeight(int height) { m_screenheight = height;};

private:
    CallbackFunction m_exitCallback;
    CallbackFunction m_startGameCallback;
    CallbackFunction m_pauseCallback;
    CallbackFunction m_resumeCallback;
    CallbackFunction m_settingsCallback;
    CallbackFunction m_playModeCallback;
    CallbackFunction m_returnToMenuCallback;
    CallbackFunction m_changeDifficultyCallback;
    CallbackFunction m_toggleMusicCallback;
    CallbackFunction m_selectSinglePlayerCallback;
    CallbackFunction m_selectMultiplayerCallback;
    
    SDL_Renderer* m_renderer; // Renderer to draw the UI elements
    int m_uiAreaWidth; // Width of the UI area
    int m_windowHeight; // Height of the window

    int m_player1Score; // Player 1 score
    int m_player2Score; // Player 2 score

    int m_screenwidth; // Width of the screen
    int m_screenheight;

    TTF_Font* m_font;
    TTF_Font* m_titlefont;
    TTF_Font* m_subtitlefont;

    std::vector<TextButton> m_buttons[5];
    int m_gamestate;

    void drawText(bool header, const std::string& text, int x, int y, TTF_Font* font);
    void drawUIBackground();
};

#endif // PONGUI_H
