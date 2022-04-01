#include"common_data_structure.h"

class airplane{
private:
    char type,fuel;
    position pos;
    int score;
    bool isCrash;
    virtual void fire();
    virtual void move();
    virtual void ifCrash();
    virtual void addScore();
};