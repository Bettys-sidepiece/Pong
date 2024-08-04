#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:

    int m_score;

    const int m_id;

    Player(int x, int y, int width, int height, int id);
    void handleEvent(const SDL_Event& event);
    void update(int gameAreaWidth, int uiAreaWidth);
    void render(SDL_Renderer* renderer) const;
    void setSpeed(float speed){m_speed = speed;};
    int getSpeed() const{return m_speed;};
    void setSize(float size);  
    float getSize() const {return m_size;};
    SDL_Rect getRect() const {return m_rect;};

    SDL_Rect& p_getRect() {return m_rect;};
    
    int getVelocityX() const { return m_velocityX; }



private:
    SDL_Rect m_rect;
    int m_velocityX;
    int m_speed;
    float m_size;
    int m_playerId;
};

#endif // PLAYER_H