#include "player.h"

Player::Player(string name,vector <Card*>* hand )
{
    this->name=name;
    this->hand=hand;
    this->money=100;

}
Player::Player(string name , vector <Card*>* hand,vector <Card*>* dealerCard){
    this->name=name;
    this->hand=hand;
    this->dealerCard=dealerCard;
}
