#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "variables.h"
#pragma once


bool EXIT = false;
int HEIGHT;
int HEIGHTLEADERBOARD;
int WIDTH;
int WIDTHLEADERBOARD;
int ROWS;
int COLUMNS;
int MINES;
float TILEWIDTH;
float TILEHEIGHT;
std::string TEXTCOLOR;
std::string NAME;
std::string LEADERBOARDTEXT;
sf::Font FONT;
sf::Text WELCOMETEXT;
sf::Text NAMEINPUTTEXT;
sf::Text USERNAME;


void ReadWindowConfig() {
    std::ifstream inFile("config.cfg");

    if (!inFile.is_open())
        std::cout << "Not open!" << std::endl;

    std::string token;

    inFile >> token;
    int cols = stoi(token);
    COLUMNS = cols;
    WIDTH = cols * 32;
    WIDTHLEADERBOARD = cols * 16;

    inFile >> token;
    int rows = stoi(token);
    ROWS = rows;
    HEIGHT = rows * 32 + 100;
    HEIGHTLEADERBOARD = rows * 16 + 50;

    inFile >> token;
    MINES = stoi(token);
    inFile.close();
}

std::string ReadLeaderboard(std::string fileName) {
    std::string res;
    std::ifstream inFile(fileName);

    std::string line;
    int count = 0;
    while (getline(inFile, line)) {
        if (count == 5)
            break;
        int index = line.find(',');
        res += std::to_string(count+1) + ".";
        res += "\t";
        res += line.substr(0, index);
        res += "\t";
        if (line.substr(index+1, line.length()-(index+1)) == NAME)
            res += line.substr(index+1, line.length()-(index+1)) + "*";
        else
            res += line.substr(index+1, line.length()-(index+1));
        res += "\n\n";
        count++;
    }

    return res;
}

void WriteLeaderboard(std::string fileName, std::string timeString, int time) {
    std::ifstream inFile(fileName);
    std::vector<std::string> times;
    std::string line;
    std::string token;
    bool pushedBack = false;
    bool seen = false;
    while (getline(inFile, line)) {
        int comparisonTime = 0;
        int index1 = line.find(',');
        token = line.substr(0, index1);
        int index2 = token.find(':');
        comparisonTime += (stoi(token.substr(0, index2)) * 60);
        comparisonTime += stoi(token.substr(index2+1, token.length()-(index2+1)));
        if (time < comparisonTime && !seen) {
            times.push_back(timeString + "," + NAME + "\n");
            pushedBack = true;
            seen = true;
        }
        times.push_back(line + "\n");
    }
    if (!pushedBack)
        times.push_back(timeString + "," + NAME + "\n");
    inFile.close();
    std::ofstream outFile(fileName);
    for (int i = 0; i < times.size(); ++i) {
        outFile << times[i];
    }
}

void CreateText(sf::Text& text, std::string words, int textSize, std::string color, bool bold = true, bool underlined = false) {
    FONT.loadFromFile("font.ttf");
    text.setFont(FONT);
    text.setString(words);
    text.setCharacterSize(textSize);
    if (color == "yellow")
        text.setFillColor(sf::Color::Yellow);
    else
        text.setFillColor(sf::Color::White);
    if (bold && underlined)
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    else if (bold && !underlined)
        text.setStyle(sf::Text::Bold);
    else if (!bold && underlined)
        text.setStyle(sf::Text::Underlined);
}

void SetText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void SetSprite(sf::Sprite& sprite, sf::Texture& texture, float x, float y) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void EditSprite(sf::Sprite& sprite, sf::Texture& texture, sf::IntRect rect) {
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void GetUserName(sf::RenderWindow& window) {
    std::string userName = "|";

    sf::Event event;
    while (window.isOpen()){
        while (window.pollEvent(event)) {
            CreateText(USERNAME, userName, 18, "yellow");
            SetText(USERNAME, WIDTH/2, HEIGHT/2 - 45);
            if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode)) {
                    if (userName.length() <= 10) {
                        char key = event.text.unicode;
                        if (userName.length() == 1) {
                            userName.pop_back();
                            userName.push_back(toupper(key));
                            userName.push_back('|');
                        }
                        else {
                            userName.pop_back();
                            userName.push_back(tolower(key));
                            userName.push_back('|');
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (userName.length() > 1) {
                        userName.pop_back();
                        userName.pop_back();
                        userName.push_back('|');
                    }
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (userName.length() > 1) {
                        NAME = userName.substr(0, userName.length()-1);
                        window.close();
                    }
                }
            }
            if (event.type == sf::Event::Closed) {
                EXIT = true;
                window.close();
            }
            window.clear(sf::Color::Blue);
            window.draw(WELCOMETEXT);
            window.draw(NAMEINPUTTEXT);
            window.draw(USERNAME);
            window.display();
        }
    }
}
