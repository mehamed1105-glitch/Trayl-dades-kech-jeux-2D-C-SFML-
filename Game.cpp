#include "Game.h"
#include "TextureManager.h"
#include "Constants.h"
#include <sstream>
#include <iostream>
#include <algorithm>

// ── Constructeur ──────────────────────────────────────────────────────────────

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Trayl Dades <=> Kech",
        sf::Style::Close | sf::Style::Titlebar),
    m_fontLoaded(false),
    m_hud(nullptr),
    m_state(GameState::Menu),
    m_level(1),
    m_gameSpeed(BASE_SPEED),
    m_timeLeft(GAME_DURATION),
    m_totalTime(GAME_DURATION),
    m_invincible(false),
    m_invincibleTimer(0.f),
    m_flashTimer(0.f)
{
    m_window.setFramerateLimit(60);
    loadAssets();

    if (m_fontLoaded)
        m_hud = new HUD(m_font);

    buildMenu();
}

// ── Chargement des assets ─────────────────────────────────────────────────────

void Game::loadAssets()
{
    auto& tm = TextureManager::getInstance();

    // Police
    m_fontLoaded = m_font.loadFromFile("assets/fonts/Rajdhani-Bold.ttf");
#ifdef _WIN32
    if (!m_fontLoaded) m_fontLoaded = m_font.loadFromFile("C:/Windows/Fonts/arial.ttf");
#endif
    if (!m_fontLoaded) m_fontLoaded = m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    // Textures
    tm.load("menu_bg", "menue.png");
    tm.load("player", "1.png");
    tm.load("player1", "1.png");
    tm.load("player2", "2.png");
    tm.load("player3", "3.png");
    tm.load("player4", "4.png");
    tm.load("player5", "5.png");
    tm.load("player6", "6.png");
    tm.load("player7", "7.png");
    tm.load("player8", "8.png");
    tm.load("player9", "9.png");
    tm.load("player10", "10.png");
    tm.load("player11", "11.png");
    tm.load("background1", "1erlevel1.png");
    tm.load("background2", "level2.png");
    tm.load("obstacle1", "obstacle1.png");
    tm.load("obstacle2", "obstacle2.png");
    tm.load("obstacle3", "obstacle3.png");
    tm.load("etoile", "etoile.png");
    tm.load("btn_play", "joueurs.png");
    tm.load("btn_about", "apropos.png");
    tm.load("btn_quit", "Quitter.png");
    tm.load("splash", "ma_photo.png");

    // Fond du menu
    if (tm.has("menu_bg")) {
        m_menuBg.setTexture(tm.get("menu_bg"));
        auto ts = tm.get("menu_bg").getSize();
        m_menuBg.setScale((float)WINDOW_WIDTH / ts.x, (float)WINDOW_HEIGHT / ts.y);
        m_menuBgLoaded = true;
    }
}

// ── Construction du menu ──────────────────────────────────────────────────────

Game::Button Game::makeButton(sf::Vector2f pos, sf::Vector2f size,
    const std::string& label, const std::string& spriteId)
{
    Button btn;
    btn.shape.setSize(size);
    btn.shape.setPosition(pos);
    btn.shape.setFillColor(sf::Color(190, 160, 120, 255));
    btn.shape.setOutlineThickness(2.f);
    btn.shape.setOutlineColor(sf::Color(190, 160, 120, 255));

    if (m_fontLoaded) {
        btn.label.setFont(m_font);
        btn.label.setString(label);
        btn.label.setCharacterSize(24);
        btn.label.setFillColor(sf::Color::White);
        auto lb = btn.label.getLocalBounds();
        btn.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        btn.label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    }

    auto& tm = TextureManager::getInstance();
    if (!spriteId.empty() && tm.has(spriteId)) {
        btn.sprite.setTexture(tm.get(spriteId));
        auto ts = tm.get(spriteId).getSize();
        btn.sprite.setScale(size.x / ts.x, size.y / ts.y);
        btn.sprite.setPosition(pos);
        btn.hasSprite = true;
    }

    return btn;
}

void Game::buildMenu()
{
    float cx = WINDOW_WIDTH / 2.f;
    m_btnPlay = makeButton({ cx - 150.f, 290.f }, { 300.f, 70.f }, "Jouer", "btn_play");
    m_btnAbout = makeButton({ cx - 150.f, 390.f }, { 300.f, 70.f }, "A propos", "btn_about");
    m_btnQuit = makeButton({ cx - 150.f, 490.f }, { 300.f, 70.f }, "Quitter", "btn_quit");
    m_btnBack = makeButton({ 30.f, 640.f }, { 200.f, 50.f }, "< Retour");

    if (m_fontLoaded) {
        m_aboutText.setFont(m_font);
        m_aboutText.setCharacterSize(19);
        m_aboutText.setFillColor(sf::Color(90, 50, 20, 255));
        m_aboutText.setPosition(80.f, 60.f);
        m_aboutText.setString(
            "       Trayl Dades <=> Kech\n\n"
			"La course a pied est divisee en deux niveaux.\n"
            "Le premier niveau se deroule dans le Ouade de Dades, \n"
            "et les qualifies participent a une course a Marrakech..\n\n"
            "CONTROLES\n"
            "  [ESPACE] / [HAUT]  Sauter\n"
            "  [BAS] / [S]        Se baisser\n"
            "  [P]                Pause\n\n"
            "OBJECTIF\n"
            "  Survivre au temps imparti avec 3 vies.\n"
            "  Deux niveaux : Dades puis Marrakech.\n\n"
            "Fait avec coeur au Maroc - 23/04/2026\n"
            "  EL ABDELLAOUI M'HAMED  2331944\n"
            "  RANIA LAHLOU            2415087");
    }
}

// ── Démarrer un niveau ────────────────────────────────────────────────────────

void Game::startLevel(int level)
{
    m_level = level;
    m_gameSpeed = BASE_SPEED + (level - 1) * 80.f;
    m_timeLeft = (level == 1) ? GAME_DURATION : LVL2_DURATION;
    m_totalTime = m_timeLeft;
    m_invincible = false;
    m_flashTimer = 0.f;

    m_player = Player();
    m_player.setLevel(level);
    m_obstacles.reset();

    std::string bgId = (level == 2) ? "background2" : "background1";
    m_background.setTexture(bgId);

    m_state = GameState::Playing;
}

// ── Boucle principale ─────────────────────────────────────────────────────────

void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);

        handleEvents();
        update(dt);
        render();
    }
    delete m_hud;
}

// ── Événements ────────────────────────────────────────────────────────────────

void Game::handleEvents()
{
    sf::Event e;
    while (m_window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            m_window.close();

        if (m_state == GameState::Menu || m_state == GameState::About) {
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                handleMenuClick(sf::Vector2f((float)e.mouseButton.x, (float)e.mouseButton.y));
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                m_state = GameState::Menu;
        }
        else {
            handleGameKeys(e);
        }
    }
}

void Game::handleMenuClick(sf::Vector2f pos)
{
    if (m_state == GameState::About) {
        if (m_btnBack.contains(pos)) m_state = GameState::Menu;
        return;
    }
    if (m_btnPlay.contains(pos))  startLevel(1);
    if (m_btnAbout.contains(pos)) m_state = GameState::About;
    if (m_btnQuit.contains(pos))  m_window.close();
}

void Game::handleGameKeys(const sf::Event& e)
{
    if (e.type != sf::Event::KeyPressed) return;

    switch (e.key.code) {
    case sf::Keyboard::P:
        if (m_state == GameState::Playing) m_state = GameState::Paused;
        else if (m_state == GameState::Paused) m_state = GameState::Playing;
        break;
    case sf::Keyboard::Escape:
        if (m_state == GameState::Playing || m_state == GameState::Paused)
            m_state = GameState::Menu;
        break;
    case sf::Keyboard::R:
        if (m_state == GameState::Win && m_level == 1) startLevel(2);
        else if (m_state == GameState::GameOver)       startLevel(m_level);
        break;
    case sf::Keyboard::Return:
        if (m_state == GameState::Win || m_state == GameState::GameOver)
            m_state = GameState::Menu;
        break;
    case sf::Keyboard::Q:
        if (m_state == GameState::Win && m_level == 1)
            m_state = GameState::Menu;
        break;
    default: break;
    }
}

// ── Mise à jour ───────────────────────────────────────────────────────────────

void Game::update(float dt)
{
    if (m_state == GameState::Splash) {
        m_splashTimer += dt;
        if (m_splashTimer >= 3.f)   // 3 secondes
            m_state = GameState::Menu;
        return;
    }
    if (m_state != GameState::Playing) return;

    m_timeLeft -= dt;
    if (m_timeLeft <= 0.f) {
        m_timeLeft = 0.f;
        m_state = GameState::Win;
        return;
    }

    m_gameSpeed = std::min(m_gameSpeed + SPEED_INCREMENT * dt, MAX_SPEED + (m_level - 1) * 100.f);

    if (m_invincible) {
        m_invincibleTimer -= dt;
        m_flashTimer += dt;
        if (m_invincibleTimer <= 0.f) m_invincible = false;
    }

    m_player.update(dt);
    m_background.update(dt, m_gameSpeed);
    m_obstacles.update(dt, m_gameSpeed);

    m_player.addScore((int)(dt * 100));

    if (!m_invincible) {
        Obstacle* hit = const_cast<Obstacle*>(m_obstacles.getCollidingObstacle(m_player.getBounds()));
        if (hit) processCollision(hit);
    }

    if (m_player.getLives() <= 0)
        m_state = GameState::GameOver;

    if (m_hud)
        m_hud->update(m_player.getScore(), m_player.getLives(),
            m_timeLeft, m_totalTime, m_gameSpeed, m_level);
}

void Game::processCollision(Obstacle* obs)
{
    if (obs->wasHit()) return;
    obs->markHit();

    switch (obs->getType()) {
    case ObstacleType::Ground1: m_player.onObstacle1Hit(m_gameSpeed); break;
    case ObstacleType::Ground2: m_player.onObstacle2Hit();            break;
    case ObstacleType::Aerial:  m_player.onObstacle3Hit();            break;
    }
    m_invincible = true;
    m_invincibleTimer = 1.5f;
}

// ── Rendu ─────────────────────────────────────────────────────────────────────

void Game::render()
{
    m_window.clear(sf::Color(20, 20, 40));

    switch (m_state) {
    case GameState::Menu:    drawMenu();  break;
    case GameState::About:   drawAbout(); break;
    case GameState::Playing:
    case GameState::Paused:  drawGame();  break;
    case GameState::GameOver:
        drawGame();
        drawOverlay("GAME OVER", "R: Rejouer   ENTRER: Menu");
        break;
    case GameState::Win:
        drawGame();
        drawWinScreen();
        break;
    case GameState::Splash:
        sf::Sprite splash;
        splash.setTexture(TextureManager::getInstance().get("splash"));
        splash.setScale((float)WINDOW_WIDTH / splash.getTexture()->getSize().x,
            (float)WINDOW_HEIGHT / splash.getTexture()->getSize().y);
        m_window.draw(splash);
        break;
    }

    m_window.display();
}

void Game::drawMenu()
{
    if (m_menuBgLoaded)
        m_window.draw(m_menuBg);
    else {
        sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        bg.setFillColor(sf::Color(15, 20, 45));
        m_window.draw(bg);
    }

    auto drawBtn = [&](const Button& btn) {
        m_window.draw(btn.shape);
        if (btn.hasSprite) m_window.draw(btn.sprite);
        else if (m_fontLoaded) m_window.draw(btn.label);
        };

    drawBtn(m_btnPlay);
    drawBtn(m_btnAbout);
    drawBtn(m_btnQuit);
}

void Game::drawAbout()
{
    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(sf::Color(240, 220, 180, 255));//(10, 15, 35)
    m_window.draw(bg);
    m_window.draw(m_aboutText);
    m_window.draw(m_btnBack.shape);
    if (m_fontLoaded) m_window.draw(m_btnBack.label);
}

void Game::drawGame()
{
    m_background.draw(m_window);

    bool showPlayer = true;
    if (m_invincible) showPlayer = ((int)(m_flashTimer * 8.f) % 2 == 0);
    if (showPlayer) m_player.draw(m_window);

    m_obstacles.draw(m_window);
    if (m_hud) m_hud->draw(m_window);

    if (m_state == GameState::Paused)
        drawOverlay("PAUSE", "P: Reprendre   ECHAP: Menu");
}

void Game::drawOverlay(const std::string& title, const std::string& sub)
{
    sf::RectangleShape dim(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    m_window.draw(dim);

    if (!m_fontLoaded) return;

    sf::Text t;
    t.setFont(m_font);
    t.setString(title);
    t.setCharacterSize(72);
    t.setFillColor(sf::Color(255, 220, 50));
    t.setStyle(sf::Text::Bold);
    auto lb = t.getLocalBounds();
    t.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    t.setPosition(WINDOW_WIDTH / 2.f, 280.f);
    m_window.draw(t);

    sf::Text s;
    s.setFont(m_font);
    s.setString(sub);
    s.setCharacterSize(24);
    s.setFillColor(sf::Color(200, 200, 200));
    auto slb = s.getLocalBounds();
    s.setOrigin(slb.left + slb.width / 2.f, slb.top + slb.height / 2.f);
    s.setPosition(WINDOW_WIDTH / 2.f, 380.f);
    m_window.draw(s);
}

void Game::drawWinScreen()
{
    sf::RectangleShape dim(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    m_window.draw(dim);

    if (!m_fontLoaded) return;

    std::string msg = (m_level == 1) ?
        "Felicitations ! Niveau 1 termine." :
        "Felicitations ! Vous avez gagne !";

    sf::Text titleText;
    titleText.setFont(m_font);
    titleText.setString(msg);
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color(255, 220, 50));
    titleText.setStyle(sf::Text::Bold);
    auto lb = titleText.getLocalBounds();
    titleText.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    titleText.setPosition(WINDOW_WIDTH / 2.f, 200.f);
    m_window.draw(titleText);

    sf::Text scoreText;
    scoreText.setFont(m_font);
    scoreText.setString("Score: " + std::to_string(m_player.getScore()));
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    auto slb = scoreText.getLocalBounds();
    scoreText.setOrigin(slb.left + slb.width / 2.f, slb.top + slb.height / 2.f);
    scoreText.setPosition(WINDOW_WIDTH / 2.f, 300.f);
    m_window.draw(scoreText);

    // Étoiles selon les vies restantes
    auto& tm = TextureManager::getInstance();
    if (tm.has("etoile")) {
        sf::Sprite star;
        star.setTexture(tm.get("etoile"));
        float scale = 50.f / tm.get("etoile").getSize().x;
        star.setScale(scale, scale);
        float startX = WINDOW_WIDTH / 2.f - (m_player.getLives() * 60.f) / 2.f;
        for (int i = 0; i < m_player.getLives(); i++) {
            star.setPosition(startX + i * 60.f, 400.f);
            m_window.draw(star);
        }
    }

    std::string instr = (m_level == 1) ? "R: Continuer (Niv.2)   Q: Menu" : "ENTRER: Menu";
    sf::Text instrText;
    instrText.setFont(m_font);
    instrText.setString(instr);
    instrText.setCharacterSize(24);
    instrText.setFillColor(sf::Color(200, 200, 200));
    auto ilb = instrText.getLocalBounds();
    instrText.setOrigin(ilb.left + ilb.width / 2.f, ilb.top + ilb.height / 2.f);
    instrText.setPosition(WINDOW_WIDTH / 2.f, 500.f);
    m_window.draw(instrText);
}