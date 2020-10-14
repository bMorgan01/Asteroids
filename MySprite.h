//
// Created by benmo on 2/14/2020.
//

#ifndef SFML_TEMPLATE_SPRITE_H
#define SFML_TEMPLATE_SPRITE_H

#include <string>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

class MySprite: public sf::Sprite{
protected:
    struct Physics {
        float velocity, direction, acceleration, maxVelocity, minVelocity;
        float rotVelocity, rotAcceleration, maxRotVelocity;
        float xPos, yPos;
    };

    struct Animation {
        float frameLength;
        unsigned int currframe = 0, columns, height, width, numFrames;
        Clock clock;
        bool isAnimated = false, isPlayed = false;
        IntRect textureRect;
    };

    Physics spritePhysics{};

    Animation spriteAnimation{};

    bool toErase = false, circularHitbox = false;

    string type = "Sprite";
public:
    enum EdgeBehavior {
        IGNORE, LOOP, BOUNCE
    };

    EdgeBehavior edgeBehavior = IGNORE;

    constexpr static const double PI = 3.1415926;

    static double distance(sf::Vector2f pos1, sf::Vector2f pos2) {
        return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
    }

    MySprite() : sf::Sprite() {
        spritePhysics.velocity = 0;
        spritePhysics.xPos = 0;
        spritePhysics.yPos = 0;
        spritePhysics.direction = 0;

        setOrigin(getGlobalBounds().width/2,getGlobalBounds().height/2);
    }

    explicit MySprite(const sf::Texture &texture) : sf::Sprite(texture) {
        spritePhysics.velocity = 0;
        spritePhysics.xPos = 0;
        spritePhysics.yPos = 0;
        spritePhysics.direction = 0;

        setOrigin(getGlobalBounds().width/2,getGlobalBounds().height/2);
    }

    MySprite(const sf::Texture &texture, float scale) : sf::Sprite(texture) {
        spritePhysics.velocity = 0;
        spritePhysics.xPos = 0;
        spritePhysics.yPos = 0;
        spritePhysics.direction = 0;

        setOrigin(getGlobalBounds().width/2,getGlobalBounds().height/2);
        setScale(scale/100, scale/100);
    }

    MySprite(const sf::Texture &texture, const sf::IntRect &rectangle) : sf::Sprite(texture, rectangle) {
        spritePhysics.velocity = 0;
        spritePhysics.xPos = 0;
        spritePhysics.yPos = 0;
        spritePhysics.direction = 0;

        setOrigin(rectangle.width/(float)2.0,rectangle.height/(float)2.0);

    }

    MySprite(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) : sf::Sprite(texture) {
        spritePhysics.velocity = velocity;
        spritePhysics.xPos = xPos;
        spritePhysics.yPos = yPos;
        spritePhysics.direction = direction;

        setScale(scale/100, scale/100);
        setOrigin(getLocalBounds().width/2, getLocalBounds().height/2);
        setPosition(spritePhysics.xPos,spritePhysics.yPos);
        setRotation(direction);
    }

    MySprite(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float maxVelocity, float direction, float rotVelocity, float maxRotVelocity) : sf::Sprite(texture) {
        spritePhysics.velocity = velocity;
        spritePhysics.xPos = xPos;
        spritePhysics.yPos = yPos;
        spritePhysics.direction = direction;
        spritePhysics.rotVelocity = rotVelocity;
        spritePhysics.maxVelocity = maxVelocity;
        spritePhysics.maxRotVelocity = maxRotVelocity;

        setScale(scale/100, scale/100);
        setOrigin(getLocalBounds().width/2, getLocalBounds().height/2);
        setPosition(spritePhysics.xPos,spritePhysics.yPos);
        setRotation(direction);
    }

    MySprite(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float acceleration, float maxVelocity, float direction, float rotVelocity, float rotAcceleration, float maxRotVelocity) : sf::Sprite(texture) {
        spritePhysics.velocity = velocity;
        spritePhysics.xPos = xPos;
        spritePhysics.yPos = yPos;
        spritePhysics.direction = direction;
        spritePhysics.rotVelocity = rotVelocity;
        spritePhysics.maxVelocity = maxVelocity;
        spritePhysics.maxRotVelocity = maxRotVelocity;
        spritePhysics.acceleration = acceleration;
        spritePhysics.rotAcceleration = rotAcceleration;

        setScale(scale/100, scale/100);
        setOrigin(getLocalBounds().width/2, getLocalBounds().height/2);
        setPosition(spritePhysics.xPos,spritePhysics.yPos);
        setRotation(direction);
    }

    MySprite(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float maxVelocity, float direction) : sf::Sprite(texture) {
        spritePhysics.velocity = velocity;
        spritePhysics.xPos = xPos;
        spritePhysics.yPos = yPos;
        spritePhysics.direction = direction;
        spritePhysics.maxVelocity = maxVelocity;

        setScale(scale/100, scale/100);
        setOrigin(getLocalBounds().width/2, getLocalBounds().height/2);
        setPosition(spritePhysics.xPos,spritePhysics.yPos);
        setRotation(direction);
    }

    void makeAnimated(int rows, int columns, float length, int frames = 0) {
        spriteAnimation.height = getTexture()->getSize().y/rows;
        spriteAnimation.width = getTexture()->getSize().x/columns;
        spriteAnimation.frameLength = length;
        spriteAnimation.columns = columns;

        if (frames == 0) spriteAnimation.numFrames = rows*columns;
        else spriteAnimation.numFrames = frames;

        spriteAnimation.isAnimated = true;

        spriteAnimation.textureRect = IntRect(0, 0, spriteAnimation.width, spriteAnimation.height);
        setTextureRect(spriteAnimation.textureRect);

        setOrigin(spriteAnimation.width/2.0, spriteAnimation.height/2.0);
    }

    virtual void update(RenderWindow* window = nullptr) {
        spritePhysics.xPos += cos(spritePhysics.direction * (PI/180.0)) * spritePhysics.velocity;
        spritePhysics.yPos += -(sin(spritePhysics.direction * (PI/180.0)) * spritePhysics.velocity);

        if (edgeBehavior == LOOP) {
            if (spritePhysics.xPos <= -getGlobalBounds().width)
                spritePhysics.xPos = window->getSize().x + getGlobalBounds().width;
            else if (spritePhysics.xPos >= window->getSize().x + getGlobalBounds().width)
                spritePhysics.xPos = -getGlobalBounds().width;

            if (spritePhysics.yPos <= -getGlobalBounds().height)
                spritePhysics.yPos = window->getSize().y + getGlobalBounds().height;
            else if (spritePhysics.yPos >= window->getSize().y + getGlobalBounds().height)
                spritePhysics.yPos = -getGlobalBounds().height;
        } else if (edgeBehavior == IGNORE) {
            if (spritePhysics.xPos <= -getGlobalBounds().width ||
                spritePhysics.xPos >= window->getSize().x + getGlobalBounds().width ||
                spritePhysics.yPos <= -getGlobalBounds().height ||
                spritePhysics.yPos >= window->getSize().y + getGlobalBounds().height)
                toErase = true;
        }

        spritePhysics.direction -= spritePhysics.rotVelocity;
        spritePhysics.direction = fmod(spritePhysics.direction, 360);
        if (spritePhysics.direction < 0)
            spritePhysics.direction += 360;

        if (spriteAnimation.isAnimated) {
            if (spriteAnimation.clock.getElapsedTime().asSeconds() > spriteAnimation.frameLength) {
                spriteAnimation.currframe++;
                if (spriteAnimation.currframe % spriteAnimation.columns == 0) {
                    spriteAnimation.textureRect.left = 0;
                    spriteAnimation.textureRect.top += spriteAnimation.height;
                } else
                    spriteAnimation.textureRect.left += spriteAnimation.height;

                setTextureRect(spriteAnimation.textureRect);
                spriteAnimation.clock.restart();

                if (spriteAnimation.currframe == spriteAnimation.numFrames) {
                    spriteAnimation.isPlayed = true;
                    spriteAnimation.currframe = 0;

                    spriteAnimation.textureRect.left = 0;
                    spriteAnimation.textureRect.top = 0;
                }

                spriteAnimation.clock.restart();
            }
        }

        setPosition(spritePhysics.xPos, spritePhysics.yPos);
        setRotation(-spritePhysics.direction);
    }

    void accelerate(float override = 0, bool ignoreMax = false) {
        if (override != 0) spritePhysics.velocity += override;
        else spritePhysics.velocity += spritePhysics.acceleration;

        if (!ignoreMax && spritePhysics.velocity > spritePhysics.maxVelocity) spritePhysics.velocity = spritePhysics.maxVelocity;
        if (!ignoreMax && spritePhysics.velocity < spritePhysics.minVelocity) spritePhysics.velocity = spritePhysics.minVelocity;
    }

    void rotAccel(float override = 0, bool ignoreMax = false) {
        if (override != 0) spritePhysics.rotVelocity += override;
        else spritePhysics.rotVelocity += spritePhysics.rotAcceleration;

        if (!ignoreMax && spritePhysics.rotVelocity > spritePhysics.maxRotVelocity) spritePhysics.rotVelocity = spritePhysics.maxRotVelocity;
        else if (!ignoreMax && -spritePhysics.rotVelocity > spritePhysics.maxRotVelocity) spritePhysics.rotVelocity = -spritePhysics.maxRotVelocity;
    }

    void turn(float degrees) {
        spritePhysics.direction -= degrees;

        setRotation(-spritePhysics.direction);
    }

    float getXPos() {
        return spritePhysics.xPos;
    }

    float getYPos() {
        return spritePhysics.yPos;
    }

    void setPosition(float xPos, float yPos) {
        spritePhysics.xPos = xPos;
        spritePhysics.yPos = yPos;

        sf::Sprite::setPosition(sf::Vector2f(xPos, yPos));
    }

    void setPosition(const sf::Vector2f &vec) {
        spritePhysics.xPos = vec.x;
        spritePhysics.yPos = vec.y;

        sf::Sprite::setPosition(vec);
    }

    float getDirection() {
        return spritePhysics.direction;
    }

    void setDirection(float angle) {
        spritePhysics.direction = angle;

        setRotation(-angle);
    }

    void setVelocity(float velo) {
        spritePhysics.velocity = velo;
    }

    float getVelocity() {
        return spritePhysics.velocity;
    }

    float getRotVelocity() {
        return spritePhysics.rotVelocity;
    };

    void setMaxVelocity(float velo) {
        spritePhysics.maxVelocity = velo;
    }

    void setMinVelocity(float velo) {
        spritePhysics.minVelocity = velo;
    }

    void setMaxRotVelocity(float velo) {
        spritePhysics.maxRotVelocity = velo;
    }

    void setEdgeBehavior(EdgeBehavior behavior) {
        edgeBehavior = behavior;
    }

    bool toBeErased() const {
        return toErase;
    }

    Animation getAnimation() {
        return spriteAnimation;
    }

    string getType() {
        return type;
    }

    void setType(string type) {
        this->type = std::move(type);
    }
};


#endif //SFML_TEMPLATE_SPRITE_H
