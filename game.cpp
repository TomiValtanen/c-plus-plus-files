#include "game.h"


game::game()
{
string dealerName="Jakaja";
Deck* deck= new Deck();
vector <Card*>* playerHand= new vector <Card*>();
vector <Card*>* dealerHand= new vector <Card*>();

Player* player= new Player(name(),playerHand);
Player* dealer= new Player(dealerName,dealerHand);

startGame(player, dealer ,deck);
}

string game::name(){


    string name;
    cout<<"Pelinimesi : ";
    cin>>name;
    cout<<"Onnea matkaan"<<name<<"!\n\n";
    return name;
}
void game::startGame(Player* player, Player* dealer, Deck* deck){
    system("cls");
    Deck* d=deck;
    vector <Card*>* playingCards= d->getCards();

    pickCard(playingCards,player,2);
    pickCard(playingCards,dealer,2);

    showHands(player,dealer);
    bool blackjack=false;

    if(totalPoints(player)==21 && totalPoints(dealer)==21){
        cout<<"Tasapeli molemmilla on blackjack!\n\n";
        blackjack=true;
    }
    else if(totalPoints(player)==21){
        cout<<"Sinä sait blackjackin ja voitit!\n\n";
        blackjack=true;
    }
    else if (totalPoints(dealer)==21){
        cout<<"Jakaja sai blackjackin ja voitti!\n\n";
        blackjack=true;
    }
    else{
        blackjack=false;
    }

    if(blackjack==false){

    bool playerTurn=true;
    bool playerOverLimit=false;
    bool dealerOverLimit=false;

    do{

        int hit=hitOrStay();

    if(hit==1){
        system("cls");
        pickCard(playingCards,player,1);
        showHands(player,dealer);

        if(totalPoints(player)>21){
            cout<<"\n\nYlitit pisterajan ! \n Jakaja voitti!\n\n";
            playerOverLimit=true;
            break;
        }
    }
    else
    {
    playerTurn=false;
    }
   }
    while(playerTurn==true);

    bool dealerTurn=true;

    while(dealerTurn==true){

        if(playerOverLimit==true){
            break;
        }
        else if(totalPoints(dealer)>21){
            cout<<"Jakaja ylitti pisterajan!\nSina voitit!\n\n";
            dealerOverLimit=true;
            break;
        }
        else if(totalPoints(dealer)>=17){
            dealerTurn=false;

        }
        else{
            system ("cls");
            pickCard(playingCards,dealer,1);
            showHands(player,dealer);
        }
    }

    if(dealerOverLimit==true || playerOverLimit==true ){

    }
    else{
    winningConditions(player,dealer);
    }
    }


}

void game::pickCard(vector <Card*>* playingCards,Player* player , int numberOfCards){

    int number=numberOfCards;
    Player* p=player;
    vector <Card*>* d= playingCards;

    for(int i=0; i<number;i++){
        Card* c=d->front();
        p->hand->push_back(c);
        d->erase(d->begin());
    }
}

void game::showHands(Player* player, Player* dealer){

    hand(player);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(player)<<"\n\n";
cout<<"\n==================================\n";

    hand(dealer);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(dealer)<<"\n\n";
}
void game::hand(Player* player){
    Player* p=player;

    cout << p->name <<"n kortit ovat : \n" << endl;
    for(std::size_t i = 0; i < p->hand->size(); ++i){
    Card* c= (*p->hand)[i];

    cout << c->Face << " of " << c->Symbol <<"  | ";
    }
}
int game::totalPoints(Player* player){
    Player* p=player;
    int points=0;

    for (std::size_t i=0; i < p->hand->size();i++){
        Card* c=(*p->hand)[i];

        if(c->Face=="Ace"){
            points=points+11;
            if(points>21){
                points=points-11+1;
            }
            else{
                points=points+0;
            }

        }
        else{
            points+=c->Point;
        }
    }


    return points;

}
int game::hitOrStay(){
    int choose;
    bool validNumber=false;
    while(!validNumber){
    cout<<"Halutko ottaa uuden kortin vai jattaa ottamatta ? \n 1. Uusi kortti |  2. Ela nosta uutta korttia\n";
    cin>>choose;
    cout<<endl;
    if(choose==1){
        choose=1;
        validNumber=true;
    }
    else if (choose==2){
        choose=2;
        validNumber=true;
    }
    else{
        cout<<"syota vaadittu numero 1 tai 2.\n";
    }
    }
    return choose;
}

void game::winningConditions(Player* player,Player* dealer){
    int dp=totalPoints(dealer);
    int pp=totalPoints(player);
    if(dp>21){
            cout<<"Sina voitit!";
            cout<<"\n\n\n======================================================\n\n\n";
        }
    else if(pp>21){
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp>pp && dp<=21){
        cout<<"Jakaja Voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp<pp && pp<=21){
        cout<<"Sina Voitit!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp<21){
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp==21){
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
}
































