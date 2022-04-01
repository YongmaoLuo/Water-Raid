#include"common_data_structure.h"

class airplane{
private:
    char type,fuel;
    position pos;
    int score;
    bool isCrash;
    void fire();
    void move();
    void ifCrash();
    void addScore();
};