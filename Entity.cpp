#include "Entity.h"
#include <iostream>

// Default constructor
Entity::Entity()
    : m_x(0.f), m_y(0.f), m_width(32.f), m_height(32.f), m_alive(true)
{
}

// Parameterised constructor
Entity::Entity(float x, float y, float w, float h)
    : m_x(x), m_y(y), m_width(w), m_height(h), m_alive(true)
{
}

// Copy constructor
Entity::Entity(const Entity& other)
    : m_sprite(other.m_sprite),
    m_x(other.m_x), m_y(other.m_y),
    m_width(other.m_width), m_height(other.m_height),
    m_alive(other.m_alive)
{
}

// Destructor
Entity::~Entity() {}


sf::FloatRect Entity::getBounds() const
{
    return sf::FloatRect(m_x, m_y, m_width, m_height);
}

sf::Vector2f Entity::getPosition() const
{
    return sf::Vector2f(m_x, m_y);
}

void Entity::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    m_sprite.setPosition(x, y);
}

bool Entity::isAlive() const { return m_alive; }
void Entity::setAlive(bool alive) { m_alive = alive; }

// Operator overloads
bool Entity::operator<(const Entity& other) const { return m_x < other.m_x; }
bool Entity::operator==(const Entity& other) const { return m_x == other.m_x && m_y == other.m_y; }

// Friend function
void printEntityInfo(const Entity& e)
{
    std::cout << "[Entity] pos=(" << e.m_x << "," << e.m_y
        << ") size=(" << e.m_width << "x" << e.m_height
        << ") alive=" << e.m_alive << "\n";
}