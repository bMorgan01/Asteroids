//
// Created by benmo on 9/28/2020.
//

#ifndef SFML_TEMPLATE_ASTEROID_H
#define SFML_TEMPLATE_ASTEROID_H

#include "Entity.h"

class Asteroid : public Entity {
public:
    Asteroid(int health, int points, const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) : Entity(health, points, texture, scale, xPos, yPos, velocity, direction) {
        size = health;
        type = "asteroid";
    }

    int getSize() const {
        return size;
    }

    void hit(vector<MySprite*> &animations, Sound &sound, Texture &explosion, default_random_engine &gen) {
        uniform_int_distribution<int> angle(0, 359);
        animations.push_back(new MySprite(explosion, getSize()*65, getXPos(), getYPos(), 0, angle(gen)));
        animations[animations.size() - 1]->makeAnimated(5, 5, 0.01,23);

        sound.setVolume(100.0/((4-size)/1.5));
        sound.play();
    }

private:
    int size;
};


#endif //SFML_TEMPLATE_ASTEROID_H
