#pragma once
#include <SFML/Graphics.hpp>
#include "Tetromino.h"
#include <iostream>

using namespace sf;

class GameBoard {
protected:
    static constexpr int rows = 20;
    static constexpr int bufferRows = 4;
    static constexpr int columns = 10;
    
    Sprite blocks[rows + bufferRows][columns];
    int score;
    int comboCounter;
    int linesCleared;

    static Texture blocksTexture;
    static Texture digitsTexture;
public:
    GameBoard();

    bool tetrominoMoveCollides(Tetromino& tetromino, int x, int y);

    bool tetrominoRotationCollides(Tetromino& tetromino);

    void addTetromino(Tetromino& tetromino);

    void draw(RenderWindow& window);

    void drawBlocks(RenderWindow& window);

    void drawScore(RenderWindow& window);

    void drawCombo(RenderWindow& window);

    void drawLinesCount(RenderWindow& window);

    void resetComboCounter();

    void checkRows(bool resetCombo);

    void deleteRow(int rowIndex);

    bool checkGameOver(Tetromino& tetromino);

    bool checkGameWon();

    void pause();

    void unpause();

    static void setTextures(String blocksFilePath, String digitsFilePath);
};



