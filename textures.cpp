#include "textures.h"


std::unordered_map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(std::string textureName) {
    auto result = textures.find(textureName);
    if (result == textures.end()) {
        // Texture does not already exist in the map, go get it!
        sf::Texture newTexture;
        newTexture.loadFromFile("images/" + textureName + ".png");
        textures[textureName] = newTexture;
        return textures[textureName];
        } else {
        // Texture already exists, return it!
        return result->second;
        }
    }