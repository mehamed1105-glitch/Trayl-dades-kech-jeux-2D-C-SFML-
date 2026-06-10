#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
public:
    static TextureManager& getInstance();

    bool load(const std::string& id, const std::string& filepath);
    const sf::Texture& get(const std::string& id) const;
    bool has(const std::string& id) const;

private:
    TextureManager() = default;
    std::map<std::string, sf::Texture> m_textures;
};
