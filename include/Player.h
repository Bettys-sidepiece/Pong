#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    Player(int x, int y, int width, int height);
    void handleEvent(const SDL_Event& event);
    void update(int gameAreaWidth, int uiAreaWidth);
    void render(SDL_Renderer* renderer);
    void setSpeed(float speed);
    float getSpeed() const{return m_speed;};
    void setSize(float size);  
    float getSize() const {return m_size;};
    SDL_Rect getRect() const {return m_rect;};


private:
    SDL_Rect m_rect;
    int m_speed;
    float m_size;
};

#endif // PLAYER_H