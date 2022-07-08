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
    sleep(2);
    return name;

}
void game::startGame(Player* player, Player* dealer, Deck* deck){
    system("cls");
    Deck* d=deck;
    vector <Card*>* playingCards= d->getCards();



    cout<<"Sinulla on rahaa "<< player->getMoney() <<" euroa\n";
    cout<<"Pienin osallistumis maksu on 10 euroa ja suurin 100 euroa.\nPaljonko tahdot panostaa?\n";
    int bet=betAmountCheck(player);

    //Jaetaan aloitus kortit
    pickCard(playingCards,player,2);
    pickCard(playingCards,dealer,1);

    // Antaa jakajalle kortin mitä ei näytetä, ennenkuin jakaja aloittaa korttien nostamisen.
    faceDownDealerCard(playingCards,dealer);

    showHands(player,dealer);
    //Tarkistaa onko pelaajalla tai jakalla Blackjack.
    bool blackjack=checkBlackjack(player);
    if (blackjack==true){
       moveFaceDownCard(dealer);
       bool blackjackDealer=checkBlackjack(dealer);
       showHands(player, dealer);

       if(blackjack==true && blackjackDealer==true){
           cout<<"Tasapeli molemmilla on BlackJack!\nVoitit "<<bet*2;
           player->setMoney(bet);
           cout << "\n \nPaina Enter jatkaaksesi.";
           cin.ignore();
           cin.ignore();
          }
               else {


        cout<<"Sinä sait blackjackin ja voitit "<< bet+bet*1.5 <<" \n\n";
        int bj=bet*1.5;
        player->setMoney(bj);
        cout << "\n \nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();

    }
    }

    else{

        //Nostetaan kortteja sen verran ,että "Bust" tai pelaaja päättää itse lopettaa korttien nostamisen. Palauttaa arvon true / false.
        bool playerOverLimit=playerTurn(playingCards,player,dealer);

        //Pelaajan ottamien korttien jälkeen annetaan mahdollisuus ottaa insurance , jos jakajalla on nakyva kortti assa.
        bool insu=checkInsurance(dealer);
        bool insuBlackJack=insurance(insu,bet,dealer,player);



        //Jakaja aloittaa korttien ottamisen , jos blackjackkia ei ollut tai pelaajan ei bustannut.
        if(playerOverLimit==false&& insuBlackJack==false){

         // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
            moveFaceDownCard(dealer);
        }
        //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
        bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer);
        //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
        busted(dealerOverLimit,playerOverLimit,bet,player,dealer);

    }

    //Tarkistetaan onko pelaajalla kyseistä summaa rahaa osallistua seuraavalle kierrokselle.
    if(player->getMoney()>=10){

    nextTurn(player, dealer, d);
    }
    else{
        cout<<"Voi harmi sinulla ei ole enaa rahaa seuraavalle kierrokselle vahimmaismaara panostukselle on 10 euroa. Sinulla on "<< player->getMoney()<<" euroa.\n\n";
        sleep(2);
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
    delete c;
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
int game::yesOrNo(bool Insurance){
    int choose;
    bool validNumber=false;
    bool insurance=Insurance;
    if(insurance==true){
        cout<<"Halutko ottaa insurancen ? \n 1. Kylla |  2. En\n";
    }
    else{

    cout<<"Halutko ottaa uuden kortin vai jattaa ottamatta ? \n 1. Uusi kortti |  2. Ela nosta uutta korttia\n";
    }
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

void game::winningConditions(Player* player,Player* dealer, int bet){
    int dp=totalPoints(dealer);
    int pp=totalPoints(player);
    int b=bet;
    bool win;
    system("cls");
    if(dp>21){
        win=true;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(pp>21){
        win=false;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit"<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp>pp && dp<=21){
        win=false;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp<pp && pp<=21){
        win=true;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp<21){
        win=false;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp==21){
        win=false;
        showHands(player,dealer);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
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
int game::betAmountCheck(Player* player){
    Player* p=player;
    int  bet;
    bool validNumber=false;
    cout<<"Kirjoita panoksesi :";
    do{
        cin>>bet;
        if(bet<10 || bet>100){
            cout<<"Pienin mahdollinen panostus on 10 euroa ja suurin mahdollinen 100 euroa!";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (bet > p->getMoney()){
            cout<<"Sinulla ei ole noin paljon rahaa! Sinulla on rahaa "<<p->getMoney()<<"\n";
        }
        else {
            validNumber=true;
        }
    }
    while(validNumber==false);
    return bet;
}

void game::money(bool win , int bet, Player* player){
    Player* p=player;
    bool w=win;
    int b=bet;

    if(w==true){
        p->setMoney(b);
    }
    else {
        p->setMoney(-b);
    }
}

bool game::playerTurn(vector <Card*>* playingCards , Player* player,Player* dealer){
    bool playerOverLimit=false;
    bool playerTurn=true;
    do{

        int hit=yesOrNo(false);

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

    return playerOverLimit;
}

bool game::dealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer){

    bool dealerOverLimit=false;
    bool dealerTurn=true;
    bool playerOverLimit=PlayerOverlimit;
    while(dealerTurn==true){

        if(playerOverLimit==true){
            break;
        }
        else if(totalPoints(dealer)>21){
            cout<<"Jakaja ylitti pisterajan!\nSina voitit!\n\n";
            dealerOverLimit=true;
            dealerTurn=false;
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
    return dealerOverLimit;
}
void game::busted(bool dealerOverLimit,bool playerOverLimit, int bet, Player* player, Player* dealer){

    if (dealerOverLimit==true){
        bool win=true;
        money(win,bet,player);
        cout<<"Voitit"<< bet+bet<<" !\nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();

    }
    else if(playerOverLimit==true){
        bool win=false;
        money(win,bet,player);
        cout<<"Havisit "<< bet << " !\nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();
    }
    else{
        winningConditions(player,dealer,bet);
        cout << "\n \nPaina Enter jatkaaksesi.";
        cin.ignore();
        cin.ignore();
    }

}

void game::nextTurn(Player* player, Player* dealer,Deck* d){

    delete dealer;
    delete d;
    player->hand->clear();
    Deck* deck= new Deck();
    string dealerName="Jakaja";
    vector <Card*>* dealerHand= new vector <Card*>();
    vector <Card*>* dealerCard=new vector <Card*>();
    Player* Dealer= new Player(dealerName,dealerHand,dealerCard);
    startGame(player,Dealer,deck);
}
bool game::checkInsurance(Player* dealer){
   bool insurance=false;
   Player* d=dealer;
   Card* c= (*d->hand)[0];

   if(c->Face=="Ace"){
       insurance=true;
   }
   return insurance;
}

bool game::insurance(bool Insurance, int bet, Player* dealer, Player* player){
    bool insurance=Insurance;
    bool blackjackDealer=false;
    int insuranceBet=bet/2;
    if(insurance==true){

        cout<<"Jakajalla on nakyva kortti assa haluatko ottaa insurancen ja katsoa onko toinen kortti 10? \nOlet antanut talle kierrokselle "<< bet << " euron panoksen. \nInsurance on puolet antamastasi panoksesta "<< bet/2<<" euroa.";
        int chooce=yesOrNo(insurance);
        if(chooce==1){
           moveFaceDownCard(dealer);
           showHands(player,dealer);
            blackjackDealer=checkBlackjack(dealer);
           if(blackjackDealer==true){
               bool win=true;
               cout<<"Jakajalla on toinen kortti 10 arvoinen eli jakajalla on blackjack. Insurancelle asettamasi panos oli"<< insuranceBet <<" euroa.\nSaat taman tuplattuna takaisin "<<bet<<" euroa.";
               money(win,bet,player);
               cout << "\n \nPaina Enter jatkaaksesi.";
               cin.ignore();
               cin.ignore();
           }
           else{
               bool win=false;
               cout<<"Jakajan toinen kortti ei ollut 10, joten hävisit"<< bet/2 <<" euron insurancen.";
               money(win,insuranceBet,player);
               cout << "\n \nPaina Enter jatkaaksesi.";
               cin.ignore();
               cin.ignore();

        }
    }
        else{
            blackjackDealer=false;
        }
}

    return blackjackDealer;
}



























