#include <iostream>                             // for standard input/output
#include <random>
using namespace std;                            // using the standard namespace

#include <SFML/Graphics.hpp>                    // include the SFML Graphics Library
#include <SFML/Audio.hpp>
#include "Collision.h"
using namespace sf;                             // using the sf namespace

#include "MySprite.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Menu.h"
#include "Game.h"

int main() {
    bool play = true;

    while (play) {
        Menu menu;
        if (menu.result == EXIT_SUCCESS) {
            Game game;
            if (game.result == 1) play = false;
        }
        else play = false;
    }

    return 0;
}