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
#include <conio.h>



class game
{
public:
    game();
    string name();
    void startGame(Player* player, Player* dealer, Deck* deck);
    void pickCard(vector <Card*>* playingCards,Player* player, int numberOfCards,bool Firsthand);
    void showHands(Player* player, Player* dealer,bool split);
    void hand(Player* player,bool firsthand);
    int totalPoints(Player* player,bool firsthand);
    int yesOrNo(bool Insurance , bool DoubleBet,bool split);
    void winningConditions(Player* player,Player* dealer,int bet,bool split,bool firsthand);
    void faceDownDealerCard(vector <Card*>* playingCards,Player* player );
    void moveFaceDownCard(Player* dealer);
    bool checkBlackjack(Player* player,bool firsthand  );
    void betAmountCheck(Player* player);
    void money(bool win , int bet,Player* player);
    bool playerTurn(vector <Card*>* playingCards , Player* player,Player* dealer,bool split,bool firsthand);
    bool dealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer,bool split);
    void busted(bool dealerOverLimit,bool playerOverLimit, Player* player, Player* dealer,bool split,bool firsthand);
    void nextTurn(Player* player, Player* dealer,Deck* d);
    bool checkInsurance(Player* dealer);
    bool insurance(bool Insurance,Player* dealer, Player* player);
    bool doubleDown(Player* player,Player* dealer,vector <Card*>* playingCards, bool blackjack, bool split,bool Firsthand);
    bool splitCheck(Player* player,bool blackjack);
    void splittingHand(Player* player, Player* dealer,vector <Card*>* playingCards,bool Split);
    void printBlackjack(Player* player);
    void playerDealerbet(Player* player, Player* dealer);


};

#endif // GAME_H
