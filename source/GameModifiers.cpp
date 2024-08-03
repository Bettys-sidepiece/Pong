#include "GameModifiers.h"
#include "Player.h"
#include <random>

// GameModifier implementation

GameModifier::GameModifier(ModifierType type, Vector2D position)
    : m_type(type), m_position(position), m_isActive(true), m_duration(10.0f) // Example duration
{
}

void GameModifier::update(float deltaTime)
{
    m_duration -= deltaTime;
    m_isActive = m_duration > 0;
}

void GameModifier::applyEffect(Player& player)
{
    switch(m_type) {
        case ModifierType::SPEED_BOOST:
            player.setSpeed(player.getSpeed() * 1.5f);
            break;
        case ModifierType::SIZE_INCREASE:
            player.setSize(player.getSize() * 1.2f);
            break;
        // Implement other modifier effects
    }
}

bool GameModifier::isActive() const
{
    return m_isActive;
}

// GameModifierManager implementation

GameModifierManager::GameModifierManager() {}

void GameModifierManager::spawnModifier()
{
    // Implement spawning logic
    // Example:
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, static_cast<int>(ModifierType::PADDLE_SHRINK));
    std::uniform_real_distribution<> posDist(0, 1); // Assume normalized coordinates

    ModifierType type = static_cast<ModifierType>(typeDist(gen));
    Vector2D position(posDist(gen), posDist(gen));

    m_activeModifiers.emplace_back(type, position);
}

void GameModifierManager::updateModifiers(float deltaTime)
{
    for (auto& modifier : m_activeModifiers) {
        modifier.update(deltaTime);
    }

    // Remove inactive modifiers
    m_activeModifiers.erase(
        std::remove_if(m_activeModifiers.begin(), m_activeModifiers.end(),
            [](const GameModifier& m) { return !m.isActive(); }),
        m_activeModifiers.end());
}

void GameModifierManager::checkCollisions(Player& player)
{
    for (auto& modifier : m_activeModifiers) {
        if (modifier.isActive() && player.collidesWith(modifier)) {
            modifier.applyEffect(player);
            modifier.m_isActive = false; // Deactivate after use
        }
    }
}