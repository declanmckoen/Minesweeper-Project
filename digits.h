#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "textures.h"
#pragma once

class DigitsManager {
    std::unordered_map<char, sf::Sprite> digits;
public:
    DigitsManager();
    sf::Sprite GetDigit(char digit);
};
