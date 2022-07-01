#include "game.h"


game::game()
{
string dealerName="Jakaja";
Deck* deck= new Deck();
vector <Card*>* playerHand= new vector <Card*>();
vector <Card*>* dealerHand= new vector <Card*>();
vector <Card*>* dealerCard=new vector <Card*>();

Player* player= new Player(name(),playerHand);
Player* dealer= new Player(dealerName,dealerHand,dealerCard);

startGame(player, dealer ,deck);
}

string game::name(){


    string name;
    cout<<"Pelinimesi : ";
    cin>>name;
    cout<<"Onnea matkaan"<<name<<" !\n\n";
    //sleep(2);
    return name;

}
void game::startGame(Player* player, Player* dealer, Deck* deck){
    system("cls");
    Deck* d=deck;
    vector <Card*>* playingCards= d->getCards();

    cout<<"Sinulla on rahaa "<<player->money<<" euroa\n";
    cout<<"Osallistumis maksu on 10 euroa\n\n";
    player->money=player->money-10;

    pickCard(playingCards,player,2);
    pickCard(playingCards,dealer,1);

    // Antaa jakajalle kortin mitä ei näytetä, ennenkuin jakaja aloittaa korttien nostamisen.
    faceDownDealerCard(playingCards,dealer);

    showHands(player,dealer);
    bool blackjack=checkBlackjack(player);
    if (blackjack==true){
        cout<<"Sinä sait blackjackin ja voitit!\n\n";
    }

    else{

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

// näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.

    if(dealerTurn==true &&playerOverLimit==false){

    moveFaceDownCard(dealer);
}

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
    if (dealerOverLimit==true || playerOverLimit==true){
        cout << "\n \nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();
    }
    else{
        winningConditions(player,dealer);
        cout << "\n \nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();
    }


    }


    if(player->money>0){
        delete dealer;
        delete d;
        player->hand->clear();
        Deck* deck1= new Deck();
        string dealerName="dealer";
        vector <Card*>* dealerHand= new vector <Card*>();
        vector <Card*>* dealerCard=new vector <Card*>();
        Player* dealer1= new Player(dealerName,dealerHand,dealerCard);
        startGame(player,dealer1,deck1);}


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

void game::faceDownDealerCard(vector <Card*>* playingCards,Player* dealer ){
    Player* Dealer=dealer;
    vector <Card*>* d= playingCards;

    Card* c=d->front();
    Dealer->dealerCard->push_back(c);
    d->erase(d->begin());
}

void game::moveFaceDownCard(Player* dealer){
    Player* Dealer=dealer;
    system ("cls");
    Card* c =Dealer->dealerCard->front();
    Dealer->hand->push_back(c);
    delete Dealer->dealerCard;
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
    int aces=0;
    bool ace=false;

    for (std::size_t i=0; i < p->hand->size();i++){
        Card* c=(*p->hand)[i];

        if(c->Face=="Ace"){
            points=points+11;
            aces+=1;
            ace=true;
        }
        else{
            points+=c->Point;
        }
    }
    if (ace==true && points>21){
        for (int i=0; i<aces;i++){
            points=points-10;
        }
    }

    return points;

}
int game::hitOrStay(){
    int choose;
    bool validNumber=false;
    cout<<"Halutko ottaa uuden kortin vai jattaa ottamatta ? \n 1. Uusi kortti |  2. Ela nosta uutta korttia\n";
    do{
    cin>>choose;
    cout<<endl;
  switch (choose){
  case 1:
      validNumber=true;
      break;
  case 2:
      validNumber=true;
      break;
  default:
      cout<<"syota vaadittu numero 1 tai 2.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    }
    while(validNumber==false);
return choose;
}

void game::winningConditions(Player* player,Player* dealer){
    int dp=totalPoints(dealer);
    int pp=totalPoints(player);
    system("cls");
    if(dp>21){
        showHands(player,dealer);
            cout<<"Sina voitit!";
            cout<<"\n\n\n======================================================\n\n\n";
        }
    else if(pp>21){
        showHands(player,dealer);
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp>pp && dp<=21){
        showHands(player,dealer);
        cout<<"Jakaja Voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp<pp && pp<=21){
        showHands(player,dealer);
        cout<<"Sina Voitit!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp<21){
        showHands(player,dealer);
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp==21){
        showHands(player,dealer);
        cout<<"Jakaja voitti!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
}

bool game::checkBlackjack(Player* player ){
    bool blackjack;

     if(totalPoints(player)==21){
        blackjack=true;
    }
    else{
        blackjack=false;
    }
    return blackjack;
}
































