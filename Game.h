#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "Player.h"
#include "ObstacleManager.h"
#include "Background.h"
#include "HUD.h"

enum class GameState { Splash, Menu, Playing, Paused, GameOver, Win, About };

class Game
{
public:
    Game();
    void run();

private:
    // Fenêtre
    sf::RenderWindow m_window;
    sf::Font         m_font;
    bool             m_fontLoaded;

    // Systèmes de jeu
    Player          m_player;
    ObstacleManager m_obstacles;
    Background      m_background;
    HUD* m_hud;

    // État
    GameState m_state;
    int       m_level;
    float     m_gameSpeed;
    float     m_timeLeft;
    float     m_totalTime;
    bool      m_invincible;
    float     m_invincibleTimer;
    float     m_flashTimer;
    float     m_splashTimer;

    // Menu
    struct Button {
        sf::RectangleShape shape;
        sf::Text           label;
        sf::Sprite         sprite;
        bool               hasSprite = false;
        bool contains(sf::Vector2f pos) const { return shape.getGlobalBounds().contains(pos); }
    };
    Button    m_btnPlay, m_btnAbout, m_btnQuit, m_btnBack;
    sf::Sprite m_menuBg;
    bool       m_menuBgLoaded = false;

    sf::Text  m_aboutText;

    // Helpers
    void loadAssets();
    void buildMenu();
    Button makeButton(sf::Vector2f pos, sf::Vector2f size, const std::string& label, const std::string& spriteId = "");

    void startLevel(int level);

    void handleEvents();
    void update(float dt);
    void render();

    void drawMenu();
    void drawGame();
    void drawAbout();
    void drawOverlay(const std::string& title, const std::string& sub);
    void drawWinScreen();

    void handleMenuClick(sf::Vector2f pos);
    void handleGameKeys(const sf::Event& e);
    void processCollision(Obstacle* obs);
};