#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

class Ball {
private:
    SDL_Rect m_rect;
    int m_radius;
    float m_speedX;
    float m_speedY;

public:
    Ball(int x, int y, int radius, float speedX, float speedY);

    void update(int gameAreaWidth, int &score);
    void render(SDL_Renderer* renderer) const;
    void collisionDetection(const SDL_Rect &playerRect, const SDL_Rect &player2Rect);
    void setSpeed(float speedX, float speedY);
    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const;
};

#endif //BALL_H
