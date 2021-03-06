#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include "deck.h"

using namespace std;

class Player
{
public:
    Player(string name , vector <Card> hand,vector <Card> splithand ,int money);
    Player(string name , vector <Card> hand,vector <Card> dealerCard);

    void setMoney(int bet);
    int getMoney();

    void setBet(int bet);
    void addBet(int bet);
    int getBet();

    void setOriginalBet(int bet);
    int getOriginalBet();

    void setSplitBet(int bet);
    void addSplitBet(int bet);
    int getSplitBet();

    void setHighScore(int money);
    int getHighScore();

    void setCard(Card c,bool firsthand);

    void setRound(int round);
    void addRound(int round);
    int getRound();

    string name;
    vector <Card> discardDeck;
    vector <Card> hand;
    vector <Card> splitHand;
    vector <Card> dealerCard;
private:
    int money;
    int bet;
    int OriginalBet;
    int splitBet;
    int highScore;
    int rounds;
};

#endif // PLAYER_H
