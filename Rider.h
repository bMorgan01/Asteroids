//
// Created by benmo on 10/13/2020.
//

#ifndef SFML_TEMPLATE_RIDER_H
#define SFML_TEMPLATE_RIDER_H


class Rider : public MySprite {
protected:
    bool followsDirection = false, followsPosition = true;
public:
    Rider(const sf::Texture &texture, float scale) : MySprite(texture, scale) {}

    bool doesFollowDirection() const {
        return followsDirection;
    }

    bool doesFollowPosition() const {
        return followsPosition;
    }
};


#endif //SFML_TEMPLATE_RIDER_H
