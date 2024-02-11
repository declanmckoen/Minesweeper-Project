#include "digits.h"


DigitsManager::DigitsManager() {
    TextureManager textureManager2;
    sf::Texture& digitsTexture = textureManager2.getTexture("digits");

    for (int i = 0; i < 10; ++i) {
        sf::Sprite digitsSprite;
        digitsSprite.setTexture(digitsTexture);
        digitsSprite.setTextureRect(sf::IntRect(21 * i, 0,
                                                21,digitsTexture.getSize().y));
        digits.emplace(i, digitsSprite);
    }
    sf::Sprite hyphenSprite;
    hyphenSprite.setTexture(digitsTexture);
    hyphenSprite.setTextureRect(sf::IntRect(digitsTexture.getSize().x - 21,
                                            0, 21, digitsTexture.getSize().y));
    digits.emplace('-', hyphenSprite);
}

sf::Sprite DigitsManager::GetDigit(char digit) {
    return digits.at(digit);
}
