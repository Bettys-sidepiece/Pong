#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

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

    void update(int gameAreaWidth, int uiAreaWidth, int &score);
    void render(SDL_Renderer* renderer) const;
    void collisionDetection(const SDL_Rect &playerRect, const SDL_Rect &player2Rect);
    void setSpeed(float speedX, float speedY);
    void attachTo(SDL_Rect* playerRect, int isPlayer);
    void launch();

    bool isAttached() const;

    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const;
};

#endif //BALL_H
