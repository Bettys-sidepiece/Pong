#ifndef GAME_MODIFIERS_H
#define GAME_MODIFIERS_H

#include <vector>
#include "Vector2D.h"

class Player; // Forward declaration

enum class ModifierType {
    SPEED_BOOST,
    SIZE_INCREASE,
    BALL_SPEED_UP,
    PADDLE_SHRINK,
    // Add other types as needed
};

class GameModifier {
public:
    GameModifier(ModifierType type, Vector2D position);
    void update(float deltaTime);
    void applyEffect(Player& player);
    bool isActive() const;

private:
    ModifierType m_type;
    Vector2D m_position;
    bool m_isActive;
    float m_duration;
};

class GameModifierManager {
public:
    GameModifierManager();
    void spawnModifier();
    void updateModifiers(float deltaTime);
    void checkCollisions(Player& player);

private:
    std::vector<GameModifier> m_activeModifiers;
};

#endif // GAME_MODIFIERS_H