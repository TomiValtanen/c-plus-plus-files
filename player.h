#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include "deck.h"

using namespace std;

class Player
{
public:
    Player(string name , vector <Card*>* hand );
    Player(string name , vector <Card*>* hand,vector <Card*>* dealerCard);

    string name;
    int money;
    vector <Card*>* hand;
    vector <Card*>* dealerCard;
};

#endif // PLAYER_H
