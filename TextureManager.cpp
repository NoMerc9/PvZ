#include "TextureManager.h"
#include <iostream>

std::map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::get(const std::string& path) {
    auto it = textures.find(path);
    if (it == textures.end()) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "Не удалось загрузить текстуру: " << path << "\n";
        }
        textures[path] = std::move(tex);
        return textures[path];
    }
    return it->second;
}