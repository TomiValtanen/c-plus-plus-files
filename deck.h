#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

class Deck
{
public:
    Deck();
    Deck(vector <Card>* playingCards);
    Deck(vector <Card>* playingCards,vector <Card> discardDeck);
    void createDeck();
    vector<Card> getCards();

    //Testaamiseen
    void printCard(int i);
    void shuffle();


private:
    vector<Card> gameDeck;

};

#endif // DECK_H
