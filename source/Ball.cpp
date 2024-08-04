#include "Ball.h"
#include <cmath>
#include <iostream>

Ball::Ball(int x, int y, int radius)
    : m_rect{x - radius, y - radius, 2 * radius, 2 * radius}, m_radius(radius), m_speedX(0), m_speedY(0), m_ballIsAttached(0), m_attachedTo(nullptr) {}

void Ball::update(int gameAreaWidth, int uiAreaWidth,int &score) 
{
   
    if(!isAttached() && m_attachedTo != nullptr){
        if (m_ballIsAttached > 0) {
            // Move the ball with player 1's paddle
            m_rect.x = m_attachedTo->x + (m_attachedTo->w / 2) - m_radius;
            m_rect.y = m_attachedTo->y - m_rect.h - 1;
        } else if(m_ballIsAttached < 0) {
            // Move the ball with player 2's paddle
            m_rect.x = m_attachedTo->x + (m_attachedTo->w / 2) - m_radius;
            m_rect.y = m_attachedTo->y + m_attachedTo->h + 1;
        }
    }else{
        m_rect.x += m_speedX;
        m_rect.y += m_speedY;

        // Check for collision with game area bounds
        if (m_rect.x < uiAreaWidth) {
            m_rect.x = uiAreaWidth;
            m_speedX = -m_speedX;
        }

        if(m_rect.x > (uiAreaWidth + gameAreaWidth)- m_rect.w){
            m_rect.x = gameAreaWidth + uiAreaWidth - m_rect.w;
            m_speedX = -m_speedX;
        }

        if (m_rect.y < 0) {
            score += 1; // Increment score when the ball hits the top
        }
        if (m_rect.y + m_rect.h > 1000) {
            score -= 1; // Decrement score when the ball hits the bottom
        }
    }
}

void Ball::render(SDL_Renderer* renderer) const 
{
    drawCircle(renderer, m_rect.x + m_radius, m_rect.y + m_radius, m_radius);
}

void Ball::collisionDetection(const SDL_Rect &playerRect, const SDL_Rect &player2Rect) 
{
    if (SDL_HasIntersection(&m_rect, &playerRect)) {
        m_speedY = -std::abs(m_speedY); // Reverse Y direction on collision
        int ballCenter = m_rect.x + m_radius;
        int paddleCenter = playerRect.x + playerRect.w / 2;
        m_speedX += 0.1f * (ballCenter - paddleCenter);
    }

    if (SDL_HasIntersection(&m_rect, &player2Rect)) {
        m_speedY = std::abs(m_speedY); // Reverse Y direction on collision
        int ballCenter = m_rect.x + m_radius;
        int paddleCenter = player2Rect.x + player2Rect.w / 2;
        m_speedX += 0.1f * (ballCenter - paddleCenter);
    }
}

void Ball::setSpeed(float speedX, float speedY) 
{
    m_speedX = speedX;
    m_speedY = speedY;
}

void Ball::attachTo(SDL_Rect* playerRect, int isPlayer) 
{
    (isPlayer > 0)? (m_ballIsAttached = 1) : (m_ballIsAttached = -1);
    m_attachedTo = playerRect;
}

void Ball::launch() 
{
    m_ballIsAttached = 0;
    m_speedX = 0.0f;
    m_speedY = 5.0f;
}

bool Ball::isAttached() const
{
    if(m_ballIsAttached!=0) return false;
    return true;
}

void Ball::drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const 
{
      for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}
