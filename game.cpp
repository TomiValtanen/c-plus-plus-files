#include "game.h"


game::game()
{
    string dealerName="Jakaja";
    Deck* deck= new Deck();
    vector <Card*>* playerHand= new vector <Card*>();
    vector <Card*>* dealerHand= new vector <Card*>();
    vector <Card*>* dealerCard=new vector <Card*>();
    vector <Card*>* splithand=new vector <Card*>();

    Player* player= new Player(name(),playerHand,splithand,100);
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
    printBlackjack(player);
    Deck* d=deck;
    vector <Card*>* playingCards= d->getCards();



    cout<<"Sinulla on rahaa "<< player->getMoney() <<" euroa\n";
    cout<<"Pienin osallistumis maksu on 10 euroa ja suurin 100 euroa.\nPaljonko tahdot panostaa?\n";
    betAmountCheck(player);
    playerDealerbet(player,dealer);

    //Jaetaan aloitus kortit
    pickCard(playingCards,player,2,true);
    pickCard(playingCards,dealer,1,true);

    // Antaa jakajalle kortin mitä ei näytetä, ennenkuin jakaja aloittaa korttien nostamisen.
    faceDownDealerCard(playingCards,dealer);

    showHands(player,dealer,false);
    //Tarkistaa onko pelaajalla tai jakalla Blackjack.
    bool blackjack=checkBlackjack(player,true);
    int bet=player->getBet();
    if (blackjack==true){

        moveFaceDownCard(dealer);
        bool blackjackDealer=checkBlackjack(dealer,true);
        showHands(player, dealer,false);

        if(blackjack==true && blackjackDealer==true){
            cout<<"Tasapeli molemmilla on BlackJack!\nVoitit "<<bet;
            player->setMoney(bet);
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
        }
        else {


            cout<<"Sinä sait blackjackin ja voitit "<<bet*1.5 <<" \n\n";
            int bj=bet*1.5;
            player->setMoney(bj);
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();

        }
    }

    else{
        //Pelaajan ottamien korttien jälkeen annetaan mahdollisuus ottaa insurance , jos jakajalla on nakyva kortti assa.
        bool insu=checkInsurance(dealer);
        bool insuBlackJack=insurance(insu,dealer,player);

        bool split=splitCheck(player,insuBlackJack);
        if(split==true){
        splittingHand(player,dealer,playingCards,split);

        }
         else if(insuBlackJack==false && split==false){
        //Double down ensimmäisten kahden kortin summa on 9 , 10 tai 11.
        bool doubleCheck=doubleDown(player,dealer,playingCards,insuBlackJack,split,true);
        if(doubleCheck==true){

            moveFaceDownCard(dealer);
            //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
            bool dealerOverLimit=dealerTurn(false,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,false,player,dealer,split,true);
        }

        else{

            //Nostetaan kortteja sen verran ,että "Bust" tai pelaaja päättää itse lopettaa korttien nostamisen. Palauttaa arvon true / false.
            bool playerOverLimit=playerTurn(playingCards,player,dealer,split,true);



            //Jakaja aloittaa korttien ottamisen , jos blackjackkia ei ollut tai pelaajan ei bustannut.
            if(playerOverLimit==false){

                // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
                moveFaceDownCard(dealer);
            }
            //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true);

        }
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
void game::pickCard(vector <Card*>* playingCards,Player* player , int numberOfCards ,bool Firsthand){
    bool firsthand=Firsthand;
    int number=numberOfCards;
    Player* p=player;
    vector <Card*>* d= playingCards;
    vector <Card*>* Hand;
    if(firsthand==true){
        Hand=p->hand;
    }
    else{
        Hand=p->splitHand;
    }
    for(int i=0; i<number;i++){
        Card* c=d->front();
       Hand->push_back(c);
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
void game::hand(Player* player,bool firsthand){
    Player* p=player;
    vector <Card*>* Hand;
    if(firsthand==true){
        Hand=p->hand;
        cout << p->name <<"n kortit ovat : \n" << endl;
    }
    else{
        Hand=p->splitHand;
        cout << p->name <<"n kortit split kadessa ovat : \n" << endl;
    }

    for(std::size_t i = 0; i < Hand->size(); ++i){
        Card* c= (*Hand)[i];

        cout << c->Face << " of " << c->Symbol <<"  | ";

    }
}
void game::showHands(Player* player, Player* dealer,bool split){
    system("cls");
    printBlackjack(player);
    hand(player,true);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(player,true)<<"\n\n";
    cout<<"\n==================================\n";
    if(split==true){
    hand(player,false);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(player,false)<<"\n\n";
    cout<<"\n==================================\n";
}
    hand(dealer,true);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(dealer,true)<<"\n\n";

}


int game::totalPoints(Player* player,bool firsthand){
    Player* p=player;
    int points=0;
    int aces=0;
    bool ace=false;
    vector <Card*>* Hand;
    if(firsthand==true){
        Hand=p->hand;
    }
    else{
        Hand=p->splitHand;
    }
    for (std::size_t i=0; i < Hand->size();i++){
        Card* c=(*Hand)[i];

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

int game::yesOrNo (bool Insurance, bool DoubleBet, bool Split){
    int choose;
    bool validNumber=false;
    bool insurance=Insurance;
    bool doubleBet=DoubleBet;
    bool split=Split;

    if (split==true){
        cout<<"Haluatko ottaa split? \n 1. Kylla |  2. En\n";
    }
    else if(doubleBet==true){
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

void game::winningConditions(Player* player,Player* dealer, int bet,bool split,bool firsthand){
    int dp=totalPoints(dealer,true);
    int pp=totalPoints(player,firsthand);
    int b=bet;
    bool win;
    system("cls");
    if(dp>21){
        win=true;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(pp>21){
        win=false;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit"<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp>pp && dp<=21){
        win=false;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp<pp && pp<=21){
        win=true;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp<21){
        win=false;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Tasapeli. Jakaja palauttaa antamasi panoksen "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp==21){
        win=false;
        showHands(player,dealer,split);
        money(win,b,player);
        cout<<"Tasapeli. Jakaja palauttaa antamasi panoksen "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
}


bool game::checkBlackjack(Player* player,bool firsthand ){
    bool blackjack;

    if(totalPoints(player,firsthand)==21){
        blackjack=true;
    }
    else{
        blackjack=false;
    }
    return blackjack;
}
void game::betAmountCheck(Player* player){
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
    p->setBet(bet);
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

bool game::playerTurn(vector <Card*>* playingCards , Player* player,Player* dealer,bool split ,bool firsthand){
    bool playerOverLimit=false;
    bool playerTurn=true;
    do{

        int hit=yesOrNo(false,false,false);

        if(hit==1){
            system("cls");
            pickCard(playingCards,player,1,firsthand);
            showHands(player,dealer,split);

            if(totalPoints(player,firsthand)>21){
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

bool game::dealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer,bool split){

    bool dealerOverLimit=false;
    bool dealerTurn=true;
    bool playerOverLimit=PlayerOverlimit;
    while(dealerTurn==true){

        if(playerOverLimit==true){
            break;
        }
        else if(totalPoints(dealer,true)>21){
            cout<<"Jakaja ylitti pisterajan!\nSina voitit!\n\n";
            dealerOverLimit=true;
            dealerTurn=false;
        }
        else if(totalPoints(dealer,true)>=17){
            dealerTurn=false;

        }
        else{
            system ("cls");
            pickCard(playingCards,dealer,1,true);
            showHands(player,dealer,split);
            sleep(1);
        }
    }
    return dealerOverLimit;
}

void game::busted(bool dealerOverLimit,bool playerOverLimit, Player* player, Player* dealer,bool split ,bool firsthand){
    int bet=player->getBet();
    if (dealerOverLimit==true){
        bool win=true;
        money(win,bet,player);
        cout<<"Voitit"<< bet+bet<<" !\nPaina Enter jatkaaksesi.";
        getch();

    }
    else if(playerOverLimit==true){
        bool win=false;
        money(win,bet,player);
        cout<<"Havisit "<< bet << " !\nPaina Enter jatkaaksesi.";
        getch();
    }
    else{
        winningConditions(player,dealer,bet,split,firsthand);
        cout << "\n \nPaina Enter jatkaaksesi.";
        getch();
    }

}

void game::nextTurn(Player* player, Player* dealer,Deck* d){

    delete dealer;
    delete d;
    player->hand->clear();
    if(player->splitHand->size()>0){
        player->splitHand->clear();
    }
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

bool game::insurance(bool Insurance, Player* dealer, Player* player){
    int bet=player->getBet();
    bool insurance=Insurance;
    bool blackjackDealer=false;
    int insuranceBet=bet/2;
    int checkMoney=bet+insuranceBet;

    if(insurance==true){

        cout<<"Jakajalla on nakyva kortti assa haluatko ottaa insurancen ja katsoa onko toinen kortti 10? \nOlet antanut talle kierrokselle "<< bet << " euron panoksen. \nInsurance on puolet antamastasi panoksesta "<< bet/2<<" euroa.";
        int chooce=yesOrNo(insurance,false,false);
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
                showHands(player,dealer,false);
                cout<<"Jakajalla on toinen kortti 10 arvoinen eli jakajalla on blackjack. Insurancelle asettamasi panos oli"<< insuranceBet <<" euroa.\nSaat taman tuplattuna takaisin "<<bet<<" euroa.";
                cout<<"Koska jakajalla on blackjack haviat alkuperaisen panoksesi"<< bet <<" euroa.";
                money(insuranceWin,bet,player);
                money(normalWin,bet,player);

                cout << "\n \nPaina Enter jatkaaksesi.";
                getch();
            }
            else{
                bool insuranceWin=false;
                cout<<"Jakajan toinen kortti ei ollut 10, joten hävisit"<< bet/2 <<" euron insurancen. Peli jatkuu normaalisti";
                money(insuranceWin,insuranceBet,player);
                cout << "\n \nPaina Enter jatkaaksesi.";
                getch();

            }
        }
        else{
            if(player->getMoney() < checkMoney)
            {
                blackjackDealer=false;
                cout<<"Sinulla ei ollut tarpeeksi rahaa laittaa insurancea. Peli jatkuu normaalisti.";
                cout << "\n \nPaina Enter jatkaaksesi.";
                getch();
            }
            else {
                blackjackDealer=false;
            }
        }
    }

    return blackjackDealer;
}

bool game::doubleDown(Player* player,Player* dealer,vector <Card*>* playingCards, bool blackjack , bool split,bool Firsthand){
    bool firsthand=Firsthand;
    bool doubleDown=false;
    int bet=player->getBet();
    int doubleBet=bet;
    if(totalPoints(player,Firsthand)<=11&&totalPoints(player,Firsthand)>=9 && blackjack==false){

        int chooce=yesOrNo(false,true,false);
        if(chooce==1){
            cout<<"Olet laittanut panokseksi "<<bet<<" euroa. Olet laittamassa tuplana alkuperaisen panoksesi.";
            doubleBet+=bet;
            if(player->getMoney()>=doubleBet){
                cout<<"Sinulla on tarpeeksi rahaa tehdä double down. Jakaja jakaa sinulle nyt viimeisen kortin.";
                pickCard(playingCards,player,1,firsthand);
                showHands(player,dealer,split);
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

bool game::splitCheck(Player* player,bool blackjack){
    //split silloinku kortit ovat samat.
    Player* p=player;
    Card* c=(*p->hand)[0];
    Card* c1=(*p->hand)[1];
    bool split=false;
    int bet=player->getBet();

    //Split tapahtuu kun if on totta.
    if(c->Face==c1->Face && blackjack==false){
        int chooce=yesOrNo(false,false,true);
        if(chooce==1){
            cout<<"Olet laittanut panokseksi "<<bet<<" euroa. Laitat saman verran panosta myos toiselle kadelle.";
            int doubleBet=bet*2;
            if(player->getMoney()>=doubleBet){
            split=true;
            }
            else{
                cout<<"Sinulla ei ole tarpeeksi rahaa splittiin, mutta voit jatkaa normaalisti korttien ottamista.";
                split=false;
            }
        }
    }
    return split;
}

void game::splittingHand(Player* player, Player* dealer,vector <Card*>* playingCards,bool Split){
    //split silloinku kortit ovat samat.
    Player* p=player;
    bool split=Split;

    //Split tapahtuu kun if on totta.
    if(split==true){
        Card* c=p->hand->front();
        p->splitHand->push_back(c);
        p->hand->erase(p->hand->begin());
        // kaksi erillistä katta.
        cout << p->name <<"n kortit ovat splitkadessa 1: \n" << endl;
        hand(player,true);
        cout << p->name <<"\nn kortit ovat splitkadessa 2: \n" << endl;
        hand(player,false);
        // nostetaan toiset kortit.
        pickCard(playingCards,player,1,true);

        // nostetaan toiset kortit.
        pickCard(playingCards,player,1,false);

        system("cls");
        showHands(player,dealer,split);
        bool blackjack=checkBlackjack(player,true);
        bool playerOverLimit=false;
        bool splitPlayerOverLimit=false;


        if(blackjack==false){
            bool doubleCheck=doubleDown(player,dealer,playingCards,blackjack,split,true);
            if(doubleCheck==false){

                cout<<"Ensimmaisen kaden vuoro.";
                playerOverLimit=playerTurn(playingCards,player,dealer,split,true);
            }
            else{


            }
        }
        bool splitBlackjack=checkBlackjack(player,false);
        if(splitBlackjack==false){

            bool splitDoubleCheck=doubleDown(player,dealer,playingCards,blackjack,split,false);
            if(splitDoubleCheck==false){
                cout<<"Toisen kaden vuoro.";
                splitPlayerOverLimit=playerTurn(playingCards,player,dealer,split,false);
            }

        }
        if(playerOverLimit==true){
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true);
        }
        else if(splitPlayerOverLimit==true){
            bool dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false);
        }
        else if(splitPlayerOverLimit==true && playerOverLimit==true){
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true);

            dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false);


        }
        else if(splitPlayerOverLimit==false && playerOverLimit==false){
            // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
            moveFaceDownCard(dealer);

            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true);

            dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false);

        }
        if(playerOverLimit==true && splitPlayerOverLimit==false){
            moveFaceDownCard(dealer);

            bool dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false);
        }
        else if(playerOverLimit==false && splitPlayerOverLimit==true){
            moveFaceDownCard(dealer);
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true);
        }
    }
}
void  game::printBlackjack(Player* player){
  cout<<"         _______  __        _____      ______  __    ___     _____   ______    ______  __    __\n";
  cout<<"        /####### /##|      /######    /######|/##|  /## |   /#####| /######   /###### /##|  /##\n";
  cout<<"       | ##__  #| ##|     /##__  ##  /##___ #| ##| /## /   |__  ##|/##__  ## /##__  #| ##| /##/\n";
  cout<<"       | ##  | #| ##|    | ##| | ## | ##|  |#| ##|/## /       | ##| ##| | ##| ##| |_#| ##|/##/ \n";
  cout<<"       | #######| ##|    | ######## | ##|    | ##### /   __   | ##| ########| ##|    | #####/  \n";
  cout<<"       | ##__  #| ##|    | ## __ ## | ##|   _| ##| ##|  /##|  | ##| ##__  ##| ##|    | ##| ##|  \n";
  cout<<"       | ##  | #| ##|____| ## | |## | ##|__|#| ##| ##| | ##|__| ##| ##| | ##| ##|__ #| ##| ##| \n";
  cout<<"       | #######| #######| ## | |## |  ######| ## | ##|| ########/| ##| | ##|  ######| ## | ##| \n";
  cout<<"       |_______/|________|___/  |__/|_______/|__/  |__/|________/ |__/  |__/|______/ |__/  |__/ \n\n";
  cout<<"         Nimimerkki: "<<player->name<<"    Rahaa jaljella: "<<player->getMoney()<<" euroa.      Panos: "<< player->getBet()<<"    \n\n\n";
}

void game::playerDealerbet(Player* player, Player* dealer){
    int pBet=player->getBet();
    dealer->setBet(pBet);
    money(false,pBet,player);
    cout<<"Sinulle jaa rahaa panostuksen jalkeen "<< player->getMoney()<<" euroa.";
    sleep(2);
}





















