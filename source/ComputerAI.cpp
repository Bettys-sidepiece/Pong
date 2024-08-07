#include "ComputerAI.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

ComputerAI::ComputerAI(Difficulty difficulty) {
    setDifficulty(difficulty);
}

void ComputerAI::setDifficulty(Difficulty difficulty) {
    m_difficulty = difficulty;
    switch (m_difficulty) {
        case Difficulty::EASY:
            m_reactionTime = 0.15f;
            m_predictionAccuracy = 0.85f;
            m_maxSpeed = 60.0f;
            break;
        case Difficulty::NORMAL:
            m_reactionTime = 0.1f;
            m_predictionAccuracy = 0.9f;
            m_maxSpeed = 70.0f;
            break;
        case Difficulty::HARD:
            m_reactionTime = 0.025f;
            m_predictionAccuracy = 0.99f;
            m_maxSpeed = 90.0f;
            break;
    }
}

float ComputerAI::predictBallPosition(const Ball& ball, const Player& aiPlayer) {
    // Simple linear prediction
    float timeToReachAI = std::abs(ball.getBallRect().y - aiPlayer.getRect().y) / ball.getSpeedY();
    float predictedX = ball.getBallRect().x + ball.getSpeedX() * timeToReachAI;

    // Add inaccuracy based on difficulty
    static std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0, (1.0f - m_predictionAccuracy) * 100.0f);
    predictedX += distribution(generator);

    return predictedX;
}

float ComputerAI::calculateMovement(const Ball& ball, const Player& aiPlayer, int gameAreaWidth, int uiAreaWidth) {
    float targetX = predictBallPosition(ball, aiPlayer);
    float paddleCenter = aiPlayer.getRect().x + aiPlayer.getRect().w / 2;
    float moveDirection = targetX - paddleCenter;
    
    // Normalize the move direction and apply max speed
    if (moveDirection > 0) {
        return std::min(moveDirection, m_maxSpeed);
    } else {
        return std::max(moveDirection, -m_maxSpeed);
    }
}

void ComputerAI::update(const Ball& ball, Player& aiPlayer, float deltaTime, int gameAreaWidth, int uiAreaWidth) {
    static float timeSinceLastMove = 0.0f;
    timeSinceLastMove += deltaTime;
    
    if (timeSinceLastMove >= m_reactionTime) {
        float movement = calculateMovement(ball, aiPlayer, gameAreaWidth, uiAreaWidth);
        aiPlayer.move(movement);
        timeSinceLastMove = 0.0f;
    }
}