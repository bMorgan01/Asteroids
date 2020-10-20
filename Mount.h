//
// Created by benmo on 10/12/2020.
//

#ifndef SFML_TEMPLATE_MOUNT_H
#define SFML_TEMPLATE_MOUNT_H


class Mount : public MySprite {
protected:
    vector<Rider> riders;
public:
    Mount(const sf::Texture &texture, float scale, float xPos, float yPos, float velocity, float direction) : MySprite(texture, scale, xPos, yPos, velocity, direction) {}

    bool hasRider() {
        return !riders.empty();
    }
    void update(RenderWindow* window = nullptr) {
        MySprite::update(window);

        for (Rider &r : riders) {
            if (r.doesFollowPosition()) r.setPosition(getPosition());
            if (r.doesFollowDirection()) r.setDirection(getDirection());
        }
    }

    void addRider(Rider &sprite, int index = -1) {
        if (index == -1) {
            riders.push_back(sprite);
            riders[riders.size() - 1].setPosition(getPosition());
        } else {
            riders.insert(riders.begin() + index, sprite);
            riders[index].setPosition(getPosition());
        }
    }

    void removeRider(int index = -1) {
        if (index == -1) riders.pop_back();
        else riders.erase(riders.begin() + index);
    }

    vector<Rider> getRiders() {
        return riders;
    }

    vector<Rider>* getRidersForEdit() {
        return &riders;
    }
};


#endif //SFML_TEMPLATE_MOUNT_H
