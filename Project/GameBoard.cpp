#include "GameBoard.h"


Texture GameBoard::blocksTexture = Texture();
Texture GameBoard::digitsTexture = Texture();


GameBoard::GameBoard() {
    for (int i = 0; i < rows + bufferRows; i++) {
        for (int j = 0; j < columns; j++) {
            blocks[i][j] = Sprite();
        }
    }
    score = 0;
    comboCounter = 0;
    linesCleared = 0;
}

bool GameBoard::tetrominoMoveCollides(Tetromino& tetromino, int x, int y) const {
    Tetromino moved = Tetromino(tetromino);
    moved.changePosition(x, y);

    int tetrominoXPosition = moved.getXPosition();
    int tetrominoYPosition = moved.getYPosition();

    for (int i = 0; i < Tetromino::placementGridSize; i++) {
        for (int j = 0; j < Tetromino::placementGridSize; j++) {
            if (moved.isBlockPresent(i, j)) {
                if (tetrominoXPosition + i < minColumnIndex or tetrominoXPosition + i > maxColumnIndex or tetrominoYPosition + j > maxRowIndex) {
                    return true;
                }
                if (blocks[bufferRows + tetrominoYPosition + j][tetrominoXPosition + i].getTexture() != nullptr) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::tetrominoRotationCollides(Tetromino& tetromino) const {
    Tetromino rotated = Tetromino(tetromino);
    rotated.rotate();

    int tetrominoXPosition = rotated.getXPosition();
    int tetrominoYPosition = rotated.getYPosition();

    for (int i = 0; i < Tetromino::placementGridSize; i++) {
        for (int j = 0; j < Tetromino::placementGridSize; j++) {
            if (rotated.isBlockPresent(i, j)) {
                if (tetrominoXPosition + i < minColumnIndex or tetrominoXPosition + i > maxColumnIndex or tetrominoYPosition + j > maxRowIndex) {
                    return true;
                }
                if (blocks[bufferRows + tetrominoYPosition + j][tetrominoXPosition + i].getTexture() != nullptr) {
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
    for (int i = 0; i < Tetromino::placementGridSize; i++) {
        for (int j = 0; j < Tetromino::placementGridSize; j++) {
            if (tetromino.isBlockPresent(i, j)) {
                blocks[bufferRows + tetrominoYPosition + j][tetrominoXPosition + i] = Sprite(blocksTexture, IntRect(tetromino.getType() * spriteBlockSize, 0, spriteBlockSize, spriteBlockSize));
            }
        }
    }
}

void GameBoard::draw(RenderWindow& window) {
    drawBlocks(window);
    drawScore(window);
    drawCombo(window);
    drawLinesCount(window);
}

void GameBoard::drawBlocks(RenderWindow& window) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            blocks[bufferRows + i][j].setPosition(xAxisBlockDrawOffset + j * spriteBlockSize, i * spriteBlockSize);
            window.draw(blocks[bufferRows + i][j]);
        }
    }
}

void GameBoard::drawScore(RenderWindow& window) {
    int currentCombo = comboCounter;
    int offset = 0;
    do {
        int digit = currentCombo % 10;
        Sprite* sprite = new Sprite(digitsTexture);
        sprite->setTextureRect(IntRect(digit * spriteBlockSize, 0, spriteBlockSize, spriteBlockSize));
        sprite->setPosition(xAxisScoreDrawOffset - offset * spriteBlockSize, yAxisScoreDrawOffset);
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
        sprite->setTextureRect(IntRect(digit * spriteBlockSize, 0, spriteBlockSize, spriteBlockSize));
        sprite->setPosition(xAxisComboDrawOffset - offset * spriteBlockSize, yAxisComboDrawOffset);
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
        sprite->setTextureRect(IntRect(digit * spriteBlockSize, 0, spriteBlockSize, spriteBlockSize));
        sprite->setPosition(xAxisLinesCountDrawOffset - offset * spriteBlockSize, yAxisLinesCountDrawOffset);
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
    for (int i = maxRowIndex; i >= minRowIndex; i--) {
        fullRow = true;
        for (int j = minColumnIndex; j <= maxColumnIndex; j++) {
            if (blocks[bufferRows + i][j].getTexture() == nullptr) {
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
        score += singleLineScore;
        break;
    case 2:
        score += doubleLineScore;
        break;
    case 3:
        score += tripleLineScore;
        break;
    case 4:
        score += tetrisScore;
        break;
    default:
        break;
    }

    for (int i = 0; i < rows; i++) {
        score += comboCounter * comboScoreMultiplier;
        comboCounter += 1;
    }

    linesCleared += rows;

    if (resetCombo and rows == 0) {
        resetComboCounter();
    }
}

void GameBoard::deleteRow(int rowIndex) {
    for (int j = minColumnIndex; j <= maxColumnIndex; j++) {
        blocks[bufferRows + rowIndex][j] = Sprite();
    }

    for (int i = rowIndex; i > minRowIndex; i--) {
        for (int j = minColumnIndex; j <= maxColumnIndex; j++) {
            blocks[bufferRows + i][j] = blocks[bufferRows + i - 1][j];
        }
    }
}

bool GameBoard::checkGameOver(Tetromino& tetromino) const {
    return tetrominoMoveCollides(tetromino, 0, 0);
}

bool GameBoard::checkGameWon() const {
    return score >= maxScore;
}

void GameBoard::pause() {
    for (int i = minRowIndex; i <= maxRowIndex; i++) {
        for (int j = minColumnIndex; j <= maxRowIndex; j++) {
            blocks[bufferRows + i][j].setColor(sf::Color(255, 255, 255, 128));
        }
    }
}

void GameBoard::unpause() {
    for (int i = minRowIndex; i <= maxRowIndex; i++) {
        for (int j = minColumnIndex; j <= maxRowIndex; j++) {
            blocks[bufferRows + i][j].setColor(sf::Color(255, 255, 255, 255));
        }
    }
}

void GameBoard::setTextures(String blocksFilePath, String digitsFilePath) {
    blocksTexture.loadFromFile(blocksFilePath);
    digitsTexture.loadFromFile(digitsFilePath);
}