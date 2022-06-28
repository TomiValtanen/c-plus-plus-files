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
    void winningConditions(Player* player,Player* dealer);


};

#endif // GAME_H
