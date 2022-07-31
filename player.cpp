#include "player.h"

Player::Player(string name,vector <Card> hand ,vector <Card> splithand,int money)
{
    this->name=name;
    this->hand=hand;
    this->splitHand=splithand;
    this->money=money;
    this->bet=0;
    this->OriginalBet=0;
    this-> highScore=0;

}

Player::Player(string name , vector <Card> hand,vector <Card> dealerCard){
    this->name=name;
    this->hand=hand;
    this->dealerCard=dealerCard;
    this->bet=0;
    this->discardDeck=vector <Card>();
    this->rounds=0;
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

void Player::setOriginalBet(int bet){
    OriginalBet=bet;
}

int Player::getOriginalBet(){
    return OriginalBet;
}

void Player::setSplitBet(int bet){
    splitBet=bet;
}
void Player::addSplitBet(int bet){
    splitBet+=bet;
}
int Player::getSplitBet(){
    return splitBet;
}
void Player::setHighScore(int money){
    highScore=money;
}
int Player::getHighScore(){
    return highScore;
}

void Player::setCard(Card c,bool firsthand){

    if(firsthand==true){
    hand.push_back(c);
    }
    else{
        splitHand.push_back(c);
    }
}
void Player::setRound(int round){
    rounds=round;
}
void Player::addRound(int round){
    rounds+=round;
}
int Player::getRound(){
    return rounds;
}
