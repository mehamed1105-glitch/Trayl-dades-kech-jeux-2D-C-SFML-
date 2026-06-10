#include "HUD.h"
#include "Constants.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

HUD::HUD(const sf::Font& font) : m_font(font)
{
    m_hudBg.setSize(sf::Vector2f((float)WINDOW_WIDTH, 50.f));
    m_hudBg.setFillColor(sf::Color(0, 0, 0, 160));
    m_hudBg.setPosition(0.f, 0.f);

    auto setup = [&](sf::Text& t, unsigned size, sf::Color col, float x, float y) {
        t.setFont(font);
        t.setCharacterSize(size);
        t.setFillColor(col);
        t.setPosition(x, y);
        };

    setup(m_scoreText, 22, sf::Color(255, 220, 50), 20.f, 10.f);
    setup(m_livesText, 22, sf::Color(255, 80, 80), 300.f, 10.f);
    setup(m_timeText, 22, sf::Color(80, 220, 255), 600.f, 10.f);
    setup(m_speedText, 18, sf::Color(180, 180, 180), 900.f, 14.f);
    setup(m_levelText, 18, sf::Color(150, 255, 150), 1150.f, 14.f);

    m_progressBg.setSize(sf::Vector2f((float)WINDOW_WIDTH - 40.f, 16.f));
    m_progressBg.setFillColor(sf::Color(60, 60, 60, 200));
    m_progressBg.setPosition(20.f, (float)WINDOW_HEIGHT - 30.f);

    m_progressBar.setSize(sf::Vector2f(0.f, 16.f));
    m_progressBar.setFillColor(sf::Color(50, 200, 100));
    m_progressBar.setPosition(20.f, (float)WINDOW_HEIGHT - 30.f);
}

void HUD::update(int score, int lives, float timeLeft, float totalTime, float gameSpeed, int level)
{
    std::ostringstream ss;

    ss << "Score: " << score;
    m_scoreText.setString(ss.str()); ss.str("");

    ss << "Vies: " << lives;
    m_livesText.setString(ss.str()); ss.str("");

    ss << std::fixed << std::setprecision(1) << "Temps: " << timeLeft << "s";
    m_timeText.setString(ss.str()); ss.str("");

    ss << std::fixed << std::setprecision(0) << "Vitesse: " << gameSpeed;
    m_speedText.setString(ss.str()); ss.str("");

    ss << "Niv." << level;
    m_levelText.setString(ss.str());

    float ratio = 1.f - (timeLeft / totalTime);
    float maxWidth = (float)WINDOW_WIDTH - 40.f;
    float fill = std::clamp(ratio * maxWidth, 0.f, maxWidth);
    m_progressBar.setSize(sf::Vector2f(fill, 16.f));

    sf::Uint8 r = (sf::Uint8)(std::clamp(ratio * 2.f, 0.f, 1.f) * 200.f);
    sf::Uint8 g = (sf::Uint8)(std::clamp(2.f - ratio * 2.f, 0.f, 1.f) * 200.f);
    m_progressBar.setFillColor(sf::Color(r + 50, g, 50));
}

void HUD::draw(sf::RenderWindow& window) const
{
    window.draw(m_hudBg);
    window.draw(m_scoreText);
    window.draw(m_livesText);
    window.draw(m_timeText);
    window.draw(m_speedText);
    window.draw(m_levelText);
    window.draw(m_progressBg);
    window.draw(m_progressBar);
}