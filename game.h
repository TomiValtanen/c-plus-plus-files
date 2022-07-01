#ifndef GAME_H
#define GAME_H
#include "card.h"
#include "deck.h"
#include "player.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include<limits>
#include <cstdlib>
#include <unistd.h>



class game
{
public:
    game();
    string name();
    void startGame(Player* player, Player* dealer, Deck* deck);
    void pickCard(vector <Card*>* playingCards,Player* player, int numberOfCards);
    void showHands(Player* player, Player* dealer);
    void hand(Player* player);
    int totalPoints(Player* player);
    int hitOrStay();
    void winningConditions(Player* player,Player* dealer,int bet);
    void faceDownDealerCard(vector <Card*>* playingCards,Player* player );
    void moveFaceDownCard(Player* dealer);
    bool checkBlackjack(Player* player );
    int betAmountCheck(Player* player);
    void money(bool win , int bet,Player* player);


};

#endif // GAME_H
