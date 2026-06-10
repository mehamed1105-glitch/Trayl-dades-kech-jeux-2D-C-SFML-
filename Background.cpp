#include "Background.h"
#include "TextureManager.h"
#include "Constants.h"
#include <algorithm>

Background::Background()
    : m_hasTexture(false), m_x1(0.f), m_x2((float)WINDOW_WIDTH)
{
    m_fallback1.setSize(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    m_fallback1.setFillColor(sf::Color(30, 60, 110));
    m_fallback2 = m_fallback1;
    m_fallback2.setPosition((float)WINDOW_WIDTH, 0.f);
}

void Background::setTexture(const std::string& id)
{
    auto& tm = TextureManager::getInstance();
    m_hasTexture = tm.has(id);
    if (!m_hasTexture) return;

    const sf::Texture& tex = tm.get(id);
    m_sprite1.setTexture(tex);
    m_sprite2.setTexture(tex);

    float scaleX = (float)WINDOW_WIDTH / tex.getSize().x;
    float scaleY = (float)WINDOW_HEIGHT / tex.getSize().y;
    float scale = std::max(scaleX, scaleY);

    m_sprite1.setScale(scale, scale);
    m_sprite2.setScale(scale, scale);
    m_sprite1.setPosition(m_x1, 0.f);
    m_sprite2.setPosition(m_x2, 0.f);
}

void Background::update(float dt, float scrollSpeed)
{
    float move = scrollSpeed * 0.3f * dt;
    m_x1 -= move;
    m_x2 -= move;

    if (m_x1 + (float)WINDOW_WIDTH < 0.f) m_x1 = m_x2 + (float)WINDOW_WIDTH;
    if (m_x2 + (float)WINDOW_WIDTH < 0.f) m_x2 = m_x1 + (float)WINDOW_WIDTH;

    if (m_hasTexture) {
        m_sprite1.setPosition(m_x1, 0.f);
        m_sprite2.setPosition(m_x2, 0.f);
    }
    else {
        m_fallback1.setPosition(m_x1, 0.f);
        m_fallback2.setPosition(m_x2, 0.f);
    }
}

void Background::draw(sf::RenderWindow& window) const
{
    if (m_hasTexture) {
        window.draw(m_sprite1);
        window.draw(m_sprite2);
    }
    else {
        window.draw(m_fallback1);
        window.draw(m_fallback2);

        sf::RectangleShape ground(sf::Vector2f((float)WINDOW_WIDTH, 120.f));
        ground.setFillColor(sf::Color(80, 55, 30));
        ground.setPosition(0.f, (float)WINDOW_HEIGHT - 120.f);
        const_cast<sf::RenderWindow&>(window).draw(ground);

        sf::RectangleShape road(sf::Vector2f((float)WINDOW_WIDTH, 10.f));
        road.setFillColor(sf::Color(210, 180, 80));
        road.setPosition(0.f, PLAYER_GROUND_Y + PLAYER_H);
        const_cast<sf::RenderWindow&>(window).draw(road);
    }
}