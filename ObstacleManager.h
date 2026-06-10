#pragma once
#include "Obstacle.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

// ─────────────────────────────────────────────
//  Generic ObjectPool template
// ─────────────────────────────────────────────
template <typename T>
class ObjectPool
{
public:
    void add(T obj) { m_pool.push_back(std::move(obj)); }
    std::vector<T>& getAll() { return m_pool; }
    const std::vector<T>& getAll() const { return m_pool; }
    void removeDeadOnes()
    {
        m_pool.erase(
            std::remove_if(m_pool.begin(), m_pool.end(),
                [](const T& o) { return !o->isAlive(); }),
            m_pool.end());
    }
    std::size_t count() const { return m_pool.size(); }
    void clear() { m_pool.clear(); }

private:
    std::vector<T> m_pool;
};

// ─────────────────────────────────────────────
//  ObstacleManager — composes ObjectPool
// ─────────────────────────────────────────────
class ObstacleManager
{
public:
    ObstacleManager();
    ~ObstacleManager();

    void update(float dt, float gameSpeed);
    void draw(sf::RenderWindow& window) const;
    void reset();

    // Returns index of colliding obstacle, -1 if none
    int  checkCollision(const sf::FloatRect& playerBounds) const;

    // Pointer to colliding obstacle (or nullptr)
    const Obstacle* getCollidingObstacle(const sf::FloatRect& bounds) const;

    std::size_t getCount() const;

    // Set spawn intervals for difficulty
    void setSpawnIntervals(float min, float max);

    // Spawn timer advance
    void advanceSpawnTimer(float dt, float gameSpeed);

private:
    void spawnObstacle(float gameSpeed);

    // Aggregation: external composition with ObjectPool
    ObjectPool<std::unique_ptr<Obstacle>> m_pool;

    float m_spawnTimer;
    float m_nextSpawnInterval;
    float m_minInterval;
    float m_maxInterval;
    std::mt19937 m_rng; // random engine for spawn intervals

    void resetSpawnInterval();
};