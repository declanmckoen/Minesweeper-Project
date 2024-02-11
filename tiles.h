#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "textures.h"
#pragma once


class Tile {
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<Tile*> adjacentTiles;
    int adjacentMines;
    bool mine;
    bool revealed;
    bool flagged;
    bool debug;
    bool paused;
    TextureManager textureManager;
    std::unordered_map<int, std::string> numbers;
public:
    int row;
    int col;
    Tile(sf::Texture texture);
    bool checkIfMine() const;
    bool checkIfFlagged() const;
    void setFlagStatus(bool condition);
    bool CheckIfDebug() const;
    void SetDebug(bool condition);
    bool CheckIfPaused() const;
    void SetPaused(bool condition);
    void setMine();
    void changeTexture(sf::Texture texture);
    void AddAdjacentTile(Tile* tile);
    void findAdjacentMines();
    int GetAdjacentMines() const;
    void Reveal();
    bool CheckIfRevealed() const;
    void DrawTile(sf::RenderWindow& window, float x, float y);
    void SetRowCol(int row, int col);
};
