#include <cstdlib>
#include <ctime>
#include <iostream>
#include "dice.h"

using namespace std;

// Destructor of Dice.
Dice::~Dice(){ }

int FairDice::getValue(){
    srand((unsigned)time(0));
    int a = rand() % 6 + 1;// First Dice
    int b = rand() % 6 + 1;// Second Dice
    
    return a + b;// The sum of two dices
}

int FairDice::getValue(const int& seed){
    srand(seed);
    int a = rand() % 6 + 1;// First Dice
    int b = rand() % 6 + 1;// Second Dice
    
    return a + b;
}

// Destructor of FairDice.
FairDice::~FairDice(){}

int LoadedDice::getValue(){
    int s;

    // Waiting the palyer to input the value.
    while(true){
        cerr << "Input a roll between 2 and 12:" << endl;
        
        cerr << ">";
        cin >> s;
        if (cin.eof()) return 0;
        if (cin.fail()){
        	cin.clear();
        	cin.ignore();
        	cerr << "Invalid Input" << endl;
        } else if((s < 13) && (s > 1)) {
        	return s;// Valid value.
        } else {
            cerr << "Invalid roll." << endl;
        }
    }
 }

int LoadedDice::getValue(const int& seed){
    int s;

    // Waiting the palyer to input the value.
    while(true){
        cerr << "Input a roll between 2 and 12:" << endl;
        
        cerr << ">";
        cin >> s;
        if (cin.eof()) return 0;
        if (cin.fail()){
        	cin.clear();
        	cin.ignore();
        	cerr << "Invalid Input" << endl;
        } else if((s < 13) && (s > 1)) {
        	return s;// Valid value.
        } else{
            cerr << "Invalid roll." << endl;
        }
    }
 }

// Destructor of LoadedDice.
LoadedDice::~LoadedDice(){ }
