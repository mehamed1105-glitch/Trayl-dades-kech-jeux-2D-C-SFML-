#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

enum class PlayerState { Running, Jumping, Sliding };

class Player
{
public:
    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    void jump();
    void startSlide();
    void stopSlide();

    sf::FloatRect getBounds() const;

    PlayerState getState()  const { return m_state; }
    int  getLives()         const { return m_lives; }
    int  getScore()         const { return m_score; }

    void addScore(int pts) { m_score += pts; }
    void loseLife() { if (m_lives > 0) m_lives--; }
    void setLevel(int lvl) { m_level = lvl; }

    void onObstacle2Hit() { loseLife(); }
    void onObstacle3Hit() { loseLife(); }
    void onObstacle1Hit(float& gameSpeed);

private:
    void applyGravity(float dt);

    mutable sf::Sprite      m_sprite;
    sf::RectangleShape      m_fallback;

    float       m_x, m_y, m_width, m_height;
    float       m_velocityY;
    PlayerState m_state;
    int         m_lives;
    int         m_score;
    int         m_level;
    int         m_animFrame;
    float       m_animTimer;
    bool        m_slideBlocked;
    float       m_slideTimer;
};