/*
MIT License

Copyright (c) 2022 Litten1up

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <string>
#include <fstream>
#include "bittendef.h"
#include "version.h"
#include <chrono>
#include <thread>
using namespace std;
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    std::cout << "THE 3 LAWS OF NEWTON\nVersion " << version << std::endl;
    bool battle = false;
    bool up = false;
    bool down = true;
    bool left = true;
    bool right = false;
    bool ballmove = false;
    // ...
    sf::Image image;
    if (!image.loadFromFile("assets/bitten.png"))
    {
    // Error handling...
        return EXIT_FAILURE;
    }

    
// ...
    #ifdef debug
    std::cout << "DEBUG VERSION" << std::endl;
    #endif
    #ifdef battleTest
    std::cout << "BATTLE TEST" << std::endl;
    battle = true;
    #endif
    string savedata = ""; // init the savedata var for storage
    ifstream myfile("bitten.sav", ios::in); // open the savefile
    if (!myfile.is_open()){
        std::ofstream outfile ("bitten.sav");
        outfile << "[bitten-engine-save-file]";
        outfile.close();
    }
    myfile >> savedata; // writes the information from the file to a buffer for later use
    myfile.close();
    int F = 1;
    ifstream forcefile("law2/F.txt", ios::in); // open the savefile
    if (!forcefile.is_open()){
        std::ofstream outfile1 ("law2/F.txt");
        outfile1 << 1;
        outfile1.close();
    }
    forcefile >> F;
    forcefile.close();
    int m = 1;
    ifstream myfile2("law2/m.txt", ios::in); // open the savefile
    if (!myfile2.is_open()){
        std::ofstream outfile2 ("law2/m.txt");
        outfile2 << 1;
        outfile2.close();
    }
    myfile2 >> m;
    myfile2.close();
    int a = 1;
    int textres = 1;
    ifstream textresfile("textres.cfg", ios::in);
    if (!textresfile.is_open()){
        std::ofstream outfiletext ("textres.cfg");
        outfiletext << 50;
        outfiletext.close();
        textres=50;
    }
    textresfile >> textres;
    textresfile.close();
    ifstream myfile3("law2/a.txt", ios::in); // open the savefile
    if (!myfile3.is_open()){
        std::ofstream outfile3 ("law2/a.txt");
        outfile3 << 1;
        outfile3.close();
        std::cout <<  "you can edit the values in the law 2 folder then edit the text to the value you want, ONLY USE INTERGERS DON'T PUT THE UNITS" << std::endl;
    }
    myfile3 >> a;
    myfile3.close();
    // Define some constants
    const float pi = 3.14159f;
    sf::Vector2f paddleSize(25, 25);
    float ballRadius = 10.f;
    cout << savedata << endl;
    // define map loading (unused at the moment for the prototype)
    // ofstream map;
    // Create the window of the application
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(  desktopMode.width,
                                            desktopMode.height,
                                            desktopMode.bitsPerPixel),
                            "3 Laws of newton",
                            sf::Style::Fullscreen);
    
    window.setVerticalSyncEnabled(true);
    window.setActive();
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    // Load and run the music
    sf::Music music;
    // Open it from an audio file
    if (!music.openFromFile("assets/bitten.wav")) // default location of file if downloaded
    {
        if (!music.openFromFile("~/.bitten/assets/bitten.wav")) // linux/macos compatibity for assets
	{
		return EXIT_FAILURE; // crash
	}
    }
    // Change some parameters
    music.setPosition(0, 1, 10); // change its 3D position
    music.setPitch(1);           // increase the pitch
    music.setVolume(50);         // reduce the volume
    music.setLoop(true);         // make it loop
    // Play it
    music.play();
    int gameWidth = window.getSize().x;
    int gameHeight = window.getSize().y;
    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("assets/ball.wav")){
	if (!ballSoundBuffer.loadFromFile("~/.bitten/assets/ball.wav")){
        	return EXIT_FAILURE;
	}
    }
    sf::Sound ballSound(ballSoundBuffer);
    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color(100, 100, 200));
    leftPaddle.setOrigin(paddleSize / 2.f);
    sf::RectangleShape crusor;
    crusor.setSize(paddleSize - sf::Vector2f(3, 3));
    crusor.setOutlineThickness(3);
    crusor.setOutlineColor(sf::Color::Black);
    crusor.setFillColor(sf::Color(100, 100, 200));
    crusor.setOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color(200, 100, 100));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);
    int enemyhp = 100;

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("assets/PressStart2P-Regular.ttf"))
    {
        if (!font.loadFromFile("~/.bitten/assets/PressStart2P-Regular.ttf")) // for the nix
	{
		return EXIT_FAILURE;
	}
    }
    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(textres/2);
    pauseMessage.setPosition(10.f, 150.f);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("THE 3 LAWS OF NEWTON");
    // Initilize the opponet text for battles
    sf::Text battleText;
    battleText.setFont(font);
    battleText.setCharacterSize(40);
    battleText.setPosition(0.f, 400.f);
    battleText.setFillColor(sf::Color::White);
    battleText.setString("Test Battle");
    sf::Texture texture;
    if (!texture.loadFromFile("assets/credits.png"))
    {
        std::cerr << "Error while loading texture" << std::endl;
        return -1;
    }
    sf::Sprite creditsprite;
    creditsprite.setTexture(texture);
    sf::FloatRect spriteSize=creditsprite.getGlobalBounds();
    creditsprite.setOrigin(spriteSize.width/2.,spriteSize.height/2.);
    creditsprite.setPosition(window.getSize().x/2, window.getSize().y/2.);

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.1f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 400.f;
    float ballAngle         = 0.f; // to be changed later

    sf::Clock clock;
    bool isPlaying = true;
    bool law2 = false;
    bool law3 = false;
    bool movePaddle=false;
    if(savedata == "[bitten-engine-save-file]<law2>"){
        isPlaying=false;
        law2=true;
    }
    else if(savedata == "[bitten-engine-save-file]<law3>"){
        isPlaying=false;
        law3=true;
    }
    leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
    rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
    ball.setPosition(gameWidth / 2, gameHeight / 2);
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
                isPlaying = true;
            }
	}
        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();
            pauseMessage.setString("An object in rest will stay in rest,\n an object in motion will stay in motion unless an unballenced force happened");
            if (ballmove){
		    float factor = ballSpeed * deltaTime;
                 ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            }

            if(!battle){ // revoke player movement in battles, we will using a different object for menu and we don't want the player moving in the menus, if it is not an battle you can move normaly
                // Move the player's paddle
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
                   (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
                {
                    leftPaddle.move(0.f, -paddleSpeed * deltaTime);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
                   (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
                {
                    leftPaddle.move(0.f, paddleSpeed * deltaTime);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                   (leftPaddle.getPosition().x + paddleSize.x / 2 > 5.f))
                {
                    leftPaddle.move(-paddleSpeed * deltaTime, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		        (leftPaddle.getPosition().x + paddleSize.x / 2 < gameWidth - 5.f))
                {
                    leftPaddle.move(paddleSpeed * deltaTime, 0.f);
                }
             }
	     else {
                // Move the battle crusor
                if (enemyhp == 0){
                    battleText.setString("You won");
                    while(!sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    if (down){
                        std::cout << "move up" << std::endl;
                        up = true;
                        down = false;
                        crusor.setPosition(crusor.getPosition().x, gameHeight - paddleSize.y - 100); 
                    }
                    else {
                        std::cout << "can't move up" << std::endl;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    if (up){
                        std::cout << "move down" << std::endl;
                        up = false;
                        down = true;
                        crusor.setPosition(crusor.getPosition().x, gameHeight - paddleSize.y);
                    }
                    else {
                        std::cout << "can't move down" << std::endl;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    if (right){
                        std::cout << "move down" << std::endl;
                        left = true;
                        right = false;
                        crusor.setPosition(100 + paddleSize.x / 2, crusor.getPosition().y);
                    }
                    else {
                        std::cout << "can't move left" << std::endl;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    if (left){
                        std::cout << "move down" << std::endl;
                        left = false;
                        right = true;
                        crusor.setPosition(gameWidth - paddleSize.x - 50, crusor.getPosition().y);
                    }
                    else {
                        std::cout << "can't move right" << std::endl;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                    if (down && right){
                        enemyhp = enemyhp - 10;
                    }
                }
             }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 4)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 4)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
//            float factor = ballSpeed * deltaTime;
//            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                pauseMessage.setString("You lost!\nPress enter to restart or\nescape to exit");
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                
                leftPaddle.setPosition(10.f, 50.f);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 5, gameHeight / 5);
		        battle = true;
		        battleText.setCharacterSize(10);
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                crusor.setPosition(100 + paddleSize.x / 2, gameHeight - paddleSize.y);
                crusor.rotate(45);
                ballmove = false;
		        isPlaying = false;
                law2=true;
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 2, gameHeight / 2);
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ball.setPosition(ball.getPosition().y, ballRadius + 0.1f);
		        leftPaddle.setPosition(1,1);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballSound.play();
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
	        {

                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
                ballmove = true;
            }
	    // Right Paddle
	    #ifndef debug
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 4 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 4)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
	    }
	    
	    #endif
	    #ifdef debug
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            	battleText.setString("battle");
                ball.setRadius(ballRadius+3);
            }
	    #endif
	    
        }
        else if(law2){
            float deltaTime = clock.restart().asSeconds();
            pauseMessage.setString("law 2: F=ma");
            if (ballmove){
		        float factor = ballSpeed * deltaTime;
                ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            }
		    pauseMessage.setString("law 2: F=ma");
		    //we don't need movement here sence this is automated
		    if (m*a > F &&
		        !ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                !ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                !ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                !ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
                    if (movePaddle){
			            leftPaddle.move(paddleSpeed * deltaTime, 0.f);
                    }
		    }
            else if (m*a > F &&
		        ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
                    movePaddle=false;
                    cout << "tbh " << m*a << " is greater than " << F;
		    }
		    else if(ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                    ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                    ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                    ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
			    ballmove=true;
			    movePaddle=false;
		    }
		    else if(movePaddle){
			    leftPaddle.move(paddleSpeed * deltaTime, 0.f);
		    }
            else if (ballmove){
		        float factor = ballSpeed * deltaTime;
                ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            }
		    else{
			    movePaddle = true;
			}
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                
                leftPaddle.setPosition(10.f, 50.f);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 5, gameHeight / 5);
		        battle = true;
		        battleText.setCharacterSize(10);
		        pauseMessage.setString("An object in rest will stay in rest,\n an object in motion will stay in motion unless an unballenced force happened");
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                crusor.setPosition(100 + paddleSize.x / 2, gameHeight - paddleSize.y);
                crusor.rotate(45);
                ballmove = false;
		        law2 = false;
                law3=true;
                movePaddle = true;
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 2, gameHeight / 2);
            }
	    }
        else if(law3){
            float deltaTime = clock.restart().asSeconds();
            pauseMessage.setString("law 2: F=ma");
            if (ballmove){
		        float factor = ballSpeed * deltaTime;
                ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            }
		    pauseMessage.setString("law 3: action reaction");
		    //we don't need movement here sence this is automated
		    if (m*a > F &&
		        !ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                !ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                !ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                !ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
                    if (movePaddle){
			            leftPaddle.move(paddleSpeed * deltaTime, 0.f);
                    }
		    }
            else if (m*a > F &&
		        ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
                    movePaddle=false;
                    cout << "tbh " << m*a << " is greater than " << F;
		    }
		    else if(ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                    ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                    ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                    ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2){
			    ballmove=true;
			    movePaddle=false;
		    }
		    else if(movePaddle){
			    leftPaddle.move(paddleSpeed * deltaTime, 0.f);
		    }
            else if (ballmove){
		        float factor = ballSpeed * deltaTime;
                ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
            }
		    else{
			    movePaddle = true;
			}
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                
                leftPaddle.setPosition(10.f, 50.f);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 5, gameHeight / 5);
		        battle = true;
		        battleText.setCharacterSize(10);
		        pauseMessage.setString("An object in rest will stay in rest,\n an object in motion will stay in motion unless an unballenced force happened");
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                crusor.setPosition(100 + paddleSize.x / 2, gameHeight - paddleSize.y);
                crusor.rotate(45);
                ballmove = false;
		        law3 = false;
                movePaddle = true;
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(gameWidth / 2, gameHeight / 2);
                
            }
	    } else{
            float deltaTime = clock.restart().asSeconds();
            pauseMessage.setCharacterSize(textres);
            pauseMessage.setString("Credits");
            battleText.setCharacterSize(textres-10);
            leftPaddle.move(paddleSpeed * deltaTime, 0.f);
            battleText.setString("Built on sfml https://sfml-dev.org and cpp\n \n'Bitten engine' by Sean Tipton (bitten1up): \nhttps://github.com/litten2up/bittens-adventure\n \nProject for Ms Gonzalez 8th grade science class\n \nMusic by Sean Tipton (bitten1up)\n \nSource code on github at https://github.com/litten2up/3-laws-of-motion\n \nPlease don't get mad at me for taking soo long it is like a year late lol\n \nnow a word from our sponsors Raid bug spray:\n keep the bugs out from your house with RAID\n\n\n\n\n\n\n\n(this project is not sponsored by raid it is a joke based on youtube raid shadow legends sponserships)");
        }
        // Clear the window
        window.clear(sf::Color(0, 0, 0));

        if (isPlaying)
        {
            // Draw the paddles and the ball
           window.draw(leftPaddle);
           window.draw(pauseMessage);
	       #ifdef drawall
           window.draw(rightPaddle);
	       #endif
	       if (battle){
		       window.draw(rightPaddle);
               window.draw(battleText);
		       window.draw(crusor);

	       }
	       else {
             window.draw(ball);
            }
            
	    } else if (law2){
		    window.draw(ball);
		    window.draw(leftPaddle);
		    window.draw(pauseMessage);
        }
        else if (law3){
		    window.draw(ball);
		    window.draw(leftPaddle);
		    window.draw(pauseMessage);
        }
        else{
            //#ifdef battleTest
            window.draw(battleText);
            //#endif
            //#ifndef battleTest
            // Draw the pause message
            window.draw(pauseMessage);
            window.draw(leftPaddle);
            window.draw(ball);
            window.draw(creditsprite);
            //#endif
        }

        // Display things on screen
        window.display();
    }
    std::cout << "Exiting" << std::endl;
    return EXIT_SUCCESS;
}
