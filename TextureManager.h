#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
    static std::map<std::string, sf::Texture> textures;
public:
    static sf::Texture& get(const std::string& path);
};