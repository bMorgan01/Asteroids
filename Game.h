class Game {
public:
    int result, loop;
    Time mTime;

    Game(int loopNum, Time mTime) {
        loop = loopNum;
        result = init(mTime);
    }
private:
    bool ended = false;

    sf::Texture loadingBarEmpty;
    sf::Texture loadingBarFull;

    //update total
    int totalTextures = 50;
    int loadedTextures = 0;

    sf::Font oxan;

    void updateLoader(sf::RenderWindow &window, const std::string& msg = "") {
        loadedTextures++;
        window.clear(sf::Color::Black);

        //auto start = std::chrono::high_resolution_clock::now();
        MySprite barEmpty(loadingBarEmpty, 100);
        MySprite barFull(loadingBarFull, 100);
        barFull.setScale(1.1, 0.87);
        sf::FloatRect fullBarRect = barFull.getLocalBounds();
        barFull.setOrigin(fullBarRect.width/2.0, fullBarRect.height/2.0);

        barEmpty.setPosition(window.getSize().x/(float)2, window.getSize().y/(float)2);
        barFull.setPosition(barEmpty.getPosition().x, barEmpty.getPosition().y);
        barFull.setTextureRect(sf::IntRect(fullBarRect.left, fullBarRect.top, fullBarRect.width/(float)totalTextures * (float)loadedTextures, fullBarRect.height));

        sf::Text loadingMessage(msg, oxan, 15);
        loadingMessage.setOrigin(loadingMessage.getGlobalBounds().width/2.0, loadingMessage.getGlobalBounds().height/2.0);
        loadingMessage.setPosition(window.getSize().x/2.0, window.getSize().y/2.0 + 50);

        window.draw(barEmpty);
        window.draw(barFull);
        window.draw(loadingMessage);

        window.display();
    }

    int init(Time mTime) {
        // display "Hello, World!" -- this still appears in our Run terminal as before
        cout << "Hello, World!" << endl;

        default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
        Clock fireClock, spawnClock, shieldTimer;

        bool poweredUpRapid = false;
        Clock rapidFireClock;

        bool poweredUpDamage = false;
        Clock damageClock;

        bool poweredUpSpray = false;
        Clock sprayFireClock;

        bool poweredUpbackwards = false;
        Clock backwardsClock;

        bool poweredUpFighter = false;
        Clock fighterClock;

        bool poweredUpTurret = false;
        Clock turretClock, turretFireClock;

        bool poweredUpTime = false;
        Clock timeClock;

        // create a RenderWindow object
        // specify the size to be 640x640
        // set the title to be "SFML Example Window"
        RenderWindow window(VideoMode(1240, 640), "Asteroids");
        window.setFramerateLimit(30);
        window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 50));

        //********************************************
        //  FONTS
        //********************************************

        oxan.loadFromFile("data/Fonts/Oxanium-Light.ttf");
        updateLoader(window, "Preparing load screen...");
        auto start = std::chrono::high_resolution_clock::now();
        loadingBarEmpty.loadFromFile("data/Gui/LoadingBarEmpty.png");
        updateLoader(window, "Preparing load screen...");
        loadingBarFull.loadFromFile("data/Gui/LoadingBarFull.png");
        updateLoader(window, "Preparing load screen...");
        loadingBarFull.setRepeated(false);

        //********************************************
        //  FONTS
        //********************************************

        Font sk;
        sk.loadFromFile("data\\Fonts\\Sk.ttf");
        updateLoader(window, "Loading fonts...");

        Font monkirta;
        monkirta.loadFromFile("data\\Fonts\\Monkirta Pursuit NC.ttf");
        updateLoader(window, "Loading fonts...");

        //********************************************
        //  GRAPHICS
        //********************************************
        Texture ship;
        ship.loadFromFile("data\\Ship.png");
        updateLoader(window, "Loading textures...");

        Texture shield;
        shield.loadFromFile("data\\bubble.png");
        updateLoader(window, "Loading textures...");

        Texture background;
        background.loadFromFile("data\\back.png");
        updateLoader(window, "Loading textures...");

        Texture turretGun;
        turretGun.loadFromFile("data\\turret.png");
        updateLoader(window, "Loading textures...");

        Texture blueLaser;
        Collision::CreateTextureAndBitmask(blueLaser, "data\\blue.png");
        updateLoader(window, "Loading textures...");

        Texture greenLaser;
        Collision::CreateTextureAndBitmask(greenLaser, "data\\greenLaser.png");
        updateLoader(window, "Loading textures...");

        Texture redLaser;
        Collision::CreateTextureAndBitmask(redLaser, "data\\redLaser.png");
        updateLoader(window, "Loading textures...");

        Texture explosion;
        explosion.loadFromFile("data\\explodeSheet.png");
        updateLoader(window, "Loading textures...");

        Texture rapidFire;
        rapidFire.loadFromFile("data\\Gui\\rapidFire.png");
        updateLoader(window, "Loading textures...");

        Texture damage;
        damage.loadFromFile("data\\Gui\\damage.png");
        updateLoader(window, "Loading textures...");

        Texture spray;
        spray.loadFromFile("data\\Gui\\spray.png");
        updateLoader(window, "Loading textures...");

        Texture backwards;
        backwards.loadFromFile("data\\Gui\\directions.png");
        updateLoader(window, "Loading textures...");

        Texture shieldUp;
        shieldUp.loadFromFile("data\\Gui\\shield.png");
        updateLoader(window, "Loading textures...");

        Texture slowTime;
        slowTime.loadFromFile("data\\Gui\\slowTime.png");
        updateLoader(window, "Loading textures...");

        Texture wingman;
        wingman.loadFromFile("data\\Gui\\fighter.png");
        updateLoader(window, "Loading textures...");

        Texture turret;
        turret.loadFromFile("data\\Gui\\turret.png");
        updateLoader(window, "Loading textures...");

        vector < Texture > largeAsteroids;
        for (int i = 1; i <= 7; i++) {
            largeAsteroids.emplace_back();
            Collision::CreateTextureAndBitmask(largeAsteroids[largeAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
            updateLoader(window, "Loading textures...");
        }

        vector < Texture > mediumAsteroids;
        for (int i = 8; i <= 17; i++) {
            mediumAsteroids.emplace_back();
            Collision::CreateTextureAndBitmask(mediumAsteroids[mediumAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
            updateLoader(window, "Loading textures...");
        }

        vector < Texture > smallAsteroids;
        for (int i = 18; i <= 23; i++) {
            smallAsteroids.emplace_back();
            Collision::CreateTextureAndBitmask(smallAsteroids[smallAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
            updateLoader(window, "Loading textures...");
        }

        //********************************************
        //  SOUNDS
        //********************************************

        Sound pew;
        SoundBuffer pewBuff;
        pewBuff.loadFromFile("data\\Sounds\\pew.wav");
        updateLoader(window, "Loading sounds...");
        pew.setBuffer(pewBuff);
        pew.setVolume(25);

        Sound enemyBoom;
        SoundBuffer enemyBoomBuff;
        enemyBoomBuff.loadFromFile("data\\Sounds\\explosions.wav");
        updateLoader(window, "Loading sounds...");
        enemyBoom.setBuffer(enemyBoomBuff);

        Sound playerBoom;
        SoundBuffer playerBoomBuff;
        playerBoomBuff.loadFromFile("data\\Sounds\\boom.wav");
        updateLoader(window, "Loading sounds...");
        playerBoom.setBuffer(playerBoomBuff);

        Sound playerLost;
        SoundBuffer playerBoomLost;
        playerBoomLost.loadFromFile("data\\Sounds\\lost.wav");
        updateLoader(window, "Loading sounds...");
        playerLost.setBuffer(playerBoomLost);

        Sound newLife;
        SoundBuffer newLifeBuff;
        newLifeBuff.loadFromFile("data\\Sounds\\pling.wav");
        updateLoader(window, "Loading sounds...");
        newLife.setBuffer(newLifeBuff);

        Sound powerUpSound;
        SoundBuffer powerUpBuff;
        powerUpBuff.loadFromFile("data\\Sounds\\powerUp.wav");
        updateLoader(window, "Loading sounds...");
        powerUpSound.setBuffer(powerUpBuff);

        sf::Music musicLoop1;
        musicLoop1.openFromFile("data/Sounds/Unknown Theme.wav");
        updateLoader(window, "Loading sounds...");
        musicLoop1.setVolume(60);

        sf::Music musicLoop2;
        musicLoop2.openFromFile("data/Sounds/loop.wav");
        updateLoader(window, "Loading sounds...");
        
        if (loop == 1) {
            musicLoop1.setPlayingOffset(mTime);
            musicLoop1.play();
        }
        else {
            musicLoop2.setPlayingOffset(mTime);
            musicLoop2.play();
        }

        //********************************************
        //  Text
        //********************************************

        Text score;
        score.setFillColor(Color::White);
        score.setPosition(10, 10);
        score.setFont(oxan);
        score.setString("Score: 0");
        score.setCharacterSize(20);

        Text destroyed;
        destroyed.setFillColor(Color::White);
        destroyed.setFont(sk);
        destroyed.setCharacterSize(70);
        destroyed.setString("Destroyed");
        destroyed.setOrigin(destroyed.getGlobalBounds().width/2.0, destroyed.getGlobalBounds().height/2.0);
        destroyed.setPosition(window.getSize().x/2.0, window.getSize().y/3.0);

        Text pressEnter;
        pressEnter.setFillColor(Color::White);
        pressEnter.setFont(monkirta);
        pressEnter.setCharacterSize(20);
        pressEnter.setString("Press ENTER to return to the menu...");
        pressEnter.setOrigin(pressEnter.getGlobalBounds().width/2.0, pressEnter.getGlobalBounds().height/2.0);
        pressEnter.setPosition(window.getSize().x/2.0, window.getSize().y/2.25);

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << " milliseconds" << std::endl;

        std::cout << loadedTextures << std::endl;
        if (loadedTextures != totalTextures) std::cout << "UPDATE!!!" << std::endl;

        //********************************************
        //  Resource loading above here
        //********************************************

        vector < MySprite > projectiles;
        vector < MySprite > animations;
        vector < Entity * > enemies;
        vector < Drawable * > gui;
        vector < MySprite > lives;
        vector < Drawable * > lost;

        lost.push_back(&destroyed);
        lost.push_back(&pressEnter);

        MySprite backSprite(background, 100, window.getSize().x / 2,
                 window.getSize().y / 2, 0, 0);

        Ship player(ship, 30, window.getSize().x / 2, window.getSize().y / 2, 0, 0);
        player.setMaxVelocity(15);
        player.setMinVelocity(-7);
        player.setEdgeBehavior(MySprite::EdgeBehavior::LOOP);
        player.setShield(true, &shield, 12, &shieldTimer);

        gui.push_back(&backSprite);
        gui.push_back(&score);

        for (int i = 0; i < player.getLives(); i++)
            lives.emplace_back(ship, 20, window.getSize().x - (i * 40 + 25), 25, 0, 270);

        // while our window is open, keep it open
        // this is our draw loop
        while (window.isOpen()) {
            window.clear(Color::Black);           // clear the contents of the old frame
            // by setting the window to black

            if (shieldTimer.getElapsedTime().asSeconds() > 5) player.setShield(false);
            if (poweredUpRapid && rapidFireClock.getElapsedTime().asSeconds() > 12) poweredUpRapid = false;
            if (poweredUpDamage && damageClock.getElapsedTime().asSeconds() > 12) poweredUpDamage = false;
            if (poweredUpSpray && sprayFireClock.getElapsedTime().asSeconds() > 12) poweredUpSpray = false;
            if (poweredUpbackwards && backwardsClock.getElapsedTime().asSeconds() > 12) poweredUpbackwards = false;
            if (poweredUpFighter && fighterClock.getElapsedTime().asSeconds() > 12) poweredUpFighter = false;
            if (poweredUpTurret && turretClock.getElapsedTime().asSeconds() > 12) {
                poweredUpTurret = false;
                player.removeRider(0);
            } if (poweredUpTime && timeClock.getElapsedTime().asSeconds() > 5) poweredUpTime = false;

            //****************************************
            //  Handle keypresses below here
            //****************************************

            if (!ended) {
                if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)))
                    player.accelerate(0.5);
                if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)))
                    player.accelerate(-.5);
                if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) player.turn(10);
                if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) player.turn(-10);
                if (Keyboard::isKeyPressed(Keyboard::Space) && ((!poweredUpRapid && fireClock.getElapsedTime().asSeconds() > 0.4) || (poweredUpRapid && fireClock.getElapsedTime().asSeconds() > 0.2))) {
                    if (!poweredUpRapid)
                        player.shoot(projectiles, pew, blueLaser, 0.3, player.getPosition().x,
                                     player.getPosition().y, 20,
                                     player.getDirection());
                    else
                        player.shoot(projectiles, pew, greenLaser, 0.3, player.getPosition().x,
                                     player.getPosition().y, 20,
                                     player.getDirection());

                    if (poweredUpDamage) {
                        projectiles[projectiles.size() - 1].setType("Damage");
                        projectiles[projectiles.size() - 1].setTexture(redLaser);
                    }

                    if (poweredUpbackwards) {
                        if (!poweredUpRapid)
                            player.shoot(projectiles, pew, blueLaser, 0.3, player.getPosition().x,
                                         player.getPosition().y, 20,
                                         player.getDirection() +180);
                        else
                            player.shoot(projectiles, pew, greenLaser, 0.3, player.getPosition().x,
                                         player.getPosition().y, 20,
                                         player.getDirection() +180);

                        if (poweredUpDamage) {
                            projectiles[projectiles.size() - 1].setType("Damage");
                            projectiles[projectiles.size() - 1].setTexture(redLaser);
                        }
                    }

                    if (poweredUpSpray) {
                        for (int offset = -15; offset <= 15; offset += 30) {
                            if (!poweredUpRapid)
                                player.shoot(projectiles, pew, blueLaser, 0.3, player.getPosition().x,
                                             player.getPosition().y, 20,
                                             player.getDirection() + offset);
                            else
                                player.shoot(projectiles, pew, greenLaser, 0.3, player.getPosition().x,
                                             player.getPosition().y, 20,
                                             player.getDirection() + offset);

                            if (poweredUpDamage) {
                                projectiles[projectiles.size() - 1].setType("Damage");
                                projectiles[projectiles.size() - 1].setTexture(redLaser);
                            }
                        }
                    }

                    fireClock.restart();
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Enter) && ended) {
                if (loop == 1) this->mTime = musicLoop1.getPlayingOffset();
                else this->mTime = musicLoop2.getPlayingOffset();

                musicLoop1.stop();
                musicLoop2.stop();

                return 0;
            }

            //****************************************
            //  Handle keypresses above here
            //****************************************

            //****************************************
            //  Spawn cycle below here
            //****************************************

            uniform_int_distribution<int> stroidChance(1, 4), powerUpChance(9, 10),
                    powerUp(7, 8), stroidSize(1, 3),
                    smallTexture(0, smallAsteroids.size() - 1),
                    mediumTexture(0, mediumAsteroids.size() - 1),
                    largeTexture(0, largeAsteroids.size() - 1), side(1, 4),
                    xPos(0, window.getSize().x), yPos(0, window.getSize().y),
                    scale(70, 110), scaleLarge(65, 85);

            uniform_real_distribution<double> velocity(0.5, 4);

            if (spawnClock.getElapsedTime().asSeconds() >= 0.10) {  //spawn chances every .1 sec
                if (stroidChance(gen) == 1) {                    //stroid spawns happen approx. every half sec
                    uniform_int_distribution<int> angle;
                    int x = 0, y = 0, s = side(gen);
                    switch (s) {                        //decide which side of window to spawn stroid
                        case 1:                                    //top
                            x = xPos(gen);                      //get random xPos
                            if (x <= window.getSize().x /
                                     2) {       //if on left half of window (calculate angle to point towards window)
                                angle = uniform_int_distribution<int>(
                                        285 - ((double) x) / (window.getSize().x / 2.0) * 75, 330);
                            } else {                               //else
                                angle = uniform_int_distribution<int>(210, 330 -
                                                                           ((double) (x - window.getSize().x / 2.0)) /
                                                                           (window.getSize().x / 2.0) * 75);
                            }
                            break;
                        case 2:                                     //left
                            y = yPos(gen);                      //get random yPos
                            if (y <= window.getSize().y /
                                     2) {       //if on top half of window (calculate angle to point towards window)
                                angle = uniform_int_distribution<int>(
                                        300, 345 + ((double) y) / (window.getSize().y / 2.0) * 75);
                            } else {                               //else
                                angle = uniform_int_distribution<int>(-60 + ((double) (y - window.getSize().y / 2.0)) /
                                                                            (window.getSize().y / 2.0) * 75, 60);
                            }
                            break;
                        case 3:                                    //bottom
                            x = xPos(gen);                      //get random xPos
                            y = window.getSize().y;
                            if (x <= window.getSize().x /
                                     2) {       //if on left half of window (calculate angle to point towards window)
                                angle = uniform_int_distribution<int>(
                                        30, 150 - ((double) x) / (window.getSize().x / 2.0) * 75);
                            } else {                               //else
                                angle = uniform_int_distribution<int>(105 - ((double) (x - window.getSize().x / 2.0)) /
                                                                            (window.getSize().x / 2.0) * 75, 150);
                            }
                            break;
                        case 4:                                    //right
                            y = yPos(gen);                      //get random yPos
                            x = window.getSize().x;
                            if (y <= window.getSize().y /
                                     2) {       //if on top half of window (calculate angle to point towards window)
                                angle = uniform_int_distribution<int>(
                                        195 - ((double) y) / (window.getSize().y / 2.0) * 75, 240);
                            } else {                               //else
                                angle = uniform_int_distribution<int>(120, 165 +
                                                                           ((double) (y - window.getSize().y / 2.0)) /
                                                                           (window.getSize().y / 2.0) * 75);
                            }
                            break;
                    }

                    int size = stroidSize(gen);
                    switch (size) {
                        case 1:
                            enemies.push_back(new Asteroid(size, size * 100, smallAsteroids[smallTexture(gen)], scale(gen), x, y,
                                                 velocity(gen), (360 + angle(gen)) % 360));
                            break;
                        case 2:
                            enemies.push_back(new Asteroid(size, size * 100, mediumAsteroids[mediumTexture(gen)], scale(gen), x,
                                                 y, velocity(gen), (360 + angle(gen)) % 360));
                            break;
                        case 3:
                            enemies.push_back(new Asteroid(size, size * 100, largeAsteroids[largeTexture(gen)], scaleLarge(gen),
                                                 x, y, velocity(gen), (360 + angle(gen)) % 360));
                            break;
                    }

                    if (size == 3 && powerUpChance(gen) == 10) {
                        Texture *temp;
                        string type;
                        switch (powerUp(gen)) {
                            case 1:
                                temp = &rapidFire;
                                type = "Rapid";
                                break;
                            case 2:
                                temp = &damage;
                                type = "Damage";
                                break;
                            case 3:
                                temp = &spray;
                                type = "Spray";
                                break;
                            case 4:
                                temp = &backwards;
                                type = "Backwards";
                                break;
                            case 5:
                                temp = &shieldUp;
                                type = "Shield";
                                break;
                            case 6:
                                temp = &slowTime;
                                type = "Time";
                                break;
                            case 7:
                                temp = &wingman;
                                type = "Wingman";
                                break;
                            case 8:
                                temp = &turret;
                                type = "Turret";
                                break;
                        }
                        Rider moveEntity(*temp, 25);
                        moveEntity.setType(type);
                        enemies[enemies.size() - 1]->addRider(moveEntity);
                    }
                }
                spawnClock.restart();
            }

            //****************************************
            //  Spawn cycle above here
            //****************************************

            //****************************************
            //  Object updates/deletion
            //****************************************

            player.update(&window);

            if (poweredUpTurret) {
                int closest = 0;
                for (int i = 1; i < enemies.size(); i++) {
                    float xDistClosest = enemies[closest]->getXPos() - player.getXPos();
                    float yDistClosest = enemies[closest]->getYPos() - player.getYPos();
                    float xDistTest = enemies[i]->getXPos() - player.getXPos();
                    float yDistTest = enemies[i]->getYPos() - player.getYPos();
                    float closeDist = xDistClosest*xDistClosest + yDistClosest*yDistClosest;
                    float testDist = xDistTest*xDistTest + yDistTest*yDistTest;
                    //float edgeOffsetClosest = player.getGlobalBounds().width/2 + enemies[closest]->getGlobalBounds().width/2;
                    //float edgeOffsetTest = player.getGlobalBounds().width/2 + enemies[i]->getGlobalBounds().width/2;

                    if ((testDist/* - (edgeOffsetTest*edgeOffsetTest)*/) < (closeDist/* - (edgeOffsetClosest*edgeOffsetClosest)*/) )
                        closest = i;
                }

                float xDistClosest = enemies[closest]->getXPos() - player.getXPos();
                float yDistClosest = enemies[closest]->getYPos() - player.getYPos();

                player.getRidersForEdit()->at(0).setDirection(-(180.0/MySprite::PI) * atan2(yDistClosest, xDistClosest));

                if (turretFireClock.getElapsedTime().asSeconds() > 0.4) {
                    player.shoot(projectiles, pew, blueLaser, 0.2, player.getPosition().x,
                                 player.getPosition().y, 20,
                                 player.getRiders()[0].getDirection());
                    turretFireClock.restart();
                }
            }

            for (int i = 0; i < projectiles.size(); i++) {
                projectiles[i].update(&window);

                if (projectiles[i].toBeErased()) {
                    projectiles.erase(projectiles.begin() + i);
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                enemies[i]->update(&window);

                if (enemies[i]->toBeErased()) {
                    delete enemies[i];
                    enemies.erase(enemies.begin() + i);
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                for (int j = 0; j < projectiles.size(); j++) {
                    if (Collision::PixelPerfectTest(*enemies[i], projectiles[j], 0)) {
                        uniform_int_distribution<int> angle(0, 359);
                        if (projectiles[j].getType() != "Damage") enemies[i]->setHealth(enemies[i]->getHealth() - 1);
                        else enemies[i]->setHealth(enemies[i]->getHealth() - 2);
                        if (enemies[i]->getHealth() <= 0) {
                            if (enemies[i]->getType() == "asteroid")
                                ((Asteroid *)enemies[i])->hit(animations, enemyBoom, explosion, gen);
                            player.setScore(player.getScore() + enemies[i]->getPoints());

                            if (enemies[i]->hasRider()) {
                                powerUpSound.play();

                                if (enemies[i]->getRiders()[0].getType() == "Rapid") {
                                    poweredUpRapid = true;
                                    rapidFireClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Damage") {
                                    poweredUpDamage = true;
                                    damageClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Spray") {
                                    poweredUpSpray = true;
                                    sprayFireClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Backwards") {
                                    poweredUpbackwards = true;
                                    backwardsClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Fighter") {
                                    poweredUpFighter = true;
                                    fighterClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Turret") {
                                    if (!poweredUpTurret) {
                                        Rider temp(turretGun, 20);
                                        player.addRider(temp, 0);
                                    }

                                    poweredUpTurret = true;
                                    turretClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Time") {
                                    poweredUpTime = true;
                                    timeClock.restart();
                                } else if (enemies[i]->getRiders()[0].getType() == "Shield") {
                                    shieldTimer.restart();
                                    player.setShield(true, &shield, 12, &shieldTimer);
                                }
                            }

                            delete enemies[i];
                            enemies.erase(enemies.begin() + i);
                        } else {
                            animations.emplace_back(explosion, 20, projectiles[j].getXPos(), projectiles[j].getYPos(), 0,
                                                 angle(gen));
                            animations[animations.size() - 1].makeAnimated(5, 5, 0.01, 23);
                        }

                        projectiles.erase(projectiles.begin() + j);
                    }
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                if (!ended && player.hasShield()) {
                    int shieldIndex;
                    if (poweredUpTurret) shieldIndex = 1;
                    else shieldIndex = 0;

                    if (Collision::CircleTest(*enemies[i], player.getRiders()[shieldIndex])) {
                        if (enemies[i]->getType() == "asteroid")
                            ((Asteroid *) enemies[i])->hit(animations, enemyBoom, explosion, gen);
                        delete enemies[i];
                        enemies.erase(enemies.begin() + i);
                    }
                } else if (!ended && Collision::CircleTest(*enemies[i], player)) {
                    if (player.hit(window, animations, explosion, gen, shield, 12, shieldTimer)) {
                        ended = true;
                        playerLost.play();
                    } else playerBoom.play();
                    lives.erase(lives.begin() + player.getLives()); //remove player icon from top corner
                }
            }

            for (int i = 0; i < animations.size(); i++) {
                animations[i].update(&window);
                if (animations[i].getAnimation().isPlayed) {
                    animations.erase(animations.begin() + i);
                }
            }

            score.setString("Score: " + to_string(player.getScore()));

            if (player.getScore() - (player.getNewLifeScore() * player.getNumLivesAdded()) >=
                player.getNewLifeScore()) {
                player.newLife(lives, ship, window, newLife);
            }

            //****************************************
            //  Object updates, deletions, collisions above
            //****************************************

            //****************************************
            //  ADD ALL OF OUR DRAWING BELOW HERE
            //****************************************

            window.draw(*gui[0]); //draw background

            for (const auto & projectile : projectiles)
                window.draw(projectile);
            for (const auto & enemy : enemies){
                window.draw(*enemy);
                if (enemy->hasRider()) {
                    for (const auto& r : enemy->getRiders()) {
                        window.draw(r);
                    }
                }
            }

            for (const MySprite& a : animations)
                window.draw(a);

            if (!ended) {
                window.draw(player);

                if (player.hasRider()) {
                    for (const auto& r : player.getRiders()) {
                        window.draw(r);
                    }
                }
            } else {
                for (auto & i : lost) window.draw(*i);
            }

            for (int i = 1; i < gui.size(); i++) {
                window.draw(*gui[i]);
            }

            for (auto i: lives) {
                window.draw(i);
            }
            //****************************************
            //  ADD ALL OF OUR DRAWING ABOVE HERE
            //****************************************

            window.display();                       // display the window

            if (loop == 1 && musicLoop1.getStatus() == Music::Stopped) {
                musicLoop2.setPlayingOffset(Time::Zero);
                musicLoop2.play();
                loop = 2;
            } else if (loop == 2 && musicLoop2.getStatus() == Music::Stopped) {
                musicLoop1.setPlayingOffset(Time::Zero);
                musicLoop1.play();
                loop = 1;
            }

            //****************************************
            // HANDLE EVENTS BELOW HERE
            //****************************************
            Event event;
            while (window.pollEvent(event)) {   // ask the window if any events occurred
                if (event.type == Event::Closed) { // if event type is a closed event
                    // i.e. they clicked the X on the window
                    window.close();                 // then close our window
                }
            }
        }

        return 1;                        // report our program exited successfully
    }
};