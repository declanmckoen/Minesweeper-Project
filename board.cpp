#include <random>
#include "board.h"


Board::Board(short rows, short cols, short numOfMines, sf::Texture texture) {
    this->rows = rows;
    this->cols = cols;
    this->numOfMines = numOfMines;
    this->numOfFlags = numOfMines;
    this->texture = texture;
    TILEWIDTH = WIDTH / static_cast<float>(COLUMNS);
    TILEHEIGHT = (HEIGHT - 100) / static_cast<float>(ROWS);

    for (int i = 0; i < rows; ++i) {
        std::vector<Tile*> subvec;
        for (int j = 0; j < cols; ++j) {
            subvec.push_back(new Tile(texture));
        }
        board.push_back(subvec);
    }
}

Board::~Board() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete board[i][j];
        }
    }
    board.clear();
}

std::vector<std::vector<Tile*>>& Board::GetBoard() {
    return board;
}

void Board::AssignMines() {
    //std::mt19937 rng(dev());
    //std::uniform_int_distribution<std::mt19937::result_type> distRows(0,rows-1);
    for (int i = 0; i < numOfMines; ++i) {
        while (true) {
            int row = rand() % rows;
            int col = rand() % cols;
            if (!board[row][col]->checkIfMine()){
                board[row][col]->setMine();
                break;
            }
        }
    }
}

int Board::GetFlags() {
    return numOfFlags;
}

void Board::IncrementMines() {
    numOfMines++;
}

void Board::DecrementMines() {
    numOfMines--;
}

int Board::CheckNumOfFlags() {
    return numOfFlags;
}

void Board::IncrementFlags() {
    numOfFlags++;
}

void Board::DecrementFlags() {
    numOfFlags--;
}

void Board::CheckAdjacentTiles(Tile& tile, int row, int col) {
    int rowParameters[2];
    int colParameters[2];
    if (row - 1 < 0) {
        rowParameters[0] = row;
        rowParameters[1] = row + 1;
    }
    if (row + 1 == ROWS) {
        rowParameters[0] = row - 1;
        rowParameters[1] = row;
    }
    if (row - 1 >= 0 && row + 1 < ROWS) {
        rowParameters[0] = row - 1;
        rowParameters[1] = row + 1;
    }

    if (col - 1 < 0) {
        colParameters[0] = col;
        colParameters[1] = col + 1;
    }
    if (col + 1 == COLUMNS) {
        colParameters[0] = col - 1;
        colParameters[1] = col;
    }
    if (col - 1 >= 0 && col + 1 < COLUMNS) {
        colParameters[0] = col - 1;
        colParameters[1] = col + 1;
    }

    if (!tile.checkIfMine()){
        for (int i = rowParameters[0]; i <= rowParameters[1]; ++i) {
            for (int j = colParameters[0]; j <= colParameters[1]; ++j) {
                if (i == row && j == col) continue;
                tile.AddAdjacentTile(board[i][j]);
            }
        }
        tile.findAdjacentMines();
    }
}

void Board::RecursiveReveal(int row, int col) {
    sf::Texture& revealedTexture = textureManager1.getTexture("tile_revealed");
    if (board[row][col]->GetAdjacentMines() > 0) {
        board[row][col]->changeTexture(revealedTexture);
        board[row][col]->Reveal();
        return;
    }
    if (!board[row][col]->CheckIfRevealed() && !board[row][col]->checkIfFlagged()) {
        board[row][col]->changeTexture(revealedTexture);
        board[row][col]->Reveal();
    }
    if (row-1 >= 0 && !board[row-1][col]->CheckIfRevealed() && !board[row-1][col]->checkIfFlagged())
        RecursiveReveal(row-1, col);
    if (row+1 < ROWS && !board[row+1][col]->CheckIfRevealed() && !board[row+1][col]->checkIfFlagged())
        RecursiveReveal(row+1, col);
    if (col-1 >= 0 && !board[row][col-1]->CheckIfRevealed() && !board[row][col-1]->checkIfFlagged())
        RecursiveReveal(row, col-1);
    if (col+1 < COLUMNS && !board[row][col+1]->CheckIfRevealed() && !board[row][col+1]->checkIfFlagged())
        RecursiveReveal(row, col+1);
}

void Board::RevealAllMines() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j]->checkIfMine())
                board[i][j]->Reveal();
        }
    }
}

void Board::DrawBoard(sf::RenderWindow& window) {
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            board[i-1][j-1]->DrawTile(window, TILEWIDTH * j - TILEWIDTH / 2.0f, TILEHEIGHT * i - TILEHEIGHT / 2.0f);
            board[i-1][j-1]->SetRowCol(i-1, j-1);
        }
    }
}

void Board::Update(short rows, short cols, short numOfMines, sf::Texture texture) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            delete board[i][j];
        }
    }
    board.clear();

    this->rows = rows;
    this->cols = cols;
    this->numOfMines = numOfMines;
    this->numOfFlags = numOfMines;
    this->texture = texture;

    for (int i = 0; i < rows; ++i) {
        std::vector<Tile*> subvec;
        for (int j = 0; j < cols; ++j) {
            subvec.push_back(new Tile(texture));
        }
        board.push_back(subvec);
    }
}

void Board::Debug(bool condition) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j]->checkIfMine())
                board[i][j]->SetDebug(condition);
        }
    }
}

void Board::TogglePause(bool condition) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j]->SetPaused(condition);
        }
    }
}

bool Board::CheckIfWin() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!board[i][j]->checkIfMine() && !board[i][j]->CheckIfRevealed()) {
                return false;
            }
        }
    }
    return true;
}
