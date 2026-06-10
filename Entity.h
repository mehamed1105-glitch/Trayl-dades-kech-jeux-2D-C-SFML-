#pragma once
#include <SFML/Graphics.hpp>
#include <string>

//  Template helper: clamp a value between two bounds

template <typename T>
T clamp(T value, T minVal, T maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

//  Abstract base class for all game entities
class Entity
{
public:
    // Default constructor
    Entity();

    // Parameterised constructor
    Entity(float x, float y, float w, float h);

    // Copy constructor
    Entity(const Entity& other);

    // Destructor (virtual so derived dtors are called correctly)
    virtual ~Entity();

    // Pure virtual update / draw
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    // Getters / setters
    sf::FloatRect getBounds() const;
    sf::Vector2f  getPosition() const;
    void          setPosition(float x, float y);
    bool          isAlive() const;
    void          setAlive(bool alive);

    // Operator overload: compare two entities by their X position
    bool operator<(const Entity& other) const;
    bool operator==(const Entity& other) const;

    // Friend function: prints debug info
    friend void printEntityInfo(const Entity& e);

protected:
    mutable sf::Sprite   m_sprite;
    float        m_x, m_y;
    float        m_width, m_height;
    bool         m_alive;
};