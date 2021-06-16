#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Tetromino {
public:
    static constexpr int placementGridSize{ 4 };
    static constexpr int totalBlockTypes{ 7 };
    static constexpr int initialXposition{ 3 };
    static constexpr int initialYposition{ -2 };
protected:
    static constexpr int numberOfBlocks{ 4 };
    static constexpr int spriteBlockSize{ 40 };
    static constexpr int xAxisDrawOffset{ 50 };
    static constexpr int types[totalBlockTypes][placementGridSize][placementGridSize]
     { {{0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 1, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 0, 0, 1},
        {0, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 0, 1, 1},
        {0, 0, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 0, 1, 0},
        {0, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}},

        {{0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}} };

    Sprite blocks[numberOfBlocks];
    int blockPlacements[placementGridSize][placementGridSize];
    int xPosition;
    int yPosition;
    int type;

    static Texture texture;

public:
    Tetromino(int type, int x, int y);

    Tetromino(const Tetromino& tetromino);

    void changePosition(int x, int y);

    int getXPosition() const;

    int getYPosition() const;

    int getType() const;

    bool isBlockPresent(int x, int y) const;

    void rotate();

    void draw(RenderWindow& window);

    void pause();

    void unpause();

    static void setTexture(String filePath);
};