#include <iostream>
#include <SFML/Graphics.hpp>
#include "windows.h"


sf::RenderWindow welcomeWindow;
sf::RenderWindow gameWindow;
sf::RenderWindow leaderboardWindow;
int main() {
    WelcomeWindow(welcomeWindow);
    if (!EXIT)
        GameWindow(gameWindow);
    if (!EXIT)
        LeaderboardWindow(leaderboardWindow);
}
