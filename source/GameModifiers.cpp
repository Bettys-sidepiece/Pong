#include "GameModifiers.h"
#include "Player.h"
#include <random>

GameModifier::GameModifier(ModifierType type, SDL_Rect position)
    : m_type(type), m_position(position), m_isActive(true), m_duration(10.0f) {}

void GameModifier::update(float deltaTime) {
    m_duration -= deltaTime;
    m_isActive = m_duration > 0;
}
/* BALL_SPEED_UP = 0,
    BALL_SLOW_DOWN = 1,
    PADDLE_SHRINK = 2,
    PADDLE_SPEED_UP = 3,
    PADDLE_INCREASE = 4,
    PADDLE_SLOW_DOWN = 5,
    PADDLE_INVERTED = 6,
    SIDE_REVERSAL =7 */
void GameModifier::applyEffect(Player& player) {
    switch(m_type) {
        case ModifierType::BALL_SLOW_DOWN:
            player.setSpeed(player.getSpeed() * 1.5f);
            break;
        case ModifierType::BALL_SPEED_UP:
            player.setSize(player.getSpeed() * 0.5f);
            break;
        case ModifierType::PADDLE_SHRINK:
            break;

        case ModifierType::PADDLE_SPEED_UP:
            break;
        
        case ModifierType::PADDLE_INCREASE:
            break;

        case ModifierType::PADDLE_SLOW_DOWN:
            break;

        case ModifierType::PADDLE_INVERTED:
            break;

        case ModifierType::SIDE_REVERSAL:
            break;
        // Implement other modifier effects
    }
}

bool GameModifier::isActive() const {
    return m_isActive;
}

GameModifierManager::GameModifierManager() {}

void GameModifierManager::spawnModifier(int gameAreaWidth, int uiAreaWidth, int windowHeight) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, static_cast<int>(ModifierType::PADDLE_SHRINK));
    std::uniform_int_distribution<> xDist(uiAreaWidth, uiAreaWidth + gameAreaWidth - 20);
    std::uniform_int_distribution<> yDist(0, windowHeight - 20);

    ModifierType type = static_cast<ModifierType>(typeDist(gen));
    SDL_Rect position = {xDist(gen), yDist(gen), 20, 20};

    m_activeModifiers.emplace_back(type, position);
}

void GameModifierManager::updateModifiers(float deltaTime) {
    for (auto& modifier : m_activeModifiers) {
        modifier.update(deltaTime);
    }

    m_activeModifiers.erase(
        std::remove_if(m_activeModifiers.begin(), m_activeModifiers.end(),
            [](const GameModifier& m) { return !m.isActive(); }),
        m_activeModifiers.end());
}

void GameModifierManager::checkCollisions(Player& player) {
    SDL_Rect playerRect = player.getRect();
    SDL_Rect position;
    
    for (auto& modifier : m_activeModifiers) {

        position = modifier.getPosition();
        if (modifier.isActive() && SDL_HasIntersection(&playerRect, &position)) {
            modifier.applyEffect(player);
            modifier = GameModifier(ModifierType::PADDLE_SPEED_UP, {0, 0, 0, 0}); // Deactivate after use
        }
    }
}

void GameModifierManager::render(SDL_Renderer* renderer) {
    for (const auto& modifier : m_activeModifiers) {
        if (modifier.isActive()) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color for modifiers
            SDL_Rect position = modifier.getPosition();
            SDL_RenderFillRect(renderer, &position);
        }
    }
}