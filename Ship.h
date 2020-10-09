//
// Created by benmo on 9/25/2020.
//

#ifndef SFML_TEMPLATE_SHIP_H
#define SFML_TEMPLATE_SHIP_H


class Ship : public MySprite {
public:
    Ship(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) : MySprite(texture, scale, xPos, yPos, velocity, direction) {}

    static void shoot(std::vector<MySprite*>* projectiles, Sound &sound, sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) {
        projectiles->push_back(new MySprite(texture, scale, xPos, yPos, velocity, direction));
        sound.play();
    }

    bool hit(RenderWindow &window, vector<MySprite*> &animations, Texture &explosionTexture, default_random_engine &gen, Texture &shieldTexture, int scale, Clock &timer) {
        uniform_int_distribution<int> angle(0, 359);
        animations.push_back(new MySprite(explosionTexture, 100, getXPos(), getYPos(), 0, angle(gen)));
        animations[animations.size() - 1]->makeAnimated(5, 5, 0.01,23);

        setPosition(window.getSize().x/2, window.getSize().y/2);
        setDirection(0);
        setVelocity(0);
        lives--;
        setShield(true, &shieldTexture, scale, &timer);

        return lives <= 0;
    }

    void newLife(vector<Drawable*> &gui, Texture &life, RenderWindow &window, Sound &sound) {
        setNumLivesAdded(getNumLivesAdded() + 1);
        setLives(getLives() + 1);
        gui.insert(gui.begin() + getLives(),new MySprite(life, 20, window.getSize().x - ((getLives()-1)*40 + 25), 25, 0, 270));
        sound.play();
    }

    int getLives() const {
        return lives;
    }

    void setLives(int lives) {
        this->lives = lives;
    }

    void setShield(bool shield, Texture* texture = nullptr, int scale = 0, Clock* timer = nullptr) {
        this->shield = shield;

        if (shield) {
            setRider(new MySprite(*texture, scale));
            timer->restart();
        } else
            setRider(nullptr);
    }

    bool hasShield() const {
        return shield;
    }

    int getScore() const {
        return score;
    }

    void setScore(int score) {
        this->score = score;
    }

    int getNewLifeScore() const {
        return newLifeScore;
    }

    int getNumLivesAdded() const {
        return newLives;
    }

    void setNumLivesAdded(int numLives) {
        newLives = numLives;
    }
private:
    int lives = 3, score = 0, newLifeScore = 10000, newLives = 0;

    bool shield;
};


#endif //SFML_TEMPLATE_SHIP_H
