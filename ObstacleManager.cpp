#include "ObstacleManager.h"
#include <algorithm>
#include <random>

ObstacleManager::ObstacleManager()
    : m_spawnTimer(0.f), m_nextSpawnInterval(1.5f), m_minInterval(1.0f), m_maxInterval(2.0f)
{
    std::random_device rd;
    m_rng.seed(rd());
    resetSpawnInterval();
}

ObstacleManager::~ObstacleManager() = default;

void ObstacleManager::resetSpawnInterval()
{
    std::uniform_real_distribution<float> dist(m_minInterval, m_maxInterval);
    m_nextSpawnInterval = dist(m_rng);
}

void ObstacleManager::spawnObstacle(float gameSpeed)
{
    int roll = std::uniform_int_distribution<int>(0, 1)(m_rng);
    if (roll == 0) {
        m_pool.add(std::make_unique<GroundObstacle>((float)WINDOW_WIDTH + 200, ObstacleType::Ground2, gameSpeed));
    }
    else {
        m_pool.add(std::make_unique<AerialObstacle>((float)WINDOW_WIDTH + 200, gameSpeed));
    }
}

void ObstacleManager::advanceSpawnTimer(float dt, float gameSpeed)
{
    m_spawnTimer += dt;
    if (m_spawnTimer >= m_nextSpawnInterval) {
        m_spawnTimer = 0.f;
        spawnObstacle(gameSpeed);
        resetSpawnInterval();
    }
}

void ObstacleManager::update(float dt, float gameSpeed)
{
    advanceSpawnTimer(dt, gameSpeed);

    auto& all = m_pool.getAll();
    for (auto& up : all) {
        if (up) {
            up->setScrollSpeed(gameSpeed);
            up->update(dt);
        }
    }

    m_pool.removeDeadOnes();
}

void ObstacleManager::draw(sf::RenderWindow& window) const
{
    for (const auto& up : m_pool.getAll()) {
        if (up) up->draw(window);
    }
}

void ObstacleManager::reset()
{
    m_pool.clear();
    m_spawnTimer = 0.f;
    resetSpawnInterval();
}

const Obstacle* ObstacleManager::getCollidingObstacle(const sf::FloatRect& bounds) const
{
    for (const auto& up : m_pool.getAll()) {
        if (!up) continue;
        sf::FloatRect hitbox = up->getBounds();
        hitbox.left += 20.f;  hitbox.width -= 40.f;
        hitbox.top += 20.f;  hitbox.height -= 40.f;
        if (bounds.intersects(hitbox))
            return up.get();
    }
    return nullptr;
}

std::size_t ObstacleManager::getCount() const
{
    return m_pool.count();
}

void ObstacleManager::setSpawnIntervals(float min, float max)
{
    m_minInterval = min;
    m_maxInterval = max;
    resetSpawnInterval();
}
