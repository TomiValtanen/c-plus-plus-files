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
    void setMoney(int bet);
    int getMoney();

    string name;

    vector <Card*>* hand;
    vector <Card*>* dealerCard;
private:
    int money;
};

#endif // PLAYER_H
