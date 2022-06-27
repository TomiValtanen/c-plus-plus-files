#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include "deck.h"

using namespace std;

class Player
{
public:
    Player(string name , vector <Card*>* hand );

    string name;
    int totalPoints;
    vector <Card*>* hand;
};

#endif // PLAYER_H
