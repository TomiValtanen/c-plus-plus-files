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
        //Pelaajan ottamien korttien jälkeen annetaan mahdollisuus ottaa insurance , jos jakajalla on nakyva kortti assa.
        bool insu=checkInsurance(dealer);
        bool insuBlackJack=insurance(insu,bet,dealer,player);

        bool split=splitCheck(player,insuBlackJack);
        if(split==true){
            splittingHand(player,dealer,playingCards,bet,split);
        }

        //Double down ensimmäisten kahden kortin summa on 9 , 10 tai 11.
        bool doubleCheck=doubleDown(player,dealer,bet,playingCards,insuBlackJack,split);
        if(doubleCheck==true){
            int doubleBet=bet*2;
            moveFaceDownCard(dealer);
            //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
            bool dealerOverLimit=dealerTurn(false,playingCards,player,dealer);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            busted(dealerOverLimit,false,doubleBet,player,dealer);
        }

        if(doubleCheck==false && insuBlackJack==false && split==false){

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
void game::splitPickCard(vector <Card*>* playingCards, int numberOfCards, vector <Card*>* splithand){
    vector <Card*>* d= playingCards;
    int number=numberOfCards;

    for(int i=0; i<number;i++){
        Card* c=d->front();
        splithand->push_back(c);
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
void game::showSplitHands(Player* player, Player* dealer, vector <Card*>* splithand){

    hand(player);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(player)<<"\n\n";
    cout<<"\n==================================\n";

    splitHand(player, splithand);
    cout<<"\n\nKokonaispisteet :"<<splitTotalPoints(splithand)<<"\n\n";
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
void game::splitHand(Player* player, vector <Card*>* splithand){
    Player* p=player;

    cout << p->name <<"n kortit ovat : \n" << endl;
    for(std::size_t i = 0; i < splithand->size(); ++i){
        Card* c= (*splithand)[i];

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
int game::splitTotalPoints( vector <Card*>* splithand){
    int points=0;
    int aces=0;
    bool ace=false;

    for (std::size_t i=0; i < splithand->size();i++){
        Card* c=(*splithand)[i];

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
void game::splitWinningConditions(Player* player,Player* dealer, int bet,vector <Card*>* splithand , bool firstHand){
    int dp=totalPoints(dealer);
    int pp=totalPoints(player);
    int stp=splitTotalPoints(splithand);
    int b=bet;
    bool win;
    system("cls");
    if(firstHand==true){
    if(dp>21){
        win=true;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(pp>21){
        win=false;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit"<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp>pp && dp<=21){
        win=false;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp<pp && pp<=21){
        win=true;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Sina voitit "<<bet + bet <<" euroa!";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp<21){
        win=false;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    else if(dp==pp && pp==21){
        win=false;
        showSplitHands(player,dealer,splithand);
        money(win,b,player);
        cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
        cout<<"\n\n\n======================================================\n\n\n";
    }
    }
    else {
        if(dp>21){
            win=true;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Sina voitit "<<bet + bet <<" euroa!";
            cout<<"\n\n\n======================================================\n\n\n";
        }
        else if(stp>21){
            win=false;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Jakaja voittija sinä hävisit"<< bet << " euroa";
            cout<<"\n\n\n======================================================\n\n\n";
        }
        else if(dp>stp && dp<=21){
            win=false;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
            cout<<"\n\n\n======================================================\n\n\n";
        }
        else if(dp<stp && pp<=21){
            win=true;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Sina voitit "<<bet + bet <<" euroa!";
            cout<<"\n\n\n======================================================\n\n\n";
        }
        else if(dp==stp && stp<21){
            win=false;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
            cout<<"\n\n\n======================================================\n\n\n";
        }
        else if(dp==stp && stp==21){
            win=false;
            showSplitHands(player,dealer,splithand);
            money(win,b,player);
            cout<<"Jakaja voittija sinä hävisit "<< bet << " euroa";
            cout<<"\n\n\n======================================================\n\n\n";
        }

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
bool game::splitCheckBlackjack(vector <Card*>* splithand ){
    bool blackjack;

    if(splitTotalPoints(splithand)==21){
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

        int hit=yesOrNo(false,false,false);

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
bool game::splitPlayerTurn(vector <Card*>* playingCards , Player* player,Player* dealer,vector <Card*>* splithand,bool FirstHand ){
    bool playerOverLimit=false;
    bool playerTurn=true;
    bool firstHand=FirstHand;

    if(firstHand==true){
        do{

            int hit=yesOrNo(false,false,false);

            if(hit==1){
                system("cls");
                pickCard(playingCards,player,1);
                showSplitHands(player,dealer,splithand);

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
    }
    else{
        do{

            int hit=yesOrNo(false,false,false);

            if(hit==1){
                system("cls");
                splitPickCard(playingCards,1,splithand);
                showSplitHands(player,dealer,splithand);

                if(splitTotalPoints(splithand)>21){
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
    }

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
bool game::splitDealerTurn(bool PlayerOverlimit,vector <Card*>* playingCards , Player* player,Player* dealer,vector <Card*>* splithand ){

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
            showSplitHands(player,dealer,splithand );
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
void game::splitBusted(bool dealerOverLimit,bool playerOverLimit, int bet, Player* player, Player* dealer,vector <Card*>* splithand, bool firsthand){

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

        splitWinningConditions(player,dealer,bet,splithand,firsthand);
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

bool game::doubleDown(Player* player,Player* dealer,int bet,vector <Card*>* playingCards, bool blackjack , bool split){

    bool doubleDown=false;
    if(totalPoints(player)<=11&&totalPoints(player)>=9 && blackjack==false && split==false){

        int chooce=yesOrNo(false,true,false);
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
bool game::splitDoubleDown(Player* player,Player* dealer,int bet,vector <Card*>* playingCards, bool blackjack ,vector <Card*>* splithand, bool firstHand){

    bool doubleDown=false;
    if(firstHand==true){
        if(totalPoints(player)<=11&&totalPoints(player)>=9 && blackjack==false){

            int chooce=yesOrNo(false,true,false);
            if(chooce==1){
                cout<<"Olet laittanut panokseksi "<<bet<<" euroa. Olet laittamassa tuplana alkuperaisen panoksesi.";
                int doubleBet=bet*2;
                if(player->getMoney()>=doubleBet){
                    cout<<"Sinulla on tarpeeksi rahaa tehdä double down. Jakaja jakaa sinulle nyt viimeisen kortin.";
                    pickCard(playingCards,player,1);
                    showSplitHands(player,dealer,splithand);
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
    }
    else{
        if(splitTotalPoints(splithand)<=11 && splitTotalPoints(splithand)>=9 && blackjack==false){

            int chooce=yesOrNo(false,true,false);
            if(chooce==1){
                cout<<"Olet laittanut panokseksi "<<bet<<" euroa. Olet laittamassa tuplana alkuperaisen panoksesi.";
                int doubleBet=bet*2;
                if(player->getMoney()>=doubleBet){
                    cout<<"Sinulla on tarpeeksi rahaa tehdä double down. Jakaja jakaa sinulle nyt viimeisen kortin.";
                    splitPickCard(playingCards,1,splithand);
                    showSplitHands(player,dealer,splithand);
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
    }
    return doubleDown;
}

bool game::splitCheck(Player* player,bool blackjack){
    //split silloinku kortit ovat samat.
    Player* p=player;
    Card* c=(*p->hand)[0];
    Card* c1=(*p->hand)[1];

    bool split=false;

    //Split tapahtuu kun if on totta.
    if(c->Face==c1->Face && blackjack==false){
        int chooce=yesOrNo(false,false,true);
        if(chooce==1){
            split=true;
        }
    }
    return split;
}

void game::splittingHand(Player* player, Player* dealer,vector <Card*>* playingCards, int Bet,bool Split){
    //split silloinku kortit ovat samat.
    Player* p=player;
    Card* c=(*p->hand)[0];
    bool split=Split;

    //Split tapahtuu kun if on totta.
    if(split==true){
        vector <Card*>* splitHand=new vector <Card*>();
        splitHand->push_back(c);
        p->hand->erase(p->hand->begin());

        // kaksi erillistä katta.
        cout << p->name <<"n kortit ovat splitkadessa 1: \n" << endl;
        for(std::size_t i = 0; i < p->hand->size(); ++i){
            Card* c= (*p->hand)[i];
            Card* c1 =(*splitHand)[i];

            cout << c->Face << " of " << c->Symbol <<"  | \n";
            cout << p->name <<"n kortit ovat splitkadessa 2: \n" << endl;
            cout << c1->Face << " of " << c1->Symbol <<"  | \n";
            cout<<"Jaetaan molempiin kasiin toiset kortit.";
        }
        // nostetaan toiset kortit.
        pickCard(playingCards,player,1);

        // nostetaan toiset kortit.
        splitPickCard(playingCards,1,splitHand);

        system("cls");
        showSplitHands(player,dealer,splitHand);
        bool blackjack=checkBlackjack(player);
        bool playerOverLimit=false;
        bool splitPlayerOverLimit=false;
        int bet =Bet;
        int splitBet=Bet;

        if(blackjack==false){
            bool doubleCheck=splitDoubleDown(player,dealer,splitBet,playingCards,blackjack,splitHand,true);
            if(doubleCheck==false){
                playerOverLimit=splitPlayerTurn(playingCards,player,dealer,splitHand,true);
            }
            else{
                bet=Bet*2;
            }
        }
        bool splitBlackjack=splitCheckBlackjack(splitHand);
        if(splitBlackjack==false){
            bool splitDoubleCheck=splitDoubleDown(player,dealer,splitBet,playingCards,splitBlackjack,splitHand,false);
            if(splitDoubleCheck==false){
                splitPlayerOverLimit=splitPlayerTurn(playingCards,player,dealer,splitHand,false);
            }
            else{
                splitBet=Bet*2;
            }
        }
        if(playerOverLimit==true){
            bool dealerOverLimit=splitDealerTurn(playerOverLimit,playingCards,player,dealer,splitHand);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            splitBusted(dealerOverLimit,playerOverLimit,bet,player,dealer,splitHand,true);
        }
        else if(splitPlayerOverLimit==true){
            bool dealerOverLimit=splitDealerTurn(playerOverLimit,playingCards,player,dealer,splitHand);
            splitBusted(dealerOverLimit,splitPlayerOverLimit,splitBet,player,dealer,splitHand,false);
        }
        else{
            // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
            moveFaceDownCard(dealer);

            //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
            bool dealerOverLimit=splitDealerTurn(playerOverLimit,playingCards,player,dealer,splitHand);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            splitBusted(dealerOverLimit,playerOverLimit,bet,player,dealer,splitHand,true);
            splitBusted(dealerOverLimit,splitPlayerOverLimit,splitBet,player,dealer,splitHand,false);
        }
        if(playerOverLimit==true && splitPlayerOverLimit==false){
            moveFaceDownCard(dealer);
            bool dealerOverLimit=splitDealerTurn(playerOverLimit,playingCards,player,dealer,splitHand);
            splitBusted(dealerOverLimit,splitPlayerOverLimit,splitBet,player,dealer,splitHand,false);
        }
        else if(playerOverLimit==false && splitPlayerOverLimit==true){
            moveFaceDownCard(dealer);
            bool dealerOverLimit=splitDealerTurn(playerOverLimit,playingCards,player,dealer,splitHand);
            splitBusted(dealerOverLimit,playerOverLimit,bet,player,dealer,splitHand,true);
        }
    }
}























