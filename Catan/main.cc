#include <iostream>
#include <string>
#include "game.h"
#include "in.h"
#include <sstream>
#include <fstream>

using namespace std;


int main(int argc, char *argv[]){
    int seed = 0;
    bool seeded = true;
    istream* in = getRandLayout(seed);
    InputType ipt = InputType::Board;
    
    string filename = "lay.out";
    in->exceptions (ios::failbit|ios::eofbit);
    try {
        if (argc == 1) {
            filename = "layout.txt";
            in = new ifstream ("layout.txt");
        } else {
            string command1 = argv[1];
            if (command1 == "-seed") {
                string seedNum = argv[2];
                istringstream ss{seedNum};
                ss.exceptions(ios::failbit|ios::eofbit);
                try{
                    ss >> seed;
                }catch(ios::failure){
                    cerr << "The given seed is invalid" << endl;
                    cerr << "The seed will be set as default" << endl;
                }
                in = getRandLayout(seed);
            } else if (command1 == "-load") {
                filename = argv[2];
                if (filename == "-random-board"){
                    in = new ifstream ("layout.txt");
                    filename = "layout.txt";
                }
                else{ 
                    in = new ifstream (filename);
                    ipt = InputType::Player;
                }
            } else if (command1 == "-board") {
                string filename = argv[2];
                in = new ifstream (filename);
                seeded = true;
            } else if (command1 == "-random-board") {
                in = getRandLayout(seed);
            } else  {
                cout << "Invalid command" << endl;
                return 0;
            }
        }
    } catch(ios::failure){
        cerr << filename << " does not exist, a default board is generated" << endl;
        in = getRandLayout(seed);
    }
    istream* inn;
    if (seeded){
        inn = getRandLayout(seed);
    } else {
        inn = getRealRandLayout();
    }
    Game g {*inn,InputType::Board,seed,seeded};
    delete inn;
    try{
        Game tmp{*in,ipt, seed, seeded};
        g = tmp; // if the above throws the belew won't reach,
        // otherwise, move constructor will be called
    } catch(GameFailure){
    	ipt = InputType::Board;
    	cerr << "The Board you provided is not valid to use" << endl;
    	cerr << "We are operating a random board instead, sorry for that" << endl;
    }
    delete in;
    cin.exceptions(ios::eofbit);
    try {
        if (ipt == InputType::Board){
            if (!g.beginning()) return 0;
        }
        while(true){
            if (g.round()){
                cerr << "Would you like to play again?" << endl << ">";
                string s;
                while (true){
                    cin >> s;
                    cin.exceptions(ios::failbit|ios::eofbit);
                    if (s == "yes"){
                        if (seeded){
                            in = getRandLayout(seed);
                        } else {
                            in = getRealRandLayout();
                        }
                        g.restart(*in);
                        delete in;
                        g.beginning();
                    } else if (s == "no"){
                        cout << "Game Ends" << endl;
                        return 0;
                    } 
                }
            } 
        }
    }catch(...){
        cerr << "Game Ends" << endl;
    }
}
