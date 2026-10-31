//
//  ScoreManager.h
//  Catris
//
//  Created by Maria Goncharuk on 29.10.2026.
//

#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager {
private:
    int score = 0;
    int highScore = 0;
    int lines = 0;

public:
    int getScore() const {return score;}
    int getHighScore() const {return highScore;}
    int getLines() const {return lines;}
    int getLevel() const {return lines / 10 + 1;}

    void addPoints(int points) {score += points;}

    void addLineClear(int cleared) {
        static const int table[5] = {0, 100, 300, 500, 800};
        if (cleared < 1 || cleared > 4) return;
        score += table[cleared] * getLevel();
        lines += cleared;
    }

    bool isNewRecord() const {return score > highScore;}
    void setHighScore(int value) {highScore = value;}
    void restore(int savedScore, int savedLines) {score = savedScore; lines = savedLines;}
    void reset() {score = 0; lines = 0;}   
};

#endif
