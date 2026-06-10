#include "TextureManager.h"
#include <stdexcept>

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

bool TextureManager::load(const std::string& id, const std::string& filepath)
{
    sf::Texture tex;
    if (!tex.loadFromFile(filepath))
        return false;
    m_textures[id] = std::move(tex);
    return true;
}

const sf::Texture& TextureManager::get(const std::string& id) const
{
    auto it = m_textures.find(id);
    if (it == m_textures.end())
        throw std::runtime_error("Texture not found: " + id);
    return it->second;
}

bool TextureManager::has(const std::string& id) const
{
    return m_textures.count(id) > 0;
}