#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

enum class ObstacleType { Ground1, Ground2, Aerial };

class Obstacle
{
public:
    Obstacle(float x, float y, float w, float h, ObstacleType type, float speed);

    virtual ~Obstacle() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const { return sf::FloatRect(m_x, m_y, m_width, m_height); }
    ObstacleType  getType()   const { return m_type; }
    bool isAlive()            const { return m_alive; }
    bool wasHit()             const { return m_wasHit; }
    void markHit() { m_wasHit = true; }
    void setScrollSpeed(float s) { m_scrollSpeed = s; }

protected:
    mutable sf::Sprite  m_sprite;
    sf::RectangleShape  m_fallback;
    float       m_x, m_y, m_width, m_height;
    float       m_scrollSpeed;
    ObstacleType m_type;
    bool        m_alive = true;
    bool        m_wasHit = false;
};

class GroundObstacle : public Obstacle
{
public:
    GroundObstacle(float x, ObstacleType type, float speed);
    void draw(sf::RenderWindow& window) const override;
};

class AerialObstacle : public Obstacle
{
public:
    AerialObstacle(float x, float speed);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
private:
    float m_baseY;
};