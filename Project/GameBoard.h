#pragma once
#include <SFML/Graphics.hpp>
#include "Tetromino.h"
#include <iostream>

using namespace sf;

class GameBoard {
protected:
    static constexpr int rows{ 20 };
    static constexpr int bufferRows{ 4 };
    static constexpr int columns{ 10 };
    static constexpr int minRowIndex{ 0 };
    static constexpr int maxRowIndex{ 19 };
    static constexpr int minColumnIndex{ 0 };
    static constexpr int maxColumnIndex{ 9 };
    static constexpr int spriteBlockSize{ 40 };
    static constexpr int xAxisBlockDrawOffset{ 50 };
    static constexpr int xAxisScoreDrawOffset{ 660 };
    static constexpr int yAxisScoreDrawOffset{ 80 };
    static constexpr int xAxisComboDrawOffset{ 660 };
    static constexpr int yAxisComboDrawOffset{ 240 };
    static constexpr int xAxisLinesCountDrawOffset{ 660 };
    static constexpr int yAxisLinesCountDrawOffset{ 400 };
    static constexpr int singleLineScore{ 10 };
    static constexpr int doubleLineScore{ 30 };
    static constexpr int tripleLineScore{ 50 };
    static constexpr int tetrisScore{ 80 };
    static constexpr int comboScoreMultiplier{ 5 };
    static constexpr int maxScore{ 99999 };
    
    Sprite blocks[rows + bufferRows][columns];
    int score;
    int comboCounter;
    int linesCleared;

    static Texture blocksTexture;
    static Texture digitsTexture;
public:
    GameBoard();

    bool tetrominoMoveCollides(Tetromino& tetromino, int x, int y) const;

    bool tetrominoRotationCollides(Tetromino& tetromino) const;

    void addTetromino(Tetromino& tetromino);

    void draw(RenderWindow& window);

    void drawBlocks(RenderWindow& window);

    void drawScore(RenderWindow& window);

    void drawCombo(RenderWindow& window);

    void drawLinesCount(RenderWindow& window);

    void resetComboCounter();

    void checkRows(bool resetCombo);

    void deleteRow(int rowIndex);

    bool checkGameOver(Tetromino& tetromino) const;

    bool checkGameWon() const;

    void pause();

    void unpause();

    static void setTextures(String blocksFilePath, String digitsFilePath);
};



