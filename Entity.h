//
// Created by benmo on 9/28/2020.
//

#ifndef SFML_TEMPLATE_ENTITY_H
#define SFML_TEMPLATE_ENTITY_H

class Entity : public MySprite {
public:
    Entity(int health, int points, const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) : MySprite(texture, scale, xPos, yPos, velocity, direction) {
        this->health = health;
        this->points = points;
    }

    int getHealth() const {
        return health;
    }

    void setHealth(int health) {
        this->health = health;
    }

    void hit() {};

    string getType() {
        return type;
    }

    int getPoints() const {
        return points;
    }
protected:
    int health, points;

    string type = "entity";
};


#endif //SFML_TEMPLATE_ENTITY_H
