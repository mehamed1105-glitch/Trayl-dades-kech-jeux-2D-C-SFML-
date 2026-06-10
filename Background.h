#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Background
{
public:
    Background();
    void setTexture(const std::string& id);
    void update(float dt, float scrollSpeed);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite m_sprite1, m_sprite2;
    sf::RectangleShape m_fallback1, m_fallback2;
    bool  m_hasTexture = false;
    float m_x1, m_x2;
};