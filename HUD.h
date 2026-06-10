#pragma once
#include <SFML/Graphics.hpp>

class HUD
{
public:
    explicit HUD(const sf::Font& font);

    void update(int score, int lives, float timeLeft, float totalTime, float gameSpeed, int level);
    void draw(sf::RenderWindow& window) const;

private:
    const sf::Font& m_font;
    sf::Text  m_scoreText, m_livesText, m_timeText, m_speedText, m_levelText;
    sf::RectangleShape m_progressBg, m_progressBar, m_hudBg;
};