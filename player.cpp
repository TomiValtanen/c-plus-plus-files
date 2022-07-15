#include "player.h"

Player::Player(string name,vector <Card*>* hand ,vector <Card*>* splithand,int money)
{
    this->name=name;
    this->hand=hand;
    this->splitHand=splithand;
    this->money=money;
    this->bet=0;

}
Player::Player(string name , vector <Card*>* hand,vector <Card*>* dealerCard){
    this->name=name;
    this->hand=hand;
    this->dealerCard=dealerCard;
    this->bet=0;
}

void Player::setMoney(int bet){
    money=money+bet;

}
int Player::getMoney(){
    return money;
}
void Player::setBet(int bet){
    this->bet=bet;

}
int Player::getBet(){
    return bet;
}
void Player::addBet(int bet){
    this->bet+=bet;
}
