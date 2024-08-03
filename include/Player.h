#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    Player(int x, int y, int width, int height);
    void handleEvent(const SDL_Event& event);
    void update(int gameAreaWidth, int uiAreaWidth);
    void render(SDL_Renderer* renderer) const;

private:
    SDL_Rect m_rect;
    int m_speed;
};

#endif // PLAYER_H