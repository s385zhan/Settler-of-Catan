#include <iostream>
#include <string>
#include <vector>
#include "textdisplay.h"
#include "info.h"
#include "subject.h"
#include "resource.h"
#include "subscription.h"

using namespace std;

const int height = 41;
const int width = 54;

// Constructor.
TextDisplay::TextDisplay(){
    for (int i = 0;i < height;++i){
        // initialize the board to be able to contain all possible char
        vector <char> v;
        for (int j = 0;j < width;++j){
            v.emplace_back(' ');// adding spaces
        }
        theDisplay.emplace_back(v);
    }
}

// Destructor.
TextDisplay::~TextDisplay(){  }

// Print out the board.
void TextDisplay::print(){
    for (auto v : theDisplay){
        for (auto c : v){
            cout << c;
        }
        cout << endl;
    }
}

// Print out the board.
void TextDisplay::notify(Subject& s, SubscriptionType t){
    Info i = s.getInfo();
    if (t == SubscriptionType::SetPH){
        // set horinzontal path, "--" before and after the num awaiting 
        theDisplay[i.PosY][i.PosX + 1] = '-';
        theDisplay[i.PosY][i.PosX + 2] = '-';
        theDisplay[i.PosY][i.PosX - 2] = '-';
        theDisplay[i.PosY][i.PosX - 3] = '-';
    } else if (t == SubscriptionType::SetPV){
        // set vertical path, "|"  above and after the num awaiting
        theDisplay[i.PosY - 1][i.PosX] = '|';
        theDisplay[i.PosY + 1][i.PosX] = '|';
    } else if (t == SubscriptionType::SetAd){
        // set address, "|"" before and after the num awaiting
        theDisplay[i.PosY][i.PosX + 1] = '|';
        theDisplay[i.PosY][i.PosX - 2] = '|';
    } else if (t == SubscriptionType::Set){
        // set the num
        string str = to_string(i.num);
        // get the num to a string
        if (str.length() != 1){ // when the num is two digital
            theDisplay[i.PosY][i.PosX - 1] = str[0];// the 1-st digit
            theDisplay[i.PosY][i.PosX] = str[1]; // the 2-nd digit 
        } else { // when the num is one digit
            theDisplay[i.PosY][i.PosX] = str[0];
        }
    } else if(t == SubscriptionType::RV){
        // set value and resources for tiles
        string str = to_string(i.num);
        // set value
        if (str.length() != 1){
            theDisplay[i.PosY][i.PosX - 1] = str[0];
            theDisplay[i.PosY][i.PosX] = str[1];
        } else {
            theDisplay[i.PosY][i.PosX] = str[0];
        }
        str = i.s;
        // set resource
        int length = str.length();
        for (int j = 0;j < length;++j){
            theDisplay[i.PosY - 1][i.PosX + j - (str.length() / 2)] = str[j];
        }
    } else if (t == SubscriptionType::Geese){
        string str = "GEESE";
        if (i.state == false){ // Erase the geese.
            int length = str.length();
            for (int j = 0;j < length;++j){
                theDisplay[i.PosY + 3][i.PosX + j - (str.length() / 2)] = ' ';
            }
        } else { // add geese
            int length = str.length();
            for (int j = 0;j < length;++j){
                theDisplay[i.PosY + 3][i.PosX + j - (str.length() / 2)] = str[j];
            }
        }
    } else if (t == SubscriptionType::Improve){
        // improve the building 
        string str = i.s;
        theDisplay[i.PosY][i.PosX - 1] = str[0];
        theDisplay[i.PosY][i.PosX] = str[1];
    } else if (t == SubscriptionType::Build){
        // build a basement in the address
        string str = i.s;
        theDisplay[i.PosY][i.PosX - 1] = str[0];
        theDisplay[i.PosY][i.PosX] = str[1];
    } else if (t == SubscriptionType::Clear){
        // clear all setted improve & build for paths and addresses
        string str = to_string(i.num);
        if (str.length() != 1){
            theDisplay[i.PosY][i.PosX - 1] = str[0];
            theDisplay[i.PosY][i.PosX] = str[1];
        } else {
            theDisplay[i.PosY][i.PosX] = str[0];
        }
    } else if (t == SubscriptionType::CL){
        // clear all setted resources & values for tiles
        theDisplay[i.PosY][i.PosX - 1] = ' ';
        theDisplay[i.PosY][i.PosX] = ' ';
        string str = i.s;
        int length = str.length();
        for (int j = 0;j < length;++j){
            theDisplay[i.PosY + 1][i.PosX + j - (str.length() / 2)] = ' ';
        }
    }
}
