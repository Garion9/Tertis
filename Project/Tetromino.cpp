#include "Tetromino.h"


Texture Tetromino::texture = Texture();


Tetromino::Tetromino(int type, int x, int y) {
    for (int i = 0; i < 4; i++) {
        Sprite sprite = Sprite(texture);
        sprite.setTextureRect(IntRect(type * 40, 0, 40, 40));
        blocks[i] = sprite;
    }
    xPosition = x;
    yPosition = y;
    std::copy(&Tetromino::types[type][0][0], &Tetromino::types[type][0][0] + 16, &blockPlacements[0][0]);
    this->type = type;
}

Tetromino::Tetromino(const Tetromino& tetromino) {
    for (int i = 0; i < 4; i++) {
        Sprite sprite = Sprite(texture, IntRect(tetromino.type * 40, 0, 40, 40));
        blocks[i] = sprite;
    }
    xPosition = tetromino.xPosition;
    yPosition = tetromino.yPosition;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            blockPlacements[i][j] = tetromino.blockPlacements[i][j];
        }
    }
    this->type = tetromino.type;
}

void Tetromino::changePosition(int x, int y) {
    xPosition += x;
    yPosition += y;
}

int Tetromino::getXPosition() { return xPosition; }

int Tetromino::getYPosition() { return yPosition; }

int Tetromino::getType() { return type; }

bool Tetromino::isBlockPresent(int x, int y) {
    if (blockPlacements[y][x] == 1) return true;
    else return false;
}

void Tetromino::rotate() {
    if (type != 3) {
        int N = 3;
        for (int i = 0; i < N / 2; i++) {
            for (int j = 0; j < N - i - 1; j++) {
                int temp = blockPlacements[i][j + 1];
                blockPlacements[i][j + 1] = blockPlacements[N - 1 - j][i + 1];
                blockPlacements[N - 1 - j][i + 1] = blockPlacements[N - 1 - i][N - j];
                blockPlacements[N - 1 - i][N - j] = blockPlacements[j][N - i];
                blockPlacements[j][N - i] = temp;
            }
        }
        int temp = blockPlacements[1][0];
        blockPlacements[1][0] = blockPlacements[3][2];
        blockPlacements[3][2] = temp;
    }
}

void Tetromino::draw(RenderWindow& window) {
    int spriteIndex = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockPlacements[i][j] == 1) {
                blocks[spriteIndex].setPosition(50 + (xPosition + j) * 40, (yPosition + i) * 40);
                window.draw(blocks[spriteIndex]);
                spriteIndex++;
            }
        }
    }
}

void Tetromino::pause() {
    for (int i = 0; i < 4; i++) {
        blocks[i].setColor(sf::Color(255, 255, 255, 128));
    }
}

void Tetromino::unpause() {
    for (int i = 0; i < 4; i++) {
        blocks[i].setColor(sf::Color(255, 255, 255, 255));
    }
}

void Tetromino::setTexture(String filePath) {
    texture.loadFromFile(filePath);
}