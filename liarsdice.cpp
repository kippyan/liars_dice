//liar's dice
//27 dec 2018

#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

void cls(){
  cout << "\033[2J\033[1;1H"; 
}

int betface(int prevbet){
  bool validbet = false;
  int face;
  while(!validbet){
    cout << "Please type the face of your bet and press ENTER.";
    if(prevbet <= 0)
      prevbet = 1;
    cout << " Face must be " << prevbet << " or higher, up to 6.";
    cin >> face;
    if(face < prevbet || face > 6){
      cout << "Invalid face value. Press ENTER to try again.";
      cin.ignore();
      cin.ignore();
      continue;
    }//if
    validbet = true;
  }//while
  return face;
}//betface

int numbet(int prevnumbet, bool maxface, int totaldice){
  bool validbet = false;
  int betnum = -1;
  while(!validbet){  
    cout << "Please the number of dice to bet to have your chosen face value." << endl;
    cout << "Enter a value ";
    if(maxface || prevnumbet <= 0)
      cout << "greater than the previous bet count, " << prevnumbet;
    else
      cout << "equal to or greater than the previous bet count, " << prevnumbet;
    cout << " and then press ENTER.";
    cin >> betnum;
    if(maxface){
      if(betnum <= prevnumbet){
        cout << "Invalid bet. Press ENTER to try again.";
        cin.ignore();
        continue;
      }
    }else{
      if(betnum < prevnumbet){
        cout << "Invalid bet. Press ENTER to try again.";
        cin.ignore();
        continue;
      }
    }
    validbet = true;
  }
  return betnum;
}

int main(){
  cout << "WARNING: This program is not robust against invalid inputs.";
  cout << endl << "Please only put in reasonable numbers or else things might explode." << endl;
  srand(time(NULL));
  cout << "Input number of players: ";
  int playercount;
  cin >> playercount;
  vector<int> playerdice;
  for(int i = 0; i < playercount; i++)// initialize dice to 5
    playerdice.push_back(5);
    
  vector<vector<int> > rolls;
  rolls.resize(playercount);
  int totaldice;
  cin.ignore(); // idk why this is needed but it is
  int round = 1;
  while(true){// main loop
    cls();
    totaldice = 0;
    for(int i = 0; i < playercount; i++)//updating rolls after each round
      rolls[i].resize(playerdice[i]);
    int playersout = 0;
    for(int i = 0; i < playercount; i++){//rolling dice
      for(int j = 0; j < playerdice[i]; j++){
        int aroll = (rand() % 6) + 1;
        rolls[i][j] = aroll;
        totaldice++;
      }
      if(playerdice[i] <= 0)//counting players out (for win condition)
        playersout++;
    }
    if(playersout >= playercount-1){//checking win condition
      int winner = -1;
      for(int i = 0; i < playercount; i++)
        if(playerdice[i] > 0)
          winner = i;
      if(winner != -1 && winner < playercount)
        cout << "And the winner is player " << winner+1 << " after " << round << " rounds!" << endl;
      else
        cout << "That's weird, nobody won. Who programmed this garbage?" << endl;
      cout << "Press ENTER to close program.";
      cin.ignore();
      return 0;
    }
    for(int i = 0; i < playercount; i++){// displaying dice
      if(!playerdice[i])
        continue;
      cout << "Remaining dice (player:dice)" << endl;
      for(int i = 0; i < playercount; i++)
        cout << i+1 << ":" << playerdice[i] << " ";
      cout << endl;
      cout << "Press ENTER when only player " << i+1 << " is looking.";
      cin.ignore();
      cout << "Player " << i+1 << "'s dice are: ";
      for(int j = 0; j < (int)playerdice[i]; j++)
        cout << rolls[i][j] << " ";
      cout << endl << "Press ENTER when you're done looking at your dice, player " << i+1;
      cin.ignore();
      cls();
    }
    bool firstturn = true;
    bool called = false;//taking turns betting and calling
    int prevbetface = -1;
    int prevbetcount = 0;
    int checkcount = 0;
    int prevbettor = -1;
    int i = 0;
    int bet = 1;
    while(!called){//loop for rounds breaks when a call is made
      cls();
      cout << "Round " << round << " bet " << bet << "." << endl;
      if(!firstturn)
        cout << "Previous bet: There are " << prevbetcount << " " << prevbetface << "s on the table." << endl;
      if(i >= playercount)
        i = 0;
      if(playerdice[i] <= 0){//skip players who are out
        i++;
        continue;
      }
      cout << "Player " << i+1 << "'s turn. Type 0 to call previous bet or 1 to make a bet and press ENTER to confirm. ";
      int input;
      cin >> input;
      cin.ignore();
      if(!input){//handling calling
        if(firstturn){//handling calling when there hasn't been a previous bet
          cout << "Nobody's made a bet to call on, you goober! Press ENTER to try again.";
          cin.ignore();
          continue;
        }
        called = true;
        cout << "A bold move, to be sure! Verifying if previous bet was correct." << endl;//handling valid calls
        for(int j = 0; j < playercount; j++){
          cout << "Player " << j+1 << "'s rolls: ";
          for(int k = 0; k < playerdice[j]; k++){
            int thisroll = rolls[j][k];
            cout << thisroll << " ";
            if(thisroll == prevbetface)
              checkcount++;
          }
          cout << endl;
        }
        cout << "Player " << prevbettor + 1;
        if(checkcount >= prevbetcount){
          cout << " has not lied! Removing a die from the caller, player " << i + 1 << endl;
          playerdice[i]--;
        }else{
          cout << " has lied! removing a die from the liar, player " << prevbettor + 1 << endl;
          playerdice[prevbettor]--; //this is right
        }//else
        cout << "Press ENTER to start the next round.";
        cin.ignore();
      }else{//handling betting
        prevbettor = i;
        firstturn = false;
        int thisbetface = betface(prevbetface);
        bool maxface = false;
        if(thisbetface == prevbetface)
          maxface = true;
        prevbetface = thisbetface;
        prevbetcount = numbet(prevbetcount, maxface, totaldice);
      }
      i++;
      bet++;
    }//while
    round++;
  }//while
}//int main
