#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <sstream>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPosition[NUM_BRANCHES];



int main()
{
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    String graphics = "assets/graphics/";
    String fonts = "assets/fonts/";
    String sounds = "assets/sound/";

    // Assets
    Texture textureBackground;
    textureBackground.loadFromFile(graphics +"background.png");

    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    Texture textureTree;
    textureTree.loadFromFile(graphics + "tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    Texture textureBee;
    textureBee.loadFromFile(graphics + "bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 1925);

    bool beeActive = false;
    float beeSpeed = 0.0f;

    Texture textureCloud;
    textureCloud.loadFromFile(graphics + "cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;

    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool activeCloud1 = false;
    bool activeCloud2 = false;
    bool activeCloud3 = false;

    float cloudSpeed1 = 0.0f;
    float cloudSpeed2 = 0.0f;
    float cloudSpeed3 = 0.0f;
    
    // Time
    Clock clock;

    RectangleShape timeBar;
    float timeBarStartWidth = 700;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2.0f) - timeBarStartWidth / 2.0f, 930);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    int score = 0;

    // HUD
    Text messageText;
    Text scoreText;

    Font font;
    font.loadFromFile(fonts + "KOMIKABG.ttf");

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press enter to start!");
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f
    );

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare branches [6]
    Texture textureBranch;
    textureBranch.loadFromFile(graphics + "branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Preparing the player
    Texture texturePlayer;
    texturePlayer.loadFromFile(graphics + "player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // Prepare gravestone
    side playerSide = side::LEFT;
    Texture textureRIP;
    textureRIP.loadFromFile(graphics + "rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);
    
    // Prepare axe
    Texture textureAxe;
    textureAxe.loadFromFile(graphics + "axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Preparing the flying log
    Texture textureLog;
    textureLog.loadFromFile(graphics + "log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Vars
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    bool acceptInput = false;

    // Prepare sound FX
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile(sounds + "chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    // Death sound
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile(sounds + "death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile(sounds + "out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);


    while (window.isOpen())
    {
        // Player Input
        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + 0.5;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1220, 720);

                // Update the brancehs
                updateBranches(score);

                // Set log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // Play sound fx
                outOfTime.play();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + 0.5;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);

                // Update the brancehs
                updateBranches(score);

                // Set log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;
            }
        }
        if (timeRemaining > 10)
        {
            timeRemaining = 10;
        }

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            
            score = 0;
            timeRemaining = 6;

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPosition[i] = side::NONE;
            }

            // HIDE gravestone
            spriteRIP.setPosition(675, 2000);

            // Player into position
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        window.clear();
        

        // Update
        if (!paused)
        {

            // Measure Time
            Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                paused = true;
                messageText.setString("Out of TIME!!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Bee setup
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                );

                if (spriteBee.getPosition().x < -100) // || spriteBee.getPosition().x > 1800
                {
                    beeActive = false; // beeSpeed *= -1;
                }
            }

            // Cloud 1
            if (!activeCloud1)
            {
                srand((int)time(0) * 10);
                cloudSpeed1 = (rand() % 200 + 10);
                srand((int)time(0) * 10);
                float height = rand() % 100;
                spriteCloud1.setPosition(-200, height);
                activeCloud1 = true;
            }
            else
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x + (cloudSpeed1 * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );

                if (spriteCloud1.getPosition().x > 2000)
                {
                    activeCloud1 = false;
                }
            }
            // Cloud 2
            if (!activeCloud2)
            {
                srand((int)time(0) * 20);
                cloudSpeed2 = (rand() % 200 + 10);
                srand((int)time(0) * 10);
                float height = (rand() % 200) + 150;
                spriteCloud2.setPosition(-200, height);
                activeCloud2 = true;
            }
            else
            {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x + (cloudSpeed2 * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );

                if (spriteCloud2.getPosition().x > 2000)
                {
                    activeCloud2 = false;
                }
            }
            // Cloud 3
            if (!activeCloud3)
            {
                srand((int)time(0));
                cloudSpeed3 = 100;
                srand((int)time(0) * 30);
                float height = (rand() % 300) + 200;
                spriteCloud3.setPosition(-200, height);
                activeCloud3 = true;
            }
            else
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x + (cloudSpeed3 * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );

                if (spriteCloud3.getPosition().x > 2000)
                {
                    activeCloud3 = false;
                }
            }
        
            // Set Score
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // update branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPosition[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branchPosition[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(3000, height);
                }
            }

            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
                );

                if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000
                    )
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // Player death
            if (branchPosition[5] == playerSide)
            {
                paused = true;
                acceptInput = false;

                spriteRIP.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
                spritePlayer.setPosition(2000, 660);
                spriteAxe.setPosition(2000, 660);
                spriteLog.setPosition(2000, 660);

                messageText.setString("SQUISHED!!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);

                messageText.setPosition(1920 / 2, 1080 / 2);

                death.play();
            }
            
        }

        // Draw
        window.draw(spriteBackground);

        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);

        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);

        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        // Render
        window.display();
    }


    return 0;
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j - 1];
    }

    // Spawn new branch at Pos 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;
    case 1:
        branchPosition[0] = side::RIGHT;
        break;

    default:
        branchPosition[0] = side::NONE;
        break;
    }
}