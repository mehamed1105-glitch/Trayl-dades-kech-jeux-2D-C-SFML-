#include "Obstacle.h"
#include "TextureManager.h"

// ── Base Obstacle ─────────────────────────────────────────────────────────────

Obstacle::Obstacle(float x, float y, float w, float h, ObstacleType type, float speed)
    : m_x(x), m_y(y), m_width(w), m_height(h), m_type(type), m_scrollSpeed(speed)
{
    m_fallback.setSize(sf::Vector2f(w, h));
    m_fallback.setFillColor(sf::Color(220, 80, 80));
    m_fallback.setPosition(x, y);
}

void Obstacle::update(float dt)
{
    m_x -= m_scrollSpeed * dt;
    m_fallback.setPosition(m_x, m_y);
    m_sprite.setPosition(m_x, m_y);
    if (m_x + m_width < 0.f) m_alive = false;
}

void Obstacle::draw(sf::RenderWindow& window) const
{
   
}

// ── GroundObstacle ────────────────────────────────────────────────────────────

GroundObstacle::GroundObstacle(float x, ObstacleType type, float speed)
    : Obstacle(x, PLAYER_GROUND_Y + (PLAYER_H - 128.f), 96.f, 128.f, type, speed)
{
    sf::Color col = (type == ObstacleType::Ground1) ? sf::Color(255, 160, 0) : sf::Color(200, 0, 0);
    m_fallback.setFillColor(col);

    std::string texId = (type == ObstacleType::Ground1) ? "obstacle1" : "obstacle2";
    auto& tm = TextureManager::getInstance();
    if (tm.has(texId)) {
        m_sprite.setTexture(tm.get(texId));
        m_sprite.setScale(m_width / tm.get(texId).getSize().x,
            m_height / tm.get(texId).getSize().y);
        m_sprite.setPosition(m_x, m_y);
    }
}

void GroundObstacle::draw(sf::RenderWindow& window) const
{
    std::string texId = (m_type == ObstacleType::Ground1) ? "obstacle1" : "obstacle2";
    if (TextureManager::getInstance().has(texId))
        window.draw(m_sprite);
    else
        window.draw(m_fallback);
}

// ── AerialObstacle ────────────────────────────────────────────────────────────

AerialObstacle::AerialObstacle(float x, float speed)
    : Obstacle(x, PLAYER_GROUND_Y - 100.f, 100.f, 100.f, ObstacleType::Aerial, speed),
    m_baseY(PLAYER_GROUND_Y - 100.f)
{
    m_fallback.setFillColor(sf::Color(80, 80, 220));

    auto& tm = TextureManager::getInstance();
    if (tm.has("obstacle3")) {
        m_sprite.setTexture(tm.get("obstacle3"));
        m_sprite.setScale(m_width / tm.get("obstacle3").getSize().x,
            m_height / tm.get("obstacle3").getSize().y);
        m_sprite.setPosition(m_x, m_y);
    }
}

void AerialObstacle::update(float dt)
{
    m_x -= m_scrollSpeed * dt;
    m_y = m_baseY;
    if (m_x + m_width < 0.f) { m_alive = false; return; }
    m_fallback.setPosition(m_x, m_y);
    m_sprite.setPosition(m_x, m_y);
}

void AerialObstacle::draw(sf::RenderWindow& window) const
{
    if (TextureManager::getInstance().has("obstacle3"))
        window.draw(m_sprite);
    else
        window.draw(m_fallback);
}