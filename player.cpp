#include "player.h"

Player::Player(string name,vector <Card*>* hand )
{
    this->name=name;
    this->hand=hand;
    this->totalPoints=0;

}
