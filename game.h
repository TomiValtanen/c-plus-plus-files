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
    int yesOrNo(bool Insurance , bool DoubleBet,bool split);
    void winningConditions(Player* player,Player* dealer,int bet);
    void faceDownDealerCard(vector <Card*>* playingCards,Player* player );
    void moveFaceDownCard(Player* dealer);
    bool checkBlackjack(Player* player );
    int betAmountCheck(Player* player);
    void money(bool win , int bet,Player* player);
    bool playerTurn(vector <Card*>* playingCards , Player* player,Player* dealer);
    bool dealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer);
    void busted(bool dealerOverLimit,bool playerOverLimit, int bet, Player* player, Player* dealer);
    void nextTurn(Player* player, Player* dealer,Deck* d);
    bool checkInsurance(Player* dealer);
    bool insurance(bool Insurance, int bet, Player* dealer, Player* player);
    bool doubleDown(Player* player,Player* dealer,int bet,vector <Card*>* playingCards, bool blackjack, bool split);
    bool splitCheck(Player* player,bool blackjack);
    void splittingHand(Player* player, Player* dealer,vector <Card*>* playingCards, int bet,bool Split);
    void splitHand(Player* player, vector <Card*>* splitHand);
    void showSplitHands(Player* player, Player* dealer, vector <Card*>* splitHand);
    int  splitTotalPoints( vector <Card*>* splithand);
    bool splitCheckBlackjack(vector <Card*>* splithand );
    bool splitDealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer,vector <Card*>* splithand );
    bool splitPlayerTurn(vector <Card*>* playingCards , Player* player,Player* dealer,vector <Card*>* splithand,bool FirstHand );
    void splitPickCard(vector <Card*>* playingCards, int numberOfCards, vector <Card*>* splithand);
    bool splitDoubleDown(Player* player,Player* dealer,int bet,vector <Card*>* playingCards, bool blackjack ,vector <Card*>* splithand, bool firstHand);
    void splitWinningConditions(Player* player,Player* dealer, int bet,vector <Card*>* splithand , bool firstHand);
    void splitBusted(bool dealerOverLimit,bool playerOverLimit, int bet, Player* player, Player* dealer,vector <Card*>* splithand, bool firsthand);


};

#endif // GAME_H
