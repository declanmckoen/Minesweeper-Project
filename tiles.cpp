#include <iostream>
#include <vector>
#include "tiles.h"
using namespace std;


Tile::Tile(sf::Texture texture) {
    this->texture = texture;
    this->adjacentMines = 0;
    this->mine = false;
    this->revealed = false;
    this->flagged = false;
    this->debug = false;
    this->paused = false;
    numbers.emplace(1, "number_1");
    numbers.emplace(2, "number_2");
    numbers.emplace(3, "number_3");
    numbers.emplace(4, "number_4");
    numbers.emplace(5, "number_5");
    numbers.emplace(6, "number_6");
    numbers.emplace(7, "number_7");
    numbers.emplace(8, "number_8");
    numbers.emplace(9, "number_9");
    sprite.setTexture(this->texture);
    sprite.setOrigin(this->texture.getSize().x/2.0f, this->texture.getSize().y/2.0f);
}

bool Tile::checkIfMine() const {
    return mine;
}

bool Tile::checkIfFlagged() const {
    return flagged && !revealed;
}

void Tile::setFlagStatus(bool condition) {
    flagged = condition;
}

bool Tile::CheckIfDebug() const {
    return debug;
}

void Tile::SetDebug(bool condition) {
    debug = condition;
}

bool Tile::CheckIfPaused() const {
    return paused;
}

void Tile::SetPaused(bool condition) {
    paused = condition;
}

void Tile::setMine() {
    mine = true;
}

void Tile::changeTexture(sf::Texture texture) {
    this->texture = texture;
    sprite.setTexture(this->texture);
    sprite.setOrigin(this->texture.getSize().x/2.0f, this->texture.getSize().y/2.0f);
}

void Tile::AddAdjacentTile(Tile* tile) {
    adjacentTiles.push_back(tile);
}

void Tile::findAdjacentMines() {
    for (Tile* tile : adjacentTiles) {
        if (tile->mine)
            adjacentMines++;
    }
}

int Tile::GetAdjacentMines() const {
    return adjacentMines;
}

void Tile::Reveal() {
    revealed = true;
}

bool Tile::CheckIfRevealed() const {
    return revealed;
}

void Tile::DrawTile(sf::RenderWindow& window, float x, float y) {
    sprite.setPosition(x, y);
    window.draw(sprite);
    if (revealed && adjacentMines > 0) {
        sf::Texture numberTexture;
        try {
            numberTexture = textureManager.getTexture(numbers.at(adjacentMines));
        }
        catch (std::out_of_range& e){
            std::cout << "HERE" << std::endl;
        }
        sf::Sprite numberSprite;
        numberSprite.setTexture(numberTexture);
        numberSprite.setOrigin(numberTexture.getSize().x/2.0f, numberTexture.getSize().y/2.0f);
        numberSprite.setPosition(x, y);
        window.draw(numberSprite);
    }
    if (mine && revealed) {
        sf::Texture mineTexture;
        mineTexture.loadFromFile("images/mine.png");
        if (!mineTexture.loadFromFile("images/mine.png"))
            std::cout << "Not open!" << std::endl;
        sf::Sprite mineSprite;
        mineSprite.setTexture(mineTexture);
        mineSprite.setOrigin(mineTexture.getSize().x/2.0f, mineTexture.getSize().y/2.0f);
        mineSprite.setPosition(x, y);
        window.draw(mineSprite);
    }
    if (flagged && !revealed) {
        sf::Texture flagTexture;
        flagTexture.loadFromFile("images/flag.png");
        if (!flagTexture.loadFromFile("images/flag.png"))
            std::cout << "Not open!" << std::endl;
        sf::Sprite flagSprite;
        flagSprite.setTexture(flagTexture);
        flagSprite.setOrigin(flagTexture.getSize().x/2.0f, flagTexture.getSize().y/2.0f);
        flagSprite.setPosition(x, y);
        window.draw(flagSprite);
    }
    if (debug) {
        sf::Texture mineTexture;
        mineTexture.loadFromFile("images/mine.png");
        if (!mineTexture.loadFromFile("images/mine.png"))
            std::cout << "Not open!" << std::endl;
        sf::Sprite mineSprite;
        mineSprite.setTexture(mineTexture);
        mineSprite.setOrigin(mineTexture.getSize().x/2.0f, mineTexture.getSize().y/2.0f);
        mineSprite.setPosition(x, y);
        window.draw(mineSprite);
    }
    if (paused) {
        sf::Texture revealedTexture;
        revealedTexture.loadFromFile("images/tile_revealed.png");
        sf::Sprite revealedSprite;
        revealedSprite.setTexture(revealedTexture);
        revealedSprite.setOrigin(revealedTexture.getSize().x/2.0f, revealedTexture.getSize().y/2.0f);
        revealedSprite.setPosition(x, y);
        window.draw(revealedSprite);
    }
}

void Tile::SetRowCol(int row, int col) {
    this->row = row;
    this->col = col;
}


