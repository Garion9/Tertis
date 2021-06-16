#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <Windows.h>

#include "Tetromino.h"
#include "GameBoard.h"

using namespace sf;

enum class GameState : int {
    TitleScreen, Playing, GameOver, GameWon, Exit
};

GameState titleScreen(RenderWindow& window) {
    Texture* titleScreenTexture = new Texture();
    titleScreenTexture->loadFromFile("Resources/title_screen.png");
    Sprite titleScreen(*titleScreenTexture);

    bool startGame = false;
    bool exit = false;

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) exit = true;

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) startGame = true;
            }
        }

        window.clear(Color::Black);
        window.draw(titleScreen);
        window.display();

        if (startGame or exit) {
            break;
        }
    }

    if (startGame) return GameState::Playing;
    else if (exit) return GameState::Exit;
}

GameState playTertis(RenderWindow& window) {
    Music music;
    music.openFromFile("Resources/tertis_theme.wav");
    music.setLoop(true);
    float musicVolume = 35.0;
    music.setVolume(musicVolume);
    music.play();

    srand(time(NULL));

    Tetromino::setTexture("Resources/blocks.png");
    GameBoard::setTextures("Resources/blocks.png", "Resources/digits.png");

    Texture* frameTexture = new Texture();
    frameTexture->loadFromFile("Resources/frame.png");
    Sprite frame(*frameTexture);

    Texture* pauseTexture = new Texture();
    pauseTexture->loadFromFile("Resources/paused.png");
    Sprite pausedLabel(*pauseTexture);
    pausedLabel.setPosition(268, 300);

    GameBoard* gameBoard = new GameBoard();

    int tetrominoType = rand() % Tetromino::totalBlockTypes;
    Tetromino* tetromino = new Tetromino(tetrominoType, Tetromino::initialXposition, Tetromino::initialYposition);

    int xMove = 0;
    bool rotate = false;
    float timer = 0;
    double delay = 1.0;
    bool paused = false;
    bool  gameOver = false;
    bool gameWon = false;

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) return GameState::Exit;

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) rotate = true;
                else if (event.key.code == Keyboard::Left) xMove = -1;
                else if (event.key.code == Keyboard::Right) xMove = 1;
                else if (event.key.code == Keyboard::Down) delay = 0.05;
                else if (event.key.code == Keyboard::Space and !paused) {
                    while (!gameBoard->tetrominoMoveCollides(*tetromino, 0, 1)) {
                        tetromino->changePosition(0, 1);
                    }
                    gameBoard->addTetromino(*tetromino);
                    gameBoard->checkRows(true);
                    int tetrominoType = rand() % Tetromino::totalBlockTypes;
                    tetromino = new Tetromino(tetrominoType, Tetromino::initialXposition, Tetromino::initialYposition);
                    gameOver = gameBoard->checkGameOver(*tetromino);
                    gameWon = gameBoard->checkGameWon();
                }
                else if (event.key.code == Keyboard::Escape) {
                    paused = !paused;
                    if (paused) {
                        music.pause();
                        tetromino->pause();
                        gameBoard->pause();
                        frame.setColor(sf::Color(255, 255, 255, 128));
                    }
                    else {
                        music.play();
                        tetromino->unpause();
                        gameBoard->unpause();
                        frame.setColor(sf::Color(255, 255, 255, 255));
                    }
                }
            }
        }

        if (!paused and xMove != 0 and !gameBoard->tetrominoMoveCollides(*tetromino, xMove, 0)) {
            tetromino->changePosition(xMove, 0);
        }

        if (!paused and rotate and !gameBoard->tetrominoRotationCollides(*tetromino)) {
            tetromino->rotate();
        }

        if (!paused and timer > delay) {
            if (!gameBoard->tetrominoMoveCollides(*tetromino, 0, 1)) {
                tetromino->changePosition(0, 1);
                gameBoard->checkRows(false);
            }
            else {
                gameBoard->addTetromino(*tetromino);
                gameBoard->checkRows(true);
                int tetrominoType = rand() % Tetromino::totalBlockTypes;
                tetromino = new Tetromino(tetrominoType, Tetromino::initialXposition, Tetromino::initialYposition);
                gameOver = gameBoard->checkGameOver(*tetromino);
                gameWon = gameBoard->checkGameWon();
            }

            timer = 0;
        }

        xMove = 0;
        rotate = false;
        delay = 1.0;

        window.clear(Color::Black);
        window.draw(frame);
        tetromino->draw(window);
        gameBoard->draw(window);
        if (paused) window.draw(pausedLabel);
        window.display();

        if (gameOver or gameWon) {
            break;
        }
    }

    if (gameOver) return GameState::GameOver;
    else if (gameWon) return GameState::GameWon;
}

GameState gameOverScreen(RenderWindow& window) {
    Texture* gameOverScreenTexture = new Texture();
    gameOverScreenTexture->loadFromFile("Resources/game_over_screen.png");
    Sprite gameOverScreen(*gameOverScreenTexture);

    bool replay = false;
    bool exitToMenu = false;
    bool exit = false;

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) exit = true;

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) replay = true;
                else if (event.key.code == Keyboard::Escape) exitToMenu = true;
            }
        }

        window.clear(Color::Black);
        window.draw(gameOverScreen);
        window.display();

        if (replay or exitToMenu or exit) {
            break;
        }
    }

    if (replay) return GameState::Playing;
    else if (exitToMenu) return GameState::TitleScreen;
    else if (exit) return GameState::Exit;
}

GameState gameWonScreen(RenderWindow& window) {
    Texture* gameWonScreenTexture = new Texture();
    gameWonScreenTexture->loadFromFile("Resources/game_won_screen.png");
    Sprite gameWonScreen(*gameWonScreenTexture);

    bool replay = false;
    bool exitToMenu = false;
    bool exit = false;

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) exit = true;

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) replay = true;
                else if (event.key.code == Keyboard::Escape) exitToMenu = true;
            }
        }

        window.clear(Color::Black);
        window.draw(gameWonScreen);
        window.display();

        if (replay or exitToMenu or exit) {
            break;
        }
    }

    if (replay) return GameState::Playing;
    else if (exitToMenu) return GameState::TitleScreen;
    else if (exit) return GameState::Exit;
}

int main()
{
    RenderWindow window(VideoMode(710, 850), "Tertis", Style::Titlebar | Style::Close);

    GameState gameState = GameState::TitleScreen;

    while (gameState != GameState::Exit) {
        switch (gameState)
        {
        case GameState::TitleScreen:
            gameState = titleScreen(window);
            break;
        case GameState::Playing:
            gameState = playTertis(window);
            break;
        case GameState::GameOver:
            gameState = gameOverScreen(window);
            break;
        case GameState::GameWon:
            gameState = gameWonScreen(window);
            break;
        }
    }

    window.close();
}
