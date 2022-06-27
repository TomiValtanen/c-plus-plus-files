#include <iostream>
#include "deck.h"
#include "game.h"

using namespace std;
int newGame();
void gameMenu();
void rules();
void menu();


int main()
//testi
{
bool gameSession=true;
    menu();
    game* startGame;
    startGame=new game();
    delete startGame;
    do{
    int continueGame=newGame();
    if(continueGame==1){
        game* Game;

        Game= new game();
        delete Game;
    }
    else{
        gameSession=false;
    }
}
    while(gameSession==true);
    return 0;
}

int newGame(){
        int choose;
        bool validNumber=false;
        while(!validNumber){
        cout<<"Halutko ottaa uuden eran blackjackkia ?\n\n 1.Kylla | 2. Ei";
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
void gameMenu(){

    cout<<"===============================================================\n";
    cout<<"===============================================================\n";
    cout<<"                         BLACKJACK                            \n";
    cout<<"                                                               \n";
    cout<<"                                                               \n";
    cout<<"                                                               \n";
    cout<<"                         1.Pelaa                               \n";
    cout<<"                                                               \n";
    cout<<"                         2.Saannot                             \n";
    cout<<"                                                               \n";
    cout<<"                                                               \n";
    cout<<"                                                               \n";
    cout<<"                                                               \n";
    cout<<"===============================================================\n";
    cout<<"===============================================================\n";

}
void rules(){
    system ("cls");
    cout<<"=====================================================================================================================\n";
    cout<<"                                                  SAANNOT                                                            \n\n\n";
    cout<<"Pelaaja pelaa pankkia (jakaja) vastaan.Jakaja jakaa pelaajalle kaksi korttia kuvapuoli \nylospain ja itselleen kaksi korttia, joista toisen kuvapuoli ylospain ja toisen kuvapuoli alaspain.\nPelaaja ilmoittaa, haluaako han uuden kortin. Jakaja jakaa, kunnes pelaaja ei halua enaa lisaa \nkortteja tai korttien pistemaara on yli kaksikymmentayksi.\nKun jokainen pelaaja pelipoydassa on tehnyt valintansa, jakaja paljastaa toisen korttinsa ja \nottaa itselleen lisakortteja, mikali hanen katensä arvo on 16 tai pienempi.\nKun jakajalla on vähintaan 17, han ei saa ottaa enaa lisaa kortteja. Pelin voittaa se, jolla korttien yhteislukema on suurempi menematta yli kahtakymmentayhta tai tasan 21.\n Kahdella ensimmaisella kortilla saatu pistemaara kaksikymmentayksi (assa ja kuvakortti/kymppi) on nimeltaan blackjack,\n ja se voittaa minka tahansa useammalla kuin kahdella kortilla saadun 21 pisteen summan. Jos pelaajalla ja jakajalla on yhteislukema sama, pelaaja haviaa, \n paitsi jos kyseessa on tasapeli kahdessakymmenessayhdessa tai blackjackissa (niin sanottu stand off).\n Jos yhteislukema on suurempi kuin kaksikymmentayksi, sen summan saanut haviaa.";
    cout << "\n \nPaina Enter jatkaaksesi takaisin alkuun.";
    cin.ignore();
    cin.ignore();
    gameMenu();

}
void menu(){
    int choice;
    bool startGame=false;

    do{
        system("cls");
        gameMenu();
        cout<<"Valitse yllaolevista vaihtoehdoista mita haluat tehda.";
        cin>>choice;

        if(choice==1){
            startGame=true;
        }
        else if(choice ==2){
            rules();
        }
        else {
            cout<<"Sinun taytyy painaa 1 , 2 tai 3 jatkaaksesi!";
        }
    }
    while(!startGame);
}


