#include "GameBoard.h"


Texture GameBoard::blocksTexture = Texture();
Texture GameBoard::digitsTexture = Texture();


GameBoard::GameBoard() {
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 10; j++) {
            blocks[i][j] = Sprite();
        }
    }
    score = 0;
    comboCounter = 0;
    linesCleared = 0;
}

bool GameBoard::tetrominoMoveCollides(Tetromino& tetromino, int x, int y) {
    Tetromino moved = Tetromino(tetromino);
    moved.changePosition(x, y);

    int tetrominoXPosition = moved.getXPosition();
    int tetrominoYPosition = moved.getYPosition();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (moved.isBlockPresent(i, j)) {
                if (tetrominoXPosition + i < 0 or tetrominoXPosition + i > 9 or tetrominoYPosition + j > 19) {
                    if (y > 0) tetromino.setInactive();
                    return true;
                }
                if (blocks[4 + tetrominoYPosition + j][tetrominoXPosition + i].getTexture() != nullptr) {
                    if (y > 0) tetromino.setInactive();
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::tetrominoRotationCollides(Tetromino& tetromino) {
    Tetromino rotated = Tetromino(tetromino);
    rotated.rotate();

    int tetrominoXPosition = rotated.getXPosition();
    int tetrominoYPosition = rotated.getYPosition();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (rotated.isBlockPresent(i, j)) {
                if (tetrominoXPosition + i < 0 or tetrominoXPosition + i > 9 or tetrominoYPosition + j > 19) {
                    return true;
                }
                if (blocks[4 + tetrominoYPosition + j][tetrominoXPosition + i].getTexture() != nullptr) {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameBoard::addTetromino(Tetromino& tetromino) {
    int tetrominoXPosition = tetromino.getXPosition();
    int tetrominoYPosition = tetromino.getYPosition();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetromino.isBlockPresent(i, j)) {
                blocks[4 + tetrominoYPosition + j][tetrominoXPosition + i] = Sprite(blocksTexture, IntRect(tetromino.getType() * 40, 0, 40, 40));
            }
        }
    }

    tetromino.setInactive();
}

void GameBoard::draw(RenderWindow& window) {
    drawBlocks(window);
    drawScore(window);
    drawCombo(window);
    drawLinesCount(window);
}

void GameBoard::drawBlocks(RenderWindow& window) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            blocks[4 + i][j].setPosition(50 + j * 40, i * 40);
            window.draw(blocks[4 + i][j]);
        }
    }
}

void GameBoard::drawScore(RenderWindow& window) {
    int currentCombo = comboCounter;
    int offset = 0;
    do {
        int digit = currentCombo % 10;
        Sprite* sprite = new Sprite(digitsTexture);
        sprite->setTextureRect(IntRect(digit * 40, 0, 40, 40));
        sprite->setPosition(660 - offset * 40, 80);
        window.draw(*sprite);
        currentCombo /= 10;
        offset += 1;
        delete sprite;
    } while (currentCombo > 0);
}

void GameBoard::drawCombo(RenderWindow& window) {
    int currentScore = score;
    int offset = 0;
    do {
        int digit = currentScore % 10;
        Sprite* sprite = new Sprite(digitsTexture);
        sprite->setTextureRect(IntRect(digit * 40, 0, 40, 40));
        sprite->setPosition(660 - offset * 40, 240);
        window.draw(*sprite);
        currentScore /= 10;
        offset += 1;
        delete sprite;
    } while (currentScore > 0);
}

void GameBoard::drawLinesCount(RenderWindow& window) {
    int currentLines = linesCleared;
    int offset = 0;
    do {
        int digit = currentLines % 10;
        Sprite* sprite = new Sprite(digitsTexture);
        sprite->setTextureRect(IntRect(digit * 40, 0, 40, 40));
        sprite->setPosition(660 - offset * 40, 400);
        window.draw(*sprite);
        currentLines /= 10;
        offset += 1;
        delete sprite;
    } while (currentLines > 0);
}

void GameBoard::resetComboCounter() {
    comboCounter = 0;
}

void GameBoard::checkRows(bool resetCombo) {
    int rows = 0;
    bool fullRow;
    for (int i = 19; i > -1; i--) {
        fullRow = true;
        for (int j = 0; j < 10; j++) {
            if (blocks[4 + i][j].getTexture() == nullptr) {
                fullRow = false;
            }
        }
        if (fullRow) {
            deleteRow(i);
            rows++;
            i++;
        }
    }

    switch (rows)
    {
    case 1:
        score += 10;
        break;
    case 2:
        score += 30;
        break;
    case 3:
        score += 50;
        break;
    case 4:
        score += 80;
        break;
    default:
        break;
    }

    for (int i = 0; i < rows; i++) {
        score += comboCounter * 5;
        comboCounter += 1;
    }

    linesCleared += rows;

    if (resetCombo and rows == 0) {
        resetComboCounter();
    }
}

void GameBoard::deleteRow(int rowIndex) {
    for (int j = 0; j < 10; j++) {
        blocks[4 + rowIndex][j] = Sprite();
    }

    for (int i = rowIndex; i > 0; i--) {
        for (int j = 0; j < 10; j++) {
            blocks[4 + i][j] = blocks[4 + i - 1][j];
        }
    }
}

bool GameBoard::checkGameOver(Tetromino& tetromino) {
    return tetrominoMoveCollides(tetromino, 0, 0);
}

bool GameBoard::checkGameWon() {
    return score >= 99999;
}

void GameBoard::pause() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            blocks[4 + i][j].setColor(sf::Color(255, 255, 255, 128));
        }
    }
}

void GameBoard::unpause() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            blocks[4 + i][j].setColor(sf::Color(255, 255, 255, 255));
        }
    }
}

void GameBoard::setTextures(String blocksFilePath, String digitsFilePath) {
    blocksTexture.loadFromFile(blocksFilePath);
    digitsTexture.loadFromFile(digitsFilePath);
}