#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "textures.h"
#include "tiles.h"
#include "variables.h"
#pragma once


class Board {
    int rows;
    int cols;
    int numOfMines;
    int numOfFlags;
    sf::Texture texture;
    std::vector<std::vector<Tile*>> board;
    TextureManager textureManager1;
    //std::random_device dev;
public:
    Board(short rows, short cols, short numOfMines, sf::Texture texture);
    ~Board();
    std::vector<std::vector<Tile*>>& GetBoard();
    void AssignMines();
    int GetFlags();
    void IncrementMines();
    void DecrementMines();
    int CheckNumOfFlags();
    void IncrementFlags();
    void DecrementFlags();
    void CheckAdjacentTiles(Tile& tile, int row, int col);
    void RecursiveReveal(int row, int col);
    void RevealAllMines();
    void DrawBoard(sf::RenderWindow& window);
    void Update(short rows, short cols, short numOfMines, sf::Texture texture);
    void Debug(bool condition);
    void TogglePause(bool condition);
    bool CheckIfWin();
};
