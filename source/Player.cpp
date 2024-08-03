#include "Player.h"

Player::Player(int x, int y, int width, int height) : m_rect{x, y, width, height}, m_speed(10) {}

void Player::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a) {
            m_rect.x -= m_speed;
        } else if (event.key.keysym.sym == SDLK_d) {
            m_rect.x += m_speed;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        m_rect.x = event.motion.x - m_rect.w / 2;
    }
}

void Player::update(int gameAreaWidth, int uiAreaWidth) {
    if (m_rect.x < uiAreaWidth) {
        m_rect.x = uiAreaWidth;
    } else if (m_rect.x > (gameAreaWidth + uiAreaWidth) - m_rect.w) {
        m_rect.x = (gameAreaWidth + uiAreaWidth) - m_rect.w;
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &m_rect);
}

void Player::setSize(float size){

}

void Player::setSpeed(float speed){

}

