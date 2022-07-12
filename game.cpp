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
        //split silloinku kortit ovat samat.
        Player* p=player;
        Card* c=(*p->hand)[0];
        Card* c1=(*p->hand)[1];
        vector <Card*>* d= playingCards;
        if(c->Face==c1->Face){
            vector <Card*>* splitHand=new vector <Card*>();
            splitHand->push_back(c);
            p->hand->erase(p->hand->begin());
            delete c1;
            cout << p->name <<"n kortit ovat splitkadessa 1: \n" << endl;
            for(std::size_t i = 0; i < p->hand->size(); ++i){
                Card* c= (*p->hand)[i];
                Card* c1 =(*splitHand)[i];

                cout << c->Face << " of " << c->Symbol <<"  | ";
                cout << p->name <<"n kortit ovat splitkadessa 2: \n" << endl;
                cout << c1->Face << " of " << c1->Symbol <<"  | ";
                cout<<"Jaetaan molempiin kasiin toiset kortit.";
            }
                for(int i=0; i<1;i++){
                    Card* c=d->front();
                    p->hand->push_back(c);
                    d->erase(d->begin());

                }
                for(int i=0; i<1;i++){
                    Card* c=d->front();
                    splitHand->push_back(c);
                    d->erase(d->begin());
                }
                system("cls");
                cout << p->name <<"n kortit ovat splitkadessa 1: \n" << endl;
                for(std::size_t i = 0; i < p->hand->size(); ++i){
                    Card* c= (*p->hand)[i];
                    cout << c->Face << " of " << c->Symbol <<"  | ";


            }
                cout << p->name <<"n kortit ovat splitkadessa 2: \n" << endl;
                for(std::size_t i = 0; i < splitHand->size(); ++i){
                    Card* c1 =(*splitHand)[i];
                    cout << c1->Face << " of " << c1->Symbol <<"  | ";


        }
        }

        //Pelaajan ottamien korttien jälkeen annetaan mahdollisuus ottaa insurance , jos jakajalla on nakyva kortti assa.
        bool insu=checkInsurance(dealer);
        bool insuBlackJack=insurance(insu,bet,dealer,player);

        //Double down ensimmäisten kahden kortin summa on 9 , 10 tai 11.
        bool doubleCheck=doubleDown(player,dealer,bet,playingCards,insuBlackJack);
        if(doubleCheck==true){
        int doubleBet=bet*2;
         moveFaceDownCard(dealer);
         //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
         bool dealerOverLimit=dealerTurn(false,playingCards,player,dealer);
         //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
         busted(dealerOverLimit,false,doubleBet,player,dealer);
        }

        if(doubleCheck==false && insuBlackJack==false){

        //Nostetaan kortteja sen verran ,että "Bust" tai pelaaja päättää itse lopettaa korttien nostamisen. Palauttaa arvon true / false.
        bool playerOverLimit=playerTurn(playingCards,player,dealer);



        //Jakaja aloittaa korttien ottamisen , jos blackjackkia ei ollut tai pelaajan ei bustannut.
        if(playerOverLimit==false){

         // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
            moveFaceDownCard(dealer);
        }
        //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
        bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer);
        //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
        busted(dealerOverLimit,playerOverLimit,bet,player,dealer);

    }
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
int game::yesOrNo (bool Insurance, bool DoubleBet){
    int choose;
    bool validNumber=false;
    bool insurance=Insurance;
    bool doubleBet=DoubleBet;

    if(doubleBet==true){
        cout<<"Haluatko double down? \n 1. Kylla |  2. En\n";
    }
    else if(insurance==true){
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

        int hit=yesOrNo(false,false);

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
    int checkMoney=bet+insuranceBet;

    if(insurance==true){

        cout<<"Jakajalla on nakyva kortti assa haluatko ottaa insurancen ja katsoa onko toinen kortti 10? \nOlet antanut talle kierrokselle "<< bet << " euron panoksen. \nInsurance on puolet antamastasi panoksesta "<< bet/2<<" euroa.";
        int chooce=yesOrNo(insurance,false);
        if(chooce==1 && player->getMoney()>=checkMoney){
           Player* d= dealer;
           Card* c= (*d->dealerCard)[0];
           if(c->Point==10){
               blackjackDealer=true;
           }
           if(blackjackDealer==true){
               bool insuranceWin=true;
               bool normalWin=false;
               moveFaceDownCard(dealer);
               showHands(player,dealer);
               cout<<"Jakajalla on toinen kortti 10 arvoinen eli jakajalla on blackjack. Insurancelle asettamasi panos oli"<< insuranceBet <<" euroa.\nSaat taman tuplattuna takaisin "<<bet<<" euroa.";
               cout<<"Koska jakajalla on blackjack haviat alkuperaisen panoksesi"<< bet <<" euroa.";
               money(insuranceWin,bet,player);
               money(normalWin,bet,player);

               cout << "\n \nPaina Enter jatkaaksesi.";
               cin.ignore();
               cin.ignore();
           }
           else{
               bool insuranceWin=false;
               cout<<"Jakajan toinen kortti ei ollut 10, joten hävisit"<< bet/2 <<" euron insurancen. Peli jatkuu normaalisti";
               money(insuranceWin,insuranceBet,player);
               cout << "\n \nPaina Enter jatkaaksesi.";
               cin.ignore();
               cin.ignore();

        }
    }
        else{
            if(player->getMoney() < checkMoney)
            {
                blackjackDealer=false;
                cout<<"Sinulla ei ollut tarpeeksi rahaa laittaa insurancea. Peli jatkuu normaalisti.";
                cout << "\n \nPaina Enter jatkaaksesi.";
                cin.ignore();
                cin.ignore();
            }
                else {
                  blackjackDealer=false;
                }
        }
}

    return blackjackDealer;
}

bool game::doubleDown(Player* player,Player* dealer,int bet,vector <Card*>* playingCards, bool blackjack ){

    bool doubleDown=false;
    if(totalPoints(player)<=11&&totalPoints(player)>=9 && blackjack==false){

       int chooce=yesOrNo(false,true);
       if(chooce==1){
           cout<<"Olet laittanut panokseksi "<<bet<<" euroa. Olet laittamassa tuplana alkuperaisen panoksesi.";
           int doubleBet=bet*2;
           if(player->getMoney()>=doubleBet){
               cout<<"Sinulla on tarpeeksi rahaa tehdä double down. Jakaja jakaa sinulle nyt viimeisen kortin.";
               pickCard(playingCards,player,1);
               showHands(player,dealer);
               doubleDown=true;
           }
           else{
               cout<<"Sinulla ei ole tarpeeksi rahaa double down, mutta voit jatkaa normaalisti korttien ottamista.";
               doubleDown=false;
           }
       }
    else{
        doubleDown=false;
    }
    }
    return doubleDown;
}



























