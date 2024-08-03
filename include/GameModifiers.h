#ifndef GAME_MODIFIERS_H
#define GAME_MODIFIERS_H

#include <vector>
#include <SDL2/SDL.h>

class Player; // Forward declaration

enum class ModifierType {
    BALL_SPEED_UP = 0,
    BALL_SLOW_DOWN,
    PADDLE_SHRINK,
    PADDLE_SPEED_UP,
    PADDLE_INCREASE,
    PADDLE_SLOW_DOWN,
    PADDLE_INVERTED,
    SIDE_REVERSAL,
    // Add other types as needed
};

class GameModifier {
public:
    GameModifier(ModifierType type, SDL_Rect position);
    void update(float deltaTime);
    void applyEffect(Player& player);
    bool isActive() const;
    SDL_Rect getPosition() const { return m_position; }
    ModifierType getType() const { return m_type; }

private:
    ModifierType m_type;
    SDL_Rect m_position;
    bool m_isActive;
    float m_duration;
};

class GameModifierManager {
public:
    GameModifierManager();
    void spawnModifier(int gameAreaWidth, int uiAreaWidth, int windowHeight);
    void updateModifiers(float deltaTime);
    void checkCollisions(Player& player);
    void render(SDL_Renderer* renderer);

private:
    std::vector<GameModifier> m_activeModifiers;
};

#endif // GAME_MODIFIERS_H