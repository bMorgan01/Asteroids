#include <iostream>                             // for standard input/output
#include <random>
#include <chrono>
using namespace std;                            // using the standard namespace

#include <SFML/Graphics.hpp>                    // include the SFML Graphics Library
#include <SFML/Audio.hpp>
#include "Collision.h"
using namespace sf;                             // using the sf namespace

#include "MySprite.h"
#include "Rider.h"
#include "Mount.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Menu.h"
#include "Game.h"

int main() {
    bool play = true;
    Time mTime = Time::Zero;
    int loop = 0;

    while (play) {
        Menu menu(loop, mTime);
        if (menu.result == EXIT_SUCCESS) {
            Game game(menu.musicPicked, menu.mTime);
            if (game.result == 1) play = false;

            loop = game.loop;
            mTime = game.mTime;
        }
        else play = false;
    }

    return 0;
}