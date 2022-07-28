#include <iostream>
#include <fstream>
#include "deck.h"
#include "game.h"


using namespace std;
int newGame();
void gameMenu();
void rules();
bool menu();
void blackjack();
void hiScore();
void file();

int main()
{
    file();
    bool gameSession=menu();

    if(gameSession==true){
        game* startGame=new game();
        delete startGame;
        do{
            int continueGame=newGame();
            gameSession=menu();
            if(continueGame==1 && gameSession==true){

                game* Game=new game();
                delete Game;
            }
            else{
                gameSession=false;
            }
        }
        while(gameSession==true);
    }
    return 0;
}

int newGame(){


    int choose;
    bool validNumber=false;
    cout<<"Halutko pelata uudestaan ? 1.Kylla / 2.Ei : ";
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
void gameMenu(){

    cout<<"_________________________________________________________________________________________\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                       MENU                                             |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                     1.Pelaa                                            |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                     2.Saannot                                          |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                     3.Ennatykset                                       |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                     4.Poistu                                           |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                                                                        |\n";
    cout<<"|                                                                                        |\n";
    cout<<"------------------------------------------------------------------------------------------\n";

}
void rules(){
    system ("cls");
    blackjack();
    cout<<"=====================================================================================================================\n";
    cout<<"                                                  SAANNOT                                                            \n\n\n";
    cout<<"Pelaaja pelaa pankkia (jakaja) vastaan.Jakaja jakaa pelaajalle kaksi korttia kuvapuoli \nylospain ja itselleen kaksi korttia, joista toisen kuvapuoli ylospain ja toisen kuvapuoli alaspain.\nPelaaja ilmoittaa, haluaako han uuden kortin. Jakaja jakaa, kunnes pelaaja ei halua enaa lisaa \nkortteja tai korttien pistemaara on yli kaksikymmentayksi.\nKun jokainen pelaaja pelipoydassa on tehnyt valintansa, jakaja paljastaa toisen korttinsa ja \nottaa itselleen lisakortteja, mikali hanen katensa arvo on 16 tai pienempi.\nKun jakajalla on vahintaan 17, han ei saa ottaa enaa lisaa kortteja. Pelin voittaa se, jolla korttien yhteislukema on suurempi menematta yli kahtakymmentayhta tai tasan 21.\n Kahdella ensimmaisella kortilla saatu pistemaara kaksikymmentayksi (assa ja kuvakortti/kymppi) on nimeltaan blackjack,\njos pelaajalla ja jakajalla on yhteislukema sama, saa laittamansa panoksen takaisin.\nJos yhteislukema on suurempi kuin kaksikymmentayksi, sen summan saanut haviaa.\nPelaajan on mahdollista ottaa pelin aikana insurance, jos jakajan paalimmainen kortti on assa.\nSlipt on mahdollista, jos ensimmaiset kaksi korttia ovat samat kuva korteissa taytyy olla kunkku ja kunkku esimerkiksi.\nDouble down on mahdollista jos ensimmaisen kahden kortin yhteisumma on 9 ,10 tai 11.\n";
    cout << "\n \nPaina Enter jatkaaksesi takaisin alkuun.";
    cin.ignore();
    cin.ignore();
    gameMenu();

}
bool menu(){
    int choice;
    bool gameSession=true;
    bool startGame=false;

    do{
        system("cls");
        blackjack();
        gameMenu();
        cout<<"Valitse yllaolevista vaihtoehdoista mita haluat tehda.\n\n";
        cin>>choice;

        if(choice==1){
            startGame=true;
        }
        else if(choice ==2){
            rules();
        }
        else if(choice==3){
            system("cls");
            hiScore();
            getch();
        }
        else if(choice==4){
            gameSession=false;
            break;
        }
        else {
            cout<<"Sinun taytyy painaa 1 , 2 , 3 tai 4 jatkaaksesi!";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    while(!startGame);
    return gameSession;
}
void blackjack(){
    cout<<"  _______  __        _____      ______  __    ___     _____   ______    ______  __    __\n";
    cout<<" /####### /##|      /######    /######|/##|  /## |   /#####| /######   /###### /##|  /##\n";
    cout<<"| ##__  #| ##|     /##__  ##  /##___ #| ##| /## /   |__  ##|/##__  ## /##__  #| ##| /##/\n";
    cout<<"| ##  | #| ##|    | ##| | ## | ##|  |#| ##|/## /       | ##| ##| | ##| ##| |_#| ##|/##/ \n";
    cout<<"| #######| ##|    | ######## | ##|    | ##### /   __   | ##| ########| ##|    | #####/  \n";
    cout<<"| ##__  #| ##|    | ## __ ## | ##|   _| ##| ##|  /##|  | ##| ##__  ##| ##|    | ##| ##|  \n";
    cout<<"| ##  | #| ##|____| ## | |## | ##|__|#| ##| ##| | ##|__| ##| ##| | ##| ##|__ #| ##| ##| \n";
    cout<<"| #######| #######| ## | |## |  ######| ## | ##|| ########/| ##| | ##|  ######| ## | ##| \n";
    cout<<"|_______/|________|___/  |__/|_______/|__/  |__/|________/ |__/  |__/|______/ |__/  |__/ \n\n";
}

void hiScore(){
    fstream myFile;
    myFile.open("HiScore.txt",ios::in);
    if(myFile.is_open()){
        string line;
        while(getline(myFile,line)){
            cout<<line<<endl;
        }
        myFile.close();
    }
}
void file(){
    int highscore=0;
    int score=0;
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
        }
        writeFile<<highscore;
    }
    writeFile.close();
}
