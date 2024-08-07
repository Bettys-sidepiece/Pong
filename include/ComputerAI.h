#ifndef COMPUTER_AI_H
#define COMPUTER_AI_H

#include "Ball.h"
#include "Player.h"

enum class Difficulty {
    EASY,
    NORMAL,
    HARD
};

class ComputerAI {
private:
    Difficulty m_difficulty;
    float m_reactionTime;
    float m_predictionAccuracy;
    float m_maxSpeed;
    SDL_Event m_moveEvent;

    float predictBallPosition(const Ball& ball, const Player& aiPlayer);

public:
    ComputerAI(Difficulty difficulty = Difficulty::NORMAL);
    ~ComputerAI() = default;
    float calculateMovement(const Ball& ball, const Player& aiPlayer, int gameAreaWidth, int uiAreaWidth);
    void setDifficulty(Difficulty difficulty);
    void update(const Ball& ball, Player& aiPlayer, float deltaTime, int gameAreaWidth, int uiAreaWidth);
};

#endif /* COMPUTER_AI_H */