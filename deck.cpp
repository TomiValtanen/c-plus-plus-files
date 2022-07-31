#include "deck.h"

Deck::Deck()
{   this->gameDeck=vector <Card>();
    createDeck();
    shuffle();
}

vector<Card> Deck::getCards(){
    return gameDeck;
}

void Deck::createDeck(){
    string Face[]{"Ace","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN","JACK","QUEEN","KING"};
    string Symbol[]{"HEARTS","DIAMONDS", "CLUBS","SPADES"};
    int Point[]{11,2,3,4,5,6,7,8,9,10,10,10,10};



    for (int i=0;i<4;i++){
        for(int j=0;j<13;j++){
            string Faces=Face[j];
            string Symbols=Symbol[i];
            int Points=Point[j];

            Card c=Card(Faces,Symbols,Points);
            gameDeck.push_back(c);

        }
    }
}

//Testaamiseen
void Deck::printCard(int i){

    Card c = gameDeck[i];
    cout << c.Face << " of " << c.Symbol << " | " << c.Point << endl;
}

void Deck::shuffle()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(this->gameDeck.begin(), this->gameDeck.end(), std::default_random_engine(seed));
}

Deck::Deck(vector <Card>* playingCards){
    for (std::size_t i=0; i <playingCards->size();i++){
    Card c=(*playingCards)[i];
    gameDeck.push_back(c);
    }
}

Deck::Deck(vector <Card>* playingCards,vector <Card> discardDeck){

        for(std::size_t i=0;i<discardDeck.size();i++){
            Card c=discardDeck[i];
            playingCards->push_back(c);
        }
        for (std::size_t i=0; i <playingCards->size();i++){
        Card c=(*playingCards)[i];
        gameDeck.push_back(c);
    }
        shuffle();
}

