#ifndef CARD_H
#define CARD_H

#include <iostream>

using namespace std;

class Card
{
public:
    Card();
    Card(string Face,string Symbol,int Point);

    string Face;
    string Symbol;
    int Point;
};

#endif // CARD_H
