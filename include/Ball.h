#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Ball {
private:
    SDL_Rect m_rect;
    int m_radius;
    float m_speedX;
    float m_speedY;
    int m_ballIsAttached;
    SDL_Rect * m_attachedTo;

public:
    Ball(int x, int y, int radius);

    void update(int gameAreaWidth, int uiAreaWidth, int &score,  Mix_Chunk* paddleHitSound);
    void render(SDL_Renderer* renderer) const;
    void collisionDetection(const SDL_Rect &playerRect, const SDL_Rect &player2Rect, Mix_Chunk* paddleHitSound);
    void setSpeed(float speedX, float speedY);
    void attachTo(SDL_Rect* playerRect, int isPlayer);
    void launch();

    SDL_Rect getBallRect() const{return m_rect;};
    float getSpeedX() const{return m_speedX;};
    float getSpeedY() const{return m_speedY;};
    bool isAttached() const;
    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const;
    void setPosition(int x, int y){m_rect.x = x- m_radius;m_rect.y = y - m_radius;};
};

#endif //BALL_H
