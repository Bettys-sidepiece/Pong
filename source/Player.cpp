#include "Player.h"
#include <algorithm>

Player::Player(int x, int y, int width, int height, int id)
    : m_rect{x, y, width, height}, m_speed(10), m_score(0), m_id(id){}

void Player::handleEvent(const SDL_Event& event) {
    if(m_id == 1){
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_a) {
                m_rect.x -= m_speed;
            } else if (event.key.keysym.sym == SDLK_d) {
                m_rect.x += m_speed;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            m_rect.x = event.motion.x - m_speed;
        }
    }
}

void Player::move(float movement) {
    m_rect.x += static_cast<int>(movement);
}

void Player::update(int gameAreaWidth, int uiAreaWidth) {
    m_rect.x = std::max(uiAreaWidth, std::min(m_rect.x, uiAreaWidth + gameAreaWidth - m_rect.w));
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &m_rect);
}

void Player::setSize(float size) {
    m_size = size;
    m_rect.w = static_cast<int>(m_rect.w * m_size);
    m_rect.h = static_cast<int>(m_rect.h * m_size);
}
