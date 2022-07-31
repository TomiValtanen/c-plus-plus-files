#include "game.h"


game::game()
{
    string dealerName="Jakaja";
    Deck* deck= new Deck();
    vector <Card> playerHand=vector <Card>();
    vector <Card> dealerHand=vector <Card>();
    vector <Card> dealerCard=vector <Card>();
    vector <Card> splithand=vector <Card>();

    Player* player=new Player(name(),playerHand,splithand,100);
    Player* dealer=new Player(dealerName,dealerHand,dealerCard);

    startGame(player, dealer ,deck);
}
// Ottaa nimimerkin
string game::name(){


    string name;
    cout<<"Pelinimesi : ";
    cin>>name;
    cout<<"Onnea matkaan "<<name<<" !\n\n";
    sleep(2);
    return name;

}
//Aloittaa pelin ja käsittelee koko kierroksen kulun ja aloittaa sen alusta, jos rahatilanne sen sallii.
void game::startGame(Player* player, Player* dealer, Deck* deck){
    setDealerAndPlayerBet(player,dealer);
    system("cls");
    printBlackjack(player);
    Deck* d=deck;
    vector <Card> playing= d->getCards();
    vector <Card>*playingCards=&playing;

    cout<<"Sinulla on rahaa "<< player->getMoney() <<" euroa\n";
    cout<<"Pienin osallistumis maksu on 10 euroa ja suurin 100 euroa.\nPaljonko tahdot panostaa?\n";
    //Panoksen antaminen ja sen laittaminen itselle ja jakajalle.
    betAmountCheck(player);
    playerDealerbet(player,dealer);

    //Jaetaan aloitus kortit annetaan pelaajalle ensimmäinen, toinen jakajalle ja kolmas pelaajalle
    pickCard(playingCards,player,1,true);
    pickCard(playingCards,dealer,1,true);
    pickCard(playingCards,player,1,true);

    // neljäs kortti annetaan jakajalle kortin mitä ei näytetä, ennenkuin jakaja aloittaa korttien nostamisen.
    faceDownDealerCard(playingCards,dealer);

    showHands(player,dealer,false);
    //Tarkistaa onko pelaajalla tai jakalla Blackjack.
    bool blackjackCheck=checkBlackjack(player,true);

    if (blackjackCheck==true){
        blackjack(player,dealer,blackjackCheck,false,true);

    }

    else{
        //Pelaajan ottamien korttien jälkeen annetaan mahdollisuus ottaa insurance , jos jakajalla on nakyva kortti ässä.
        bool insu=checkInsurance(dealer);
        bool insuBlackJack=insurance(insu,dealer,player);
        //Split käy läpi myös double down mahdollisuuden omassa functiossaan ja siellä tapahtuu myös normaali korttien nosto ja voittojen jako.
        bool split=splitCheck(player,dealer,insuBlackJack);
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
                busted(dealerOverLimit,false,player,dealer,split,true,blackjackCheck);
            }

            else{

                //Nostetaan kortteja sen verran ,että "Bust" tai pelaaja päättää itse lopettaa korttien nostamisen.
                bool playerOverLimit=playerTurn(playingCards,player,dealer,split,true);



                //Jakaja aloittaa korttien ottamisen , jos blackjackkia ei ollut tai pelaajan ei bustannut.
                if(playerOverLimit==false){

                    // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
                    moveFaceDownCard(dealer);
                }
                //Jakaja nostaa kortteja siihen asti ,että kädessä on 17+ korttien arvo tai "Bust".
                bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
                //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
                busted(dealerOverLimit,playerOverLimit,player,dealer,split,true,blackjackCheck);

            }
        }
    }
    /*Tarkistetaan onko pelaajalla kyseistä summaa rahaa osallistua seuraavalle kierrokselle.
     * Samalla katsotaan pistetilanne ja tehdään sen mukaiset tallennukset
     * Jos ei ollut varoja seuraavalle kierrokselle tallennetaan pisteet.
     */
    if(player->getMoney()>=10){
        checkScore(player);
        nextTurn(player, dealer, d,playingCards);
    }
    else{
        cout<<"\nVoi harmi sinulla ei ole enaa rahaa seuraavalle kierrokselle vahimmaismaara panostukselle on 10 euroa. Sinulla on "<< player->getMoney()<<" euroa.\n\n";
        saveScore(player);
        sleep(2);
    }


}
// Nostaa kortin pakasta functioon laitetaan pakka, kukanostaa, montako korttia nostaa ja kumpaan käteen nostaa.
void game::pickCard(vector <Card>* playingCards,Player* player , int numberOfCards ,bool Firsthand){
    bool firsthand=Firsthand;
    int number=numberOfCards;
    Player* p=player;
    vector <Card> *d= playingCards;

    for(int i=0; i<number;i++){
        Card c=d->front();
        p->setCard(c,firsthand);
        d->erase(d->begin());
    }
}
//Jakajan kortti mitä ei aluksi näytetä.
void game::faceDownDealerCard(vector <Card>* playingCards,Player* dealer ){
    Player* Dealer=dealer;
    vector <Card>* d= playingCards;

    Card c= d->front();
    Dealer->dealerCard.push_back(c);
    d->erase(d->begin());
}
// Siirretään väärinpäin oleva kortti jakajan käteen.
void game::moveFaceDownCard(Player* dealer){
    Player* Dealer=dealer;
    system ("cls");
    Card c =Dealer->dealerCard.front();
    Dealer->hand.push_back(c);
    Dealer->dealerCard.erase(Dealer->dealerCard.begin());
}
//Kertoo mikä kortti kyseessä.
void game::hand(Player* player,bool firsthand){
    Player* p=player;
    vector <Card> Hand;
    if(firsthand==true){
        Hand=p->hand;
        cout << p->name <<"n kortit ovat : \n" << endl;

    }
    else{
        Hand=p->splitHand;
        cout << p->name <<"n kortit split kadessa ovat : \n" << endl;

    }

    for(std::size_t i = 0; i < Hand.size(); ++i){
        Card c= Hand[i];

        cout << c.Face << " of " << c.Symbol <<"  | ";

    }
}//Näyttää mitä pelaajalla ja jakajalla on kädessä.
void game::showHands(Player* player, Player* dealer,bool split){
    system("cls");
    printBlackjack(player);
    hand(player,true);
    cout<<"\n\nEnsimmainen kasi\n";
    cout<<"Kokonaispisteet :"<<totalPoints(player,true)<<"\n";
    cout<<"\n===========================================================================================\n";
    if(split==true){
        hand(player,false);
        cout<<"\n\nToinen kasi\n";
        cout<<"Kokonaispisteet :"<<totalPoints(player,false)<<"\n";
        cout<<"\n===========================================================================================\n";
    }
    hand(dealer,true);
    cout<<"\n\nKokonaispisteet :"<<totalPoints(dealer,true)<<"\n\n";

}

/* Pisteiden lasku.
 * Samalla katsotaan myös onko ässiä kädessä ja toimitaan pistetilanteiden mukaan eli onko soft hand vai ei.
 */
int game::totalPoints(Player* player,bool firsthand){
    Player* p=player;
    int points=0;
    int aces=0;
    bool ace=false;
    vector <Card> Hand;
    if(firsthand==true){
        Hand=p->hand;
    }
    else{
        Hand=p->splitHand;
    }
    for (std::size_t i=0; i < Hand.size();i++){
        Card c=Hand[i];

        if(c.Face=="Ace"){
            points=points+11;
            aces+=1;
            ace=true;
        }
        else{
            points+=c.Point;
        }
    }
    if (ace==true && points>21){
        for (int i=0; i<aces;i++){
            points=points-10;
        }
    }

    return points;

}
//Kyllä tai ei valinta kysely.
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
// Tarkistaa kuka voitti.
void game::winningConditions(Player* player,Player* dealer,bool split,bool firsthand){
    int dp=totalPoints(dealer,true);
    int pp=totalPoints(player,firsthand);
    int betPlayer=player->getBet();
    int betDealer=dealer->getBet();
    int winningPool=betPlayer+betDealer;
    if(split==true && firsthand==false){
        betPlayer=player->getSplitBet();
        winningPool=betPlayer+betPlayer;
    }
    else if(split==true && firsthand==true){
        winningPool=betPlayer+betPlayer;
    }

    bool win;
    system("cls");
    if(dp>21){
        win=true;
        showHands(player,dealer,split);
        money(win,winningPool,player);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Sina voitit "<< betPlayer <<" euroa! Samalla saat oman panoksen takaisin";

    }
    else if(pp>21){
        showHands(player,dealer,split);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Jakaja voittija ja keraa panoksen havisit "<< betPlayer << " euroa";

    }
    else if(dp>pp && dp<=21){
        showHands(player,dealer,split);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Jakaja voittija ja keraa panoksen havisit "<< betPlayer << " euroa";

    }
    else if(dp<pp && pp<=21){
        win=true;
        showHands(player,dealer,split);
        money(win,winningPool,player);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Sina voitit "<< betPlayer <<" euroa! Samalla saat oman panoksen takaisin";

    }
    else if(dp==pp && pp<21){
        win=true;
        showHands(player,dealer,split);
        money(win,betPlayer,player);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Tasapeli. Jakaja palauttaa antamasi panoksen "<< betPlayer << " euroa";

    }
    else if(dp==pp && pp==21){
        win=true;
        showHands(player,dealer,split);
        money(win,betPlayer,player);
        cout<<"\n===========================================================================================\n\n\n";
        cout<<"Tasapeli. Jakaja palauttaa antamasi panoksen "<< betPlayer << " euroa";

    }
}

// tarkistaa onko blackjack
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
//Antaa panoksen sille kierrokselle ja tarkistaa onko pelaajalla varaa tehdä niin isoa panosta.
void game::betAmountCheck(Player* player){
    Player* p=player;
    int  bet;
    bool validNumber=false;
    cout<<"Kirjoita panoksesi :";
    do{
        cin>>bet;
        if(bet<10 || bet>100){
            cout<<"\nPienin mahdollinen panostus on 10 euroa ja suurin mahdollinen 100 euroa!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (bet > p->getMoney()){
            cout<<"\nSinulla ei ole noin paljon rahaa! Sinulla on rahaa "<<p->getMoney()<<"\n";
        }
        else {
            validNumber=true;
        }
    }
    while(validNumber==false);
    p->setBet(bet);
}
//Vähentää pelaajalta rahaa tai antaa sitä.
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
//Pelaajan vuoro nostaa kortteja pelaaja itse päättää milloin lopettaa ellei korttien summa ole yli 21.
bool game::playerTurn(vector <Card>* playingCards , Player* player,Player* dealer,bool split ,bool firsthand){
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
                sleep(2);
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
//Jakajan vuoro nostaa kortteja jakaja lopettaa jos pisteet ovat 17 tai enemmän.
bool game::dealerTurn(bool PlayerOverlimit,vector <Card>* playingCards , Player* player,Player* dealer,bool split){

    bool dealerOverLimit=false;
    bool dealerTurn=true;
    bool playerOverLimit=PlayerOverlimit;
    while(dealerTurn==true){

        if(playerOverLimit==true){
            break;
        }
        else if(totalPoints(dealer,true)>21){
            cout<<"\nJakaja ylitti pisterajan!\nSina voitit!\n\n";
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
//Tarkistaa onko menty menty yli 21 pisteet vai mennäänkö tavallisen voitonjaon kautta
void game::busted(bool dealerOverLimit,bool playerOverLimit, Player* player, Player* dealer,bool split ,bool firsthand,bool blackjack){
    int betPlayer=player->getBet();
    int betDealer=dealer->getBet();
    int winningPool=betPlayer+betDealer;
    if(split==true && firsthand==false){
        betPlayer=player->getSplitBet();
        winningPool=betPlayer+betPlayer;
    }
    else if(split==true && firsthand==true){
        winningPool=betPlayer+betPlayer;
    }
    if(blackjack==false){
        if (dealerOverLimit==true){
            bool win=true;
            money(win,winningPool,player);
            cout<<"\n===========================================================================================\n\n\n";
            cout<<"Voitit "<< betPlayer<<" euroa ! Jakaja palauttaa laittamasi panoksen.\nPaina Enter jatkaaksesi.";
            getch();

        }
        else if(playerOverLimit==true){
            //showHands(player,dealer,split);
            cout<<"\n===========================================================================================\n\n\n";
            cout<<"Havisit "<< betPlayer << " euroa !\nPaina Enter jatkaaksesi.";
            getch();
        }
        else{
            winningConditions(player,dealer,split,firsthand);
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
        }
    }
    else{
        winningPool=(betPlayer*1.5)+betPlayer;
        cout<<"Sait blackjackin! Voitit "<<betPlayer*1.5<<" euroa.Jakaja antaa panoksesi myos takaisin.";

        money(true,winningPool,player);
        getch();
    }

}
/*Seuraavalle kierrokselle meneminen
 * Syötetään kaikista käsistä kortit poistopakkaan
 * Kierroksia , kun on tullut 5 yhteensä yhdistetään jäljellä oleva pakka ja poistopakka uudeksi pelipakaksi ja sekoitetaan se.
 */
void game::nextTurn(Player* player, Player* dealer,Deck* d,vector <Card>*playingCards){

    for (std::size_t i=0; i < player->hand.size();i++){
        Card c=player->hand[i];
        dealer->discardDeck.push_back(c);
    }
    if(player->splitHand.size()>0){
        for (std::size_t i=0; i < player->splitHand.size();i++){
            Card c=player->splitHand[i];
            dealer->discardDeck.push_back(c);
        }
        player->splitHand.clear();
    }
    if(dealer->dealerCard.size()>0){
        moveFaceDownCard(dealer);
        for (std::size_t i=0; i < dealer->hand.size();i++){
            Card c=dealer->hand[i];
            dealer->discardDeck.push_back(c);

        }
    }
    else{
        for (std::size_t i=0; i < dealer->hand.size();i++){
            Card c=dealer->hand[i];
            dealer->discardDeck.push_back(c);
        }
    }
    player->hand.clear();
    dealer->hand.clear();
    dealer->addRound(1);
    if(dealer->getRound()==5){
        dealer->setRound(0);
        Deck* deck=new Deck(playingCards,dealer->discardDeck);
        dealer->discardDeck.clear();
        delete d;
        system("cls");
        printBlackjack(player);
        cout<<"\n\nJakaja ottaa poistopakan ja kayttopakan. Sekoittaa ne keskenaan seuraavaa kierrosta varten.\n";
        sleep(3);
        cout<<"Kierros jatkuu aivan hetken kuluttua!";
        sleep(2);
        startGame(player,dealer,deck);
    }
    else{
        Deck* deck=new Deck(playingCards);
        delete d;
        startGame(player,dealer,deck);
    }

}
//tarkistaa onko jakajan näkyvä kortti ässä
bool game::checkInsurance(Player* dealer){
    bool insurance=false;
    Player* d=dealer;
    Card c= d->hand[0];

    if(c.Face=="Ace"){
        insurance=true;
    }
    return insurance;
}
//Jos ässä oli ensimmäinen jakajan kortti tarjotaan pelaajalle insurancea
bool game::insurance(bool Insurance, Player* dealer, Player* player){
    int bet=player->getBet();
    bool insurance=Insurance;
    bool blackjackDealer=false;
    int insuranceBet=bet/2;


    if(insurance==true){

        cout<<"Jakajalla on nakyva kortti assa haluatko ottaa insurancen ja katsoa onko toinen kortti 10? \nOlet antanut talle kierrokselle "<< bet << " euron panoksen. \nInsurance on puolet antamastasi panoksesta "<< bet/2<<" euroa.";
        int chooce=yesOrNo(insurance,false,false);
        if(chooce==1 && player->getMoney()>=insuranceBet){
            Player* d= dealer;
            Card c= d->dealerCard[0];
            money(false,insuranceBet,player);
            if(c.Point==10){
                blackjackDealer=true;
            }
            if(blackjackDealer==true){
                bool insuranceWin=true;
                moveFaceDownCard(dealer);
                showHands(player,dealer,false);
                cout<<"\nJakajalla on toinen kortti 10 arvoinen eli jakajalla on blackjack. Insurancelle asettamasi panos oli "<< insuranceBet <<" euroa.\nSaat taman tuplattuna takaisin "<<bet<<" euroa.";
                cout<<"Koska jakajalla on blackjack haviat alkuperaisen panoksesi "<< bet <<" euroa.\n";
                money(insuranceWin,bet,player);


                cout << "\n \nPaina Enter jatkaaksesi.";
                getch();
            }
            else{
                cout<<"\nJakajan toinen kortti ei ollut 10, joten havisit"<< bet/2 <<" euron insurancen. Peli jatkuu normaalisti\n";
                cout << "\n \nPaina Enter jatkaaksesi.";
                getch();

            }
        }
        else if(chooce==2){
            blackjackDealer=false;
            cout<<"\nEt halua ottaa insurancea. Peli jatkuu normaalisti.";
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
        }
        else{
            if(player->getMoney() < insuranceBet)
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
//Tarjotaan mahdollista ottaa double down , jos kahden ensimmäisen kortin yhteispisteet ovat 9 , 10 tai 11
bool game::doubleDown(Player* player,Player* dealer,vector <Card>* playingCards, bool blackjack , bool split,bool Firsthand){
    bool firsthand=Firsthand;
    bool doubleDown=false;
    int originalBet=player->getOriginalBet();

    if(totalPoints(player,Firsthand)<=11&&totalPoints(player,Firsthand)>=9 && blackjack==false){

        int chooce=yesOrNo(false,true,false);
        if(chooce==1){
            cout<<"\nOlet laittanut panokseksi "<<originalBet<<" euroa. Olet laittamassa tuplana alkuperaisen panoksesi.\n";


            if(player->getMoney()>=originalBet){
                if(firsthand==true){
                    player->addBet(originalBet);
                }
                else{
                    player->addSplitBet(originalBet);
                }
                money(false,originalBet,player);
                dealer->addBet(originalBet);
                cout<<"\nSinulla on tarpeeksi rahaa tehda double down. Jakaja jakaa sinulle nyt viimeisen kortin.\n";
                pickCard(playingCards,player,1,firsthand);
                showHands(player,dealer,split);
                doubleDown=true;
            }
            else{
                cout<<"\nSinulla ei ole tarpeeksi rahaa double down, mutta voit jatkaa normaalisti korttien ottamista.\n";
                doubleDown=false;
            }
        }
        else{
            doubleDown=false;
        }
    }
    return doubleDown;
}
//Tarkistaa onko pelaajan kaksi ensimmäistä kortti samanlaiset tarjoaa sitten split vaihtoehtoa.
bool game::splitCheck(Player* player,Player* dealer,bool blackjack){
    //split silloinku kortit ovat samat.
    Player* p=player;
    Card c=p->hand[0];
    Card c1=p->hand[1];
    bool split=false;
    int bet=player->getOriginalBet();

    //Split tapahtuu kun if on totta.
    if(c.Face==c1.Face && blackjack==false){
        int chooce=yesOrNo(false,false,true);
        if(chooce==1){
            cout<<"\nOlet laittanut panokseksi "<<bet<<" euroa. Laitat saman verran panosta myos toiselle kadelle.\n";
            if(player->getMoney()>=bet){
                player->setSplitBet(bet);
                dealer->addBet(bet);
                money(false,bet,player);
                split=true;
            }
            else{
                cout<<"\nSinulla ei ole tarpeeksi rahaa splittiin, mutta voit jatkaa normaalisti korttien ottamista.\n";
                split=false;
            }
        }
    }
    return split;
}
//Split functio missä on myös double down ja normaalit korttien nostot.
void game::splittingHand(Player* player, Player* dealer,vector <Card>* playingCards,bool Split){
    //split silloinku kortit ovat samat.
    Player* p=player;
    bool split=Split;

    //Split tapahtuu kun if on totta.
    if(split==true){
        Card c=p->hand.front();
        p->splitHand.push_back(c);
        p->hand.erase(p->hand.begin());
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
        bool blackjackCheck=checkBlackjack(player,true);
        bool playerOverLimit=false;
        bool splitPlayerOverLimit=false;


        if(blackjackCheck==false){


            bool doubleCheck=doubleDown(player,dealer,playingCards,blackjackCheck,split,true);
            if(doubleCheck==false){

                cout<<"Ensimmaisen kaden vuoro.";
                playerOverLimit=playerTurn(playingCards,player,dealer,split,true);
            }

        }

        bool splitBlackjack=checkBlackjack(player,false);
        if(splitBlackjack==false){

            bool splitDoubleCheck=doubleDown(player,dealer,playingCards,splitBlackjack,split,false);
            if(splitDoubleCheck==false){
                cout<<"Toisen kaden vuoro.";
                splitPlayerOverLimit=playerTurn(playingCards,player,dealer,split,false);
            }


        }
        if(splitPlayerOverLimit==false || playerOverLimit==false){
            // näyttää jakajan toisen kortin ja siirtää sen jakajan käteen.
            moveFaceDownCard(dealer);
            showHands(player,dealer,split);
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            cout<<"\nEnsimmaisen kaden tarkastus : \n";
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
            system("cls");
            showHands(player,dealer,split);
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true,blackjackCheck);


            dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            cout<<"\nToisen kaden tarkastus : \n";
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
            system("cls");
            showHands(player,dealer,split);
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false,splitBlackjack);

        }
        else if(splitPlayerOverLimit==true && playerOverLimit==true){
            bool dealerOverLimit=dealerTurn(playerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            cout<<"\nEnsimmaisen kaden tarkastus : \n";
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
            system("cls");
            showHands(player,dealer,split);
            busted(dealerOverLimit,playerOverLimit,player,dealer,split,true,blackjackCheck);


            dealerOverLimit=dealerTurn(splitPlayerOverLimit,playingCards,player,dealer,split);
            //Tarkistaa onko kumpikaan jakaja tai pelaaja "Bust" , jos ei niin tavallisten sääntöjen mukaan kumpi voittaa kierroksen.
            cout<<"\nToisen kaden tarkastus : \n";
            cout << "\n \nPaina Enter jatkaaksesi.";
            getch();
            system("cls");
            showHands(player,dealer,split);
            busted(dealerOverLimit,splitPlayerOverLimit,player,dealer,split,false,splitBlackjack);
        }
    }
}


// koriste teksti , jonka alla näkyy pelaajan nimi, rahatilanne, ensimmäisen käden panos ja myös toisen , jos pääsee splittiin
void  game::printBlackjack(Player* player){
    cout<<"  _______  __        _____      ______  __    ___     _____   ______    ______  __    __\n";
    cout<<" /####### /##|      /######    /######|/##|  /## |   /#####| /######   /###### /##|  /##\n";
    cout<<"| ##__  #| ##|     /##__  ##  /##___ #| ##| /## /   |__  ##|/##__  ## /##__  #| ##| /##/\n";
    cout<<"| ##  | #| ##|    | ##| | ## | ##|  |#| ##|/## /       | ##| ##| | ##| ##| |_#| ##|/##/ \n";
    cout<<"| #######| ##|    | ######## | ##|    | ##### /   __   | ##| ########| ##|    | #####/  \n";
    cout<<"| ##__  #| ##|    | ## __ ## | ##|   _| ##| ##|  /##|  | ##| ##__  ##| ##|   _| ##| ##|  \n";
    cout<<"| ##  | #| ##|____| ## | |## | ##|__|#| ##| ##| | ##|__| ##| ##| | ##| ##|__|#| ##| ##| \n";
    cout<<"| #######| #######| ## | |## |  ######| ## | ##|| ########/| ##| | ##|  ######| ## | ##| \n";
    cout<<"|_______/|________|___/  |__/|_______/|__/  |__/|________/ |__/  |__/|_______/|__/  |__/ \n\n";
    cout<<"Nimimerkki: "<<player->name<<"    Rahaa jaljella: "<<player->getMoney()<<" euroa.     1.kaden Panos: "<< player->getBet()<<"   2.kaden panos: "<<player->getSplitBet()<<" \n";
    cout<<"\n===========================================================================================\n\n\n";
}
//Ottaa pelaajan valitseman panostuksen ja laittaa sen myös jakajalle ja itselleen samalla vähentää panoksen verran rahaa.
void game::playerDealerbet(Player* player, Player* dealer){
    int pBet=player->getBet();
    dealer->setBet(pBet);
    player->setOriginalBet(pBet);
    money(false,pBet,player);
    cout<<"\nSinulle jaa rahaa panostuksen jalkeen "<< player->getMoney()<<" euroa.\n";
    sleep(2);
}
//Laittaa panokset 0
void game::setDealerAndPlayerBet(Player* player,Player* dealer){
    player->setBet(0);
    dealer->setBet(0);
    player->setOriginalBet(0);
    player->setSplitBet(0);
}
//Tarkistaa onko pelaajan rahatilanne isompi mitä aikaisemmin ja asettaa uuden highscoren
void game::checkScore(Player*player){
    int moneyAmount=player->getMoney();
    int highscore=player->getHighScore();

    if(moneyAmount>highscore){
        player->setHighScore(moneyAmount);
    }
}
//tallentaa jos uusi ennätys tulee.
void game::saveScore(Player* player){
    int highscore = 0;
    int score=player->getHighScore();
    bool newrecord=false;
    fstream readFile;
    readFile.open("Score.txt",ios::in);
    if(readFile.is_open()){
        while(!readFile.eof()){
            readFile>>highscore;
        }
    }
    readFile.close();
    fstream writeFile;
    writeFile.open("Score.txt",ios::out);
    if(writeFile.is_open()){
        if(score>highscore){
            highscore=score;
            newrecord=true;
        }
        writeFile<<highscore;
    }
    writeFile.close();
    if(newrecord==true){
        fstream myFile;
        myFile.open("HiScore.txt",ios::out);
        if(myFile.is_open()){
            myFile<<"Ennatykset\n";
            myFile<<"Nimimerkki:     Pisteet:\n";
            myFile.close();
        }
        myFile.open("HiScore.txt",ios::app);
        if(myFile.is_open()){
            myFile<<player->name<<"            "<<score<<"\n";
            myFile.close();

        }
    }
}
//Blackjack tarkistus
void game::blackjack(Player* player, Player* dealer,bool blackjack,bool split,bool firsthand){
    int bet=player->getBet();
    int winningPool=bet+bet;
    if(firsthand==false && split==true){
        bet=player->getSplitBet();
        winningPool=bet+bet;
    }
    if(split==true && firsthand==true){
        winningPool=bet+bet;
    }


    moveFaceDownCard(dealer);
    bool blackjackDealer=checkBlackjack(dealer,true);
    showHands(player, dealer,split);

    if(blackjack==true && blackjackDealer==true){
        cout<<"Tasapeli molemmilla on BlackJack!\nVoitit "<<bet;
        money(true,winningPool,player);
        cout << "\n \nPaina Enter jatkaaksesi.";
        getch();
    }
    else {
        if(firsthand==false && split==true){
            bet=player->getSplitBet();
            winningPool=(bet*1.5)+bet;
        }
        else if(split==true && firsthand==true){
            winningPool=(bet*1.5)+bet;
        }
        else{
            winningPool=(bet*1.5)+bet;
        }
        cout<<"Sina sait blackjackin ja voitit "<<bet*1.5 <<" \n\n";
        money(true,winningPool,player);
        cout << "\n \nPaina Enter jatkaaksesi.";
        getch();

    }
}



















