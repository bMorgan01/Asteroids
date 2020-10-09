class Game {
public:
    int result;

    Game() {
        result = init();
    }
private:
    bool ended = false;

    int init() {
        // display "Hello, World!" -- this still appears in our Run terminal as before
        cout << "Hello, World!" << endl;

        default_random_engine gen(random_device{}());
        Clock fireClock, spawnClock, shieldTimer;

        // create a RenderWindow object
        // specify the size to be 640x640
        // set the title to be "SFML Example Window"
        RenderWindow window(VideoMode(1240, 640), "Asteroids");
        window.setFramerateLimit(30);
        window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 50));

        //********************************************
        //  GRAPHICS
        //********************************************
        Texture ship;
        ship.loadFromFile("data\\Ship.png");

        Texture shield;
        shield.loadFromFile("data\\bubble.png");

        Texture background;
        background.loadFromFile("data\\back.png");

        Texture blueLaser;
        Collision::CreateTextureAndBitmask(blueLaser, "data\\blue.png");

        Texture explosion;
        explosion.loadFromFile("data\\explodeSheet.png");

        vector < Texture * > largeAsteroids;
        for (int i = 1; i <= 7; i++) {
            largeAsteroids.push_back(new Texture);
            Collision::CreateTextureAndBitmask(*largeAsteroids[largeAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
        }

        vector < Texture * > mediumAsteroids;
        for (int i = 8; i <= 17; i++) {
            mediumAsteroids.push_back(new Texture);
            Collision::CreateTextureAndBitmask(*mediumAsteroids[mediumAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
        }

        vector < Texture * > smallAsteroids;
        for (int i = 18; i <= 23; i++) {
            smallAsteroids.push_back(new Texture);
            Collision::CreateTextureAndBitmask(*smallAsteroids[smallAsteroids.size() - 1],
                                               "data\\Asteroids\\(" + to_string(i) + ").png");
        }

        //********************************************
        //  SOUNDS
        //********************************************

        Sound pew;
        SoundBuffer pewBuff;
        pewBuff.loadFromFile("data\\Sounds\\pew.wav");
        pew.setBuffer(pewBuff);
        pew.setVolume(25);

        Sound enemyBoom;
        SoundBuffer enemyBoomBuff;
        enemyBoomBuff.loadFromFile("data\\Sounds\\explosions.wav");
        enemyBoom.setBuffer(enemyBoomBuff);

        Sound newLife;
        SoundBuffer newLifeBuff;
        newLifeBuff.loadFromFile("data\\Sounds\\pling.wav");
        newLife.setBuffer(newLifeBuff);

        //********************************************
        //  Text
        //********************************************

        Text score;
        Font f;
        score.setFillColor(Color::White);
        score.setPosition(10, 10);
        f.loadFromFile("data\\Fonts\\arial.ttf");
        score.setFont(f);
        score.setString("Score: 0");
        score.setCharacterSize(20);

        Text destroyed;
        Font sk;
        destroyed.setFillColor(Color::White);
        sk.loadFromFile("data\\Fonts\\Sk.ttf");
        destroyed.setFont(sk);
        destroyed.setCharacterSize(70);
        destroyed.setString("Destroyed");
        destroyed.setOrigin(destroyed.getGlobalBounds().width/2.0, destroyed.getGlobalBounds().height/2.0);
        destroyed.setPosition(window.getSize().x/2.0, window.getSize().y/3.0);

        Text pressEnter;
        Font monkirta;
        pressEnter.setFillColor(Color::White);
        monkirta.loadFromFile("data\\Fonts\\Monkirta Pursuit NC.ttf");
        pressEnter.setFont(monkirta);
        pressEnter.setCharacterSize(20);
        pressEnter.setString("Press ENTER to return to the menu...");
        pressEnter.setOrigin(pressEnter.getGlobalBounds().width/2.0, pressEnter.getGlobalBounds().height/2.0);
        pressEnter.setPosition(window.getSize().x/2.0, window.getSize().y/2.25);

        //********************************************
        //  Resource loading above here
        //********************************************

        vector < MySprite * > projectiles;
        vector < MySprite * > animations;
        vector < Entity * > enemies;
        vector < Drawable * > gui;
        vector < Drawable * > lost;

        lost.push_back(&destroyed);
        lost.push_back(&pressEnter);

        gui.push_back(new MySprite(background, 100, window.getSize().x / 2, window.getSize().y / 2, 0, 0));
        Ship player(ship, 30, window.getSize().x / 2, window.getSize().y / 2, 0, 0);
        player.setMaxVelocity(15);
        player.setMinVelocity(-7);
        player.setEdgeBehavior(MySprite::EdgeBehavior::LOOP);
        player.setShield(true, &shield, 12, &shieldTimer);

        for (int i = 0; i < player.getLives(); i++)
            gui.push_back(new MySprite(ship, 20, window.getSize().x - (i * 40 + 25), 25, 0, 270));

        gui.push_back(&score);

        // while our window is open, keep it open
        // this is our draw loop
        while (window.isOpen()) {
            window.clear(Color::Black);           // clear the contents of the old frame
            // by setting the window to black

            if (shieldTimer.getElapsedTime().asSeconds() > 5) {
                player.setShield(false);
            }

            //****************************************
            //  Handle keypresses below here
            //****************************************

            if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) player.accelerate(0.5);
            if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) player.accelerate(-.5);
            if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) player.turn(10);
            if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) player.turn(-10);
            if (Keyboard::isKeyPressed(Keyboard::Space) && fireClock.getElapsedTime().asSeconds() > 0.4) {
                player.shoot(&projectiles, pew, blueLaser, 0.3, player.getPosition().x, player.getPosition().y, 20,
                             player.getDirection());
                fireClock.restart();
            }
            if (Keyboard::isKeyPressed(Keyboard::Enter) && ended) return 0;

            //****************************************
            //  Handle keypresses above here
            //****************************************

            //****************************************
            //  Spawn cycle below here
            //****************************************

            uniform_int_distribution<int> stroidChance(1, 4), stroidSize(1, 3),
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
                            enemies.push_back(
                                    new Asteroid(size, size * 100, *smallAsteroids[smallTexture(gen)], scale(gen), x, y,
                                                 velocity(gen), (360 + angle(gen)) % 360));
                            break;
                        case 2:
                            enemies.push_back(
                                    new Asteroid(size, size * 100, *mediumAsteroids[mediumTexture(gen)], scale(gen), x,
                                                 y, velocity(gen), (360 + angle(gen)) % 360));
                            break;
                        case 3:
                            enemies.push_back(
                                    new Asteroid(size, size * 100, *largeAsteroids[largeTexture(gen)], scaleLarge(gen),
                                                 x, y, velocity(gen), (360 + angle(gen)) % 360));
                            break;
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

            for (int i = 0; i < projectiles.size(); i++) {
                projectiles[i]->update(&window);

                if (projectiles[i]->toBeErased()) {
                    projectiles.erase(projectiles.begin() + i);
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                enemies[i]->update(&window);

                if (enemies[i]->toBeErased()) {
                    enemies.erase(enemies.begin() + i);
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                for (int j = 0; j < projectiles.size(); j++) {
                    if (Collision::PixelPerfectTest(*enemies[i], *projectiles[j], 0)) {
                        uniform_int_distribution<int> angle(0, 359);
                        enemies[i]->setHealth(enemies[i]->getHealth() - 1);
                        if (enemies[i]->getHealth() <= 0) {
                            if (enemies[i]->getType() == "asteroid")
                                ((Asteroid *) enemies[i])->hit(animations, enemyBoom, explosion, gen);
                            player.setScore(player.getScore() + enemies[i]->getPoints());
                            enemies.erase(enemies.begin() + i);
                        } else {
                            animations.push_back(
                                    new MySprite(explosion, 20, projectiles[j]->getXPos(), projectiles[j]->getYPos(), 0,
                                                 angle(gen)));
                            animations[animations.size() - 1]->makeAnimated(5, 5, 0.01, 23);
                        }

                        projectiles.erase(projectiles.begin() + j);
                    }
                }
            }

            for (int i = 0; i < enemies.size(); i++) {
                if (player.hasShield() && Collision::CircleTest(*enemies[i], *player.getRider().sprite)) {
                    if (enemies[i]->getType() == "asteroid")
                        ((Asteroid *) enemies[i])->hit(animations, enemyBoom, explosion, gen);
                    enemies.erase(enemies.begin() + i);
                } else if (Collision::CircleTest(*enemies[i], player)) {
                    if (!ended && player.hit(window, animations, explosion, gen, shield, 12, shieldTimer)) {
                        ended = true;
                    }
                    gui.erase(gui.begin() + player.getLives() + 1); //remove player icon from top corner
                }
            }

            for (int i = 0; i < animations.size(); i++) {
                animations[i]->update(&window);
                if (animations[i]->getAnimation().isPlayed)
                    animations.erase(animations.begin() + i);
            }

            score.setString("Score: " + to_string(player.getScore()));

            if (player.getScore() - (player.getNewLifeScore() * player.getNumLivesAdded()) >=
                player.getNewLifeScore()) {
                player.newLife(gui, ship, window, newLife);
            }

            //****************************************
            //  Object updates, deletions, collisions above
            //****************************************

            //****************************************
            //  ADD ALL OF OUR DRAWING BELOW HERE
            //****************************************

            window.draw(*gui[0]); //draw background

            for (int i = 0; i < projectiles.size(); i++)
                window.draw(*projectiles[i]);
            for (int i = 0; i < enemies.size(); i++)
                window.draw(*enemies[i]);

            for (MySprite *a : animations)
                window.draw(*a);

            if (!ended) {
                window.draw(player);

                if (player.getRider().sprite != nullptr)
                    window.draw(*player.getRider().sprite);
            } else {
                for (auto d : lost) window.draw(*d);
            }

            for (int i = 1; i < gui.size(); i++) {
                window.draw(*gui[i]);
            }
            //****************************************
            //  ADD ALL OF OUR DRAWING ABOVE HERE
            //****************************************

            window.display();                       // display the window

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