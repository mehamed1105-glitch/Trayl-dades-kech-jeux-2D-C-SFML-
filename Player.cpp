#include "Player.h"
#include "TextureManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <string>

Player::Player()
    : m_x(PLAYER_START_X), m_y(PLAYER_GROUND_Y),
    m_width(PLAYER_W), m_height(PLAYER_H),
    m_velocityY(0.f), m_state(PlayerState::Running),
    m_lives(MAX_LIVES), m_score(0), m_level(1),
    m_animFrame(0), m_animTimer(0.f), m_slideBlocked(false), m_slideTimer(0.f)
{
    m_fallback.setSize(sf::Vector2f(PLAYER_W, PLAYER_H));
    m_fallback.setFillColor(sf::Color(50, 180, 255));
    m_fallback.setPosition(m_x, m_y);
}

sf::FloatRect Player::getBounds() const
{
    return sf::FloatRect(m_x, m_y, m_width, m_height);
}

void Player::jump()
{
    if (m_state == PlayerState::Running) {
        m_velocityY = JUMP_FORCE;
        m_state = PlayerState::Jumping;
    }
}

void Player::startSlide()
{
    if (m_state == PlayerState::Running) {
        m_state = PlayerState::Sliding;
        m_height = PLAYER_SLIDE_H;
        m_y = PLAYER_GROUND_Y + (PLAYER_H - PLAYER_SLIDE_H);
        // block retrigger until key released
        m_slideBlocked = true;
    }
}

void Player::stopSlide()
{
    if (m_state == PlayerState::Sliding) {
        m_state = PlayerState::Running;
        m_height = PLAYER_H;
        m_y = PLAYER_GROUND_Y;
        m_slideTimer = 0.f;
    }
}

void Player::applyGravity(float dt)
{
    if (m_state == PlayerState::Jumping) {
        m_velocityY += GRAVITY * dt;
        m_y += m_velocityY * dt;
        if (m_y >= PLAYER_GROUND_Y) {
            m_y = PLAYER_GROUND_Y;
            m_velocityY = 0.f;
            m_state = PlayerState::Running;
        }
    }
}

void Player::update(float dt)
{
    // Jump input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        jump();

    // Slide input
    bool downHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    if (!downHeld) m_slideBlocked = false;
    if (!m_slideBlocked && downHeld && m_state == PlayerState::Running) startSlide();
    if (!downHeld && m_state == PlayerState::Sliding)                   stopSlide();
    // sliding: remain sliding while the down key is held; stop when released
    // (m_slideBlocked is used only to prevent retrigger while key stays held)
    applyGravity(dt);

    // Animation frames (running: 8 frames, slide: 3 frames)
    if (m_state == PlayerState::Running || m_state == PlayerState::Sliding) {
        m_animTimer += dt;
        float fps = (m_state == PlayerState::Running) ? 0.1f : 0.12f;
        int   max = (m_state == PlayerState::Running) ? 8 : 3;
        if (m_animTimer >= fps) {
            m_animTimer = 0.f;
            m_animFrame = (m_animFrame + 1) % max;
        }
    }
    else {
        m_animFrame = 0;
    }

    // Sync fallback
    m_fallback.setSize(sf::Vector2f(m_width, m_height));
    m_fallback.setPosition(m_x, m_y);
}

void Player::draw(sf::RenderWindow& window) const
{
    auto& tm = TextureManager::getInstance();

    // Slide sprite
    if (m_state == PlayerState::Sliding && tm.has("player10")) {
        m_sprite.setTexture(tm.get("player10"));
        m_sprite.setScale(m_width / m_sprite.getTexture()->getSize().x,
            m_height / m_sprite.getTexture()->getSize().y);
        m_sprite.setPosition(m_x, m_y);
        window.draw(m_sprite);
        return;
    }

    // Running / jumping frames
    std::string frameId = "player" + std::to_string(1 + m_animFrame % 8);
    if (m_state == PlayerState::Jumping && tm.has("player8")) frameId = "player8";

    if (tm.has(frameId)) {
        m_sprite.setTexture(tm.get(frameId));
        m_sprite.setScale(m_width / m_sprite.getTexture()->getSize().x,
            m_height / m_sprite.getTexture()->getSize().y);
        m_sprite.setPosition(m_x, m_y);
        window.draw(m_sprite);
    }
    else {
        window.draw(m_fallback);
    }
}

void Player::onObstacle1Hit(float& gameSpeed)
{
    gameSpeed = std::max(BASE_SPEED, gameSpeed - 60.f);
    m_score = std::max(0, m_score - 20);
    loseLife();
}