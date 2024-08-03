#include "Ball.h"
#include <cmath>

Ball::Ball(int x, int y, int radius, float speedX, float speedY)
    : m_rect{x - radius, y - radius, 2 * radius, 2 * radius}, m_radius(radius), m_speedX(speedX), m_speedY(speedY) {}

void Ball::update(int gameAreaWidth, int &score) {
    m_rect.x += m_speedX;
    m_rect.y += m_speedY;

    // Check for collision with game area bounds
    if (m_rect.x - m_rect.w < gameAreaWidth) {
        m_speedX = -m_speedX;
    }

    if(m_rect.x + m_rect.w > gameAreaWidth){
        m_speedX = -m_speedX;
    }

    if (m_rect.y < 0) {
        score += 1; // Increment score when the ball hits the top
    }
    if (m_rect.y + m_rect.h > 1000) {
        score -= 1; // Decrement score when the ball hits the bottom
    }
}

void Ball::render(SDL_Renderer* renderer) const {
    drawCircle(renderer, m_rect.x + m_radius, m_rect.y + m_radius, m_radius);
}

void Ball::collisionDetection(const SDL_Rect &playerRect, const SDL_Rect &player2Rect) {
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

void Ball::setSpeed(float speedX, float speedY) {
    m_speedX = speedX;
    m_speedY = speedY;
}

void Ball::drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) const {
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
