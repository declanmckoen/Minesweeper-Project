#include <iostream>
#include <chrono>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "variables.h"
#include "general.h"
#include "textures.h"
#include "board.h"
#include "digits.h"
#pragma once


void WelcomeWindow(sf::RenderWindow& window);
void GameWindow(sf::RenderWindow& window);
int LeaderboardWindow(sf::RenderWindow& window);


void WelcomeWindow(sf::RenderWindow& window) {
    ReadWindowConfig();
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper", sf::Style::Close);

    CreateText(WELCOMETEXT, "WELCOME TO MINESWEEPER!", 24, "white", true, true);
    SetText(WELCOMETEXT, WIDTH/2, HEIGHT/2 - 150);

    CreateText(NAMEINPUTTEXT, "Enter your name:", 20, "white");
    SetText(NAMEINPUTTEXT, WIDTH/2, HEIGHT/2 - 75);

    GetUserName(window);
}

void GameWindow(sf::RenderWindow& window) {
    TextureManager textureManager;
    sf::Texture& hiddenTexture = textureManager.getTexture("tile_hidden");
    sf::Texture& revealedTexture = textureManager.getTexture("tile_revealed");
    sf::Texture& happyTexture = textureManager.getTexture("face_happy");
    sf::Texture& winTexture = textureManager.getTexture("face_win");
    sf::Texture& sadTexture = textureManager.getTexture("face_lose");
    sf::Texture& mineTexture = textureManager.getTexture("mine");
    sf::Texture& debugTexture = textureManager.getTexture("debug");
    sf::Texture& playTexture = textureManager.getTexture("play");
    sf::Texture& pauseTexture = textureManager.getTexture("pause");
    sf::Texture& leaderboardTexture = textureManager.getTexture("leaderboard");

    Board board(ROWS, COLUMNS, MINES, hiddenTexture);
    board.AssignMines();
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            board.CheckAdjacentTiles(*board.GetBoard()[i][j], i, j);
        }
    }

    window.create(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper", sf::Style::Close);

    sf::Sprite happySprite;
    SetSprite(happySprite, happyTexture, (COLUMNS / 2 * 32) - 32, 32 * (ROWS + 0.5f));

    sf::Sprite winSprite;
    SetSprite(winSprite, winTexture, (COLUMNS / 2 * 32) - 32, 32 * (ROWS + 0.5F));

    sf::Sprite sadSprite;
    SetSprite(sadSprite, sadTexture, (COLUMNS / 2 * 32) - 32, 32 * (ROWS + 0.5f));

    sf::Sprite debugSprite;
    SetSprite(debugSprite, debugTexture, (COLUMNS * 32) - 304, 32 * (ROWS + 0.5f));

    sf::Sprite playSprite;
    SetSprite(playSprite, playTexture, (COLUMNS * 32) - 240, 32 * (ROWS + 0.5f));

    sf::Sprite pauseSprite;
    SetSprite(pauseSprite, pauseTexture, (COLUMNS * 32) - 240, 32 * (ROWS + 0.5f));

    sf::Sprite leaderboardSprite;
    SetSprite(leaderboardSprite, leaderboardTexture, (COLUMNS * 32) - 176, 32 * (ROWS + 0.5f));


    DigitsManager digitsManager;
    int count = 0;
    int time = 0;
    int tempTime = 0;
    int displacedTime = 0;
    bool gameLose = false;
    bool gameWin = false;
    bool debug = false;
    bool paused = false;
    bool leaderboard = false;
    bool leaderboardOpen = false;
    auto start = std::chrono::high_resolution_clock::now();
    while (window.isOpen()) {
        sf::Event event;

        auto current = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);
        if (!gameWin && !gameLose && !paused && !leaderboard) {
            time = elapsed.count() - displacedTime;
        }
        else {
            tempTime = elapsed.count();
            displacedTime = static_cast<int>(tempTime - time);
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                EXIT = true;
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    if ((happySprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse)) && !gameLose) ||
                            sadSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse)) ||
                            winSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse))) {
                        board.Update(ROWS, COLUMNS, MINES, hiddenTexture);
                        board.AssignMines();
                        for (int i = 0; i < ROWS; ++i) {
                            for (int j = 0; j < COLUMNS; ++j) {
                                board.CheckAdjacentTiles(*board.GetBoard()[i][j], i, j);
                            }
                        }
                        debug = false;
                        gameWin = false;
                        gameLose = false;
                        gameWin = false;
                        paused = false;
                        leaderboard = false;
                        leaderboardOpen = false;
                        displacedTime = 0;
                        count = 0;
                        start = std::chrono::high_resolution_clock::now();
                    }
                    if (debugSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse)) && !paused &&
                    !(gameWin) && !(gameLose)) {
                        debug = !debug;
                        board.Debug(debug);
                    }
                    if ((pauseSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse)) ||
                            playSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse))) &&
                            !(gameLose) && !(gameWin)) {
                        paused = !paused;
                        board.TogglePause(paused);
                    }
                    if (leaderboardSprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse))) {
                        //sf::RenderWindow tempLeaderboardWindow;
                        //LeaderboardWindow(tempLeaderboardWindow);
                        leaderboard = true;
                        board.TogglePause(leaderboard);
                    }
                    if ((mouse.y < HEIGHT - 100) && (!paused) && (!gameLose) && !(gameWin)) {
                        int row = mouse.y / TILEWIDTH;
                        int col = mouse.x / TILEHEIGHT;
                        if (!board.GetBoard()[row][col]->checkIfFlagged()) {
                            if (board.GetBoard()[row][col]->checkIfMine()) {
                                gameLose = true;
                                board.GetBoard()[row][col]->Reveal();
                                board.RevealAllMines();
                            }
                            else {
                                board.GetBoard()[row][col]->changeTexture(revealedTexture);
                                board.GetBoard()[row][col]->Reveal();
                                if (board.GetBoard()[row][col]->GetAdjacentMines() == 0) {
                                    board.RecursiveReveal(row, col);
                                }
                            }
                        }
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    if ((mouse.y < HEIGHT - 100) && (!paused) && !(gameLose) && !(gameWin)) {
                        int row = mouse.y / TILEWIDTH;
                        int col = mouse.x / TILEWIDTH;
                        if (board.GetBoard()[row][col]->checkIfFlagged()) {
                            board.IncrementFlags();
                            board.GetBoard()[row][col]->setFlagStatus(false);
                        }
                        else {
                            board.DecrementFlags();
                            board.GetBoard()[row][col]->setFlagStatus(true);
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        if (board.GetFlags() > 99) {
            sf::Sprite variable1 = digitsManager.GetDigit(board.GetFlags() / 100);
            variable1.setPosition(33, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite variable2 = digitsManager.GetDigit((board.GetFlags() / 10) % 10);
            variable2.setPosition(54, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite variable3 = digitsManager.GetDigit((board.GetFlags() % 10));
            variable3.setPosition(75, 32 * (ROWS + 0.5f) + 16);
            window.draw(variable1);
            window.draw(variable2);
            window.draw(variable3);
        }
        else if (board.GetFlags() >= 0) {
            sf::Sprite zero1 = digitsManager.GetDigit(0);
            zero1.setPosition(33, 32 * (ROWS + 0.5f) + 16);
            window.draw(zero1);
            if (board.GetFlags() < 10) {
                sf::Sprite zero2 = digitsManager.GetDigit(0);
                zero2.setPosition(54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable1 = digitsManager.GetDigit(board.GetFlags());
                variable1.setPosition(75, 32 * (ROWS + 0.5f) + 16);
                window.draw(zero2);
                window.draw(variable1);
            }
            else {
                sf::Sprite variable1 = digitsManager.GetDigit(board.GetFlags() / 10);
                variable1.setPosition(54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable2 = digitsManager.GetDigit(board.GetFlags() % 10);
                variable2.setPosition(75, 32 * (ROWS + 0.5f) + 16);
                window.draw(variable1);
                window.draw(variable2);
            }
        }
        else {
            sf::Sprite negative = digitsManager.GetDigit('-');
            negative.setPosition(12, 32 * (ROWS + 0.5f) + 16);
            window.draw(negative);
            if (board.GetFlags() > -10) {
                sf::Sprite zero1 = digitsManager.GetDigit(0);
                zero1.setPosition(33, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite zero2 = digitsManager.GetDigit(0);
                zero2.setPosition(54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable1 = digitsManager.GetDigit(abs(board.GetFlags()));
                variable1.setPosition(75, 32 * (ROWS + 0.5f) + 16);
                window.draw(zero1);
                window.draw(zero2);
                window.draw(variable1);
            }
            else if (board.GetFlags() > -100) {
                sf::Sprite zero1 = digitsManager.GetDigit(0);
                zero1.setPosition(33, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable1 = digitsManager.GetDigit(abs(board.GetFlags() / 10));
                variable1.setPosition(54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable2 = digitsManager.GetDigit(abs(board.GetFlags() % 10));
                variable2.setPosition(75, 32 * (ROWS + 0.5f) + 16);
                window.draw(zero1);
                window.draw(variable1);
                window.draw(variable2);
            }
            else {
                sf::Sprite variable1 = digitsManager.GetDigit(abs(board.GetFlags() / 100));
                variable1.setPosition(33, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable2 = digitsManager.GetDigit((abs((board.GetFlags() / 10) % 10)));
                variable2.setPosition(54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable3 = digitsManager.GetDigit((abs(board.GetFlags() % 10)));
                variable3.setPosition(75, 32 * (ROWS + 0.5f) + 16);
                window.draw(variable1);
                window.draw(variable2);
                window.draw(variable3);
            }
        }

        if (!gameLose && !gameWin) {
            window.draw(happySprite);
        }

        else if (gameWin && !gameLose) {
            window.draw(winSprite);
            std::string content;
            int temp = time;
            if (temp / 60 == 0) {
                content += "00:";
                temp = temp % 60;
                if (temp < 10)
                    content += "0" + std::to_string(temp);
                else
                    content += std::to_string(temp);
            }
            else if (temp / 60 < 10) {
                content += "0" + std::to_string(temp / 60) + ":";
                temp = temp % 60;
                if (temp < 10)
                    content += "0" + std::to_string(temp);
                else
                    content += std::to_string(temp);
            }
            else {
                content += std::to_string(temp / 60) + ":";
                time = temp % 60;
                if (temp < 10)
                    content += "0" + std::to_string(temp);
                else
                    content += std::to_string(temp);
            }
            while (count == 1) {
                WriteLeaderboard("leaderboard.txt", content, time);
                sf::RenderWindow tempLeaderboardWindow;
                LeaderboardWindow(tempLeaderboardWindow);
                count++;
            }
            count++;
        }
        else {
            window.draw(sadSprite);
        }

        if (time < 10) {
            sf::Sprite zero1 = digitsManager.GetDigit(0);
            zero1.setPosition(COLUMNS * 32 - 97, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite zero2 = digitsManager.GetDigit(0);
            zero2.setPosition(COLUMNS * 32 - 76, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite zero3 = digitsManager.GetDigit(0);
            zero3.setPosition(COLUMNS * 32 - 54, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite variable = digitsManager.GetDigit(time);
            variable.setPosition(COLUMNS * 32 - 35, 32 * (ROWS + 0.5f) + 16);
            window.draw(zero1);
            window.draw(zero2);
            window.draw(zero3);
            window.draw(variable);
        }
        if (time >= 10 && time < 60) {
            sf::Sprite zero1 = digitsManager.GetDigit(0);
            zero1.setPosition(COLUMNS * 32 - 97, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite zero2 = digitsManager.GetDigit(0);
            zero2.setPosition(COLUMNS * 32 - 76, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite variable1 = digitsManager.GetDigit(time / 10);
            variable1.setPosition(COLUMNS * 32 - 54, 32 * (ROWS + 0.5f) + 16);
            sf::Sprite variable2 = digitsManager.GetDigit(time % 10);
            variable2.setPosition(COLUMNS * 32 - 35, 32 * (ROWS + 0.5f) + 16);
            window.draw(zero1);
            window.draw(zero2);
            window.draw(variable1);
            window.draw(variable2);
        }
        if (time >= 60) {
            int temp = time;
            if (temp / 60 < 10) {
                sf::Sprite zero1 = digitsManager.GetDigit(0);
                zero1.setPosition(COLUMNS * 32 - 97, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable1 = digitsManager.GetDigit(temp / 60);
                variable1.setPosition(COLUMNS * 32 - 76, 32 * (ROWS + 0.5f) + 16);
                window.draw(zero1);
                window.draw(variable1);
            }
            else {
                sf::Sprite variable1 = digitsManager.GetDigit((temp / 60) / 10);
                variable1.setPosition(COLUMNS * 32 - 97, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable2 = digitsManager.GetDigit((temp / 60) % 10);
                variable2.setPosition(COLUMNS * 32 - 76, 32 * (ROWS + 0.5f) + 16);
                window.draw(variable1);
                window.draw(variable2);
            }
            temp = temp % 60;
            if (temp < 10) {
                sf::Sprite zero1 = digitsManager.GetDigit(0);
                zero1.setPosition(COLUMNS * 32 - 54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable1 = digitsManager.GetDigit(temp);
                variable1.setPosition(COLUMNS * 32 - 35, 32 * (ROWS + 0.5f) + 16);
                window.draw(zero1);
                window.draw(variable1);
            }
            else {
                sf::Sprite variable1 = digitsManager.GetDigit(temp / 10);
                variable1.setPosition(COLUMNS * 32 - 54, 32 * (ROWS + 0.5f) + 16);
                sf::Sprite variable2 = digitsManager.GetDigit(temp % 10);
                variable2.setPosition(COLUMNS * 32 - 35, 32 * (ROWS + 0.5f) + 16);
                window.draw(variable1);
                window.draw(variable2);
            }

        }
        if (board.CheckIfWin()) {
            gameWin = true;
            while (board.CheckNumOfFlags() > 0)
                board.DecrementFlags();
            while (board.CheckNumOfFlags() < 0)
                board.IncrementFlags();
            for (int row = 0; row < board.GetBoard().size(); ++row) {
                for (int col = 0; col < board.GetBoard()[row].size(); ++col) {
                    if (board.GetBoard()[row][col]->checkIfMine())
                        board.GetBoard()[row][col]->setFlagStatus(true);
                    if (!board.GetBoard()[row][col]->checkIfMine())
                        board.GetBoard()[row][col]->setFlagStatus(false);
                }
            }
        }
        board.DrawBoard(window);
        window.draw(debugSprite);
        if (leaderboardOpen) {
            sf::RenderWindow tempLeaderboardWindow;
            displacedTime += LeaderboardWindow(tempLeaderboardWindow);
            leaderboard = false;
            leaderboardOpen = false;
            board.TogglePause(leaderboard);
        }
        if (leaderboard) {
            leaderboardOpen = true;
        }
        if (paused)
            window.draw(playSprite);
        else
            window.draw(pauseSprite);
        window.draw(leaderboardSprite);
        window.display();
    }
}

int LeaderboardWindow(sf::RenderWindow& window) {
    auto start = std::chrono::high_resolution_clock::now();
    int elapsed;

    window.create(sf::VideoMode(WIDTHLEADERBOARD, HEIGHTLEADERBOARD), "Minesweeper", sf::Style::Close);

    sf::Text leaderboardText;
    CreateText(leaderboardText, "LEADERBOARD", 20, "white", true, true);
    SetText(leaderboardText, WIDTHLEADERBOARD / 2, HEIGHTLEADERBOARD / 2 - 120);

    std::string leaderboardInfo = ReadLeaderboard("leaderboard.txt");
    sf::Text leaderboardInfoText;
    CreateText(leaderboardInfoText, leaderboardInfo, 18, "white");
    SetText(leaderboardInfoText, WIDTHLEADERBOARD / 2, HEIGHTLEADERBOARD / 2 + 20);
    while (window.isOpen()) {
        auto current = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(leaderboardText);
        window.draw(leaderboardInfoText);
        window.display();
    }

    return elapsed;
}
