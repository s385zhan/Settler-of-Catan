#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <vector>
#include "game.h"
#include "info.h"
#include "direction.h"
#include "textdisplay.h"
#include "player.h"
#include "input.h"
#include "tile.h"
#include "path.h"
#include "address.h"
#include "dice.h"

using namespace std;

// To compare every path and address ever produced by their coordinations
// and assign their corresponding number to them starting from 0.
bool operator<(const Info& i1,const Info& i2){
    if (i1.PosY == i2.PosY){ // If they are in the same row.
        return (i1.PosX < i2.PosX); // Assign sammler value to lefter subject.
    }
    return (i1.PosY < i2.PosY);// Assign smaller value to higer subject.
}


// Merge sort.
void merge(vector<Path*>& dest, vector<Path*>& left, vector<Path*>& right){
    int pos1 = 0;
    int pos2 = 0;
    int len1 = left.size();
    int len2 = right.size();
    for (int i = 0;i < len1 + len2;++i){
        if (pos1 == len1){
            right[pos2]->setNum(i);
            dest[i] = right[pos2];
            ++pos2;
        } else if (pos2 == len2){
            left[pos1]->setNum(i);
            dest[i] = left[pos1];
            ++pos1;
        } else {
            Info i1 = left[pos1]->getInfo();
            Info i2 = right[pos2]->getInfo();
            if (i2 < i1){
                right[pos2]->setNum(i);
                dest[i] = right[pos2];
                ++pos2;
            } else {
                left[pos1]->setNum(i);
                dest[i] = left[pos1];
                ++pos1;
            }
        }
    }
}

void sort (vector<Path*>& v){
    if (v.size() <= 1) return;
    int len = v.size();
    int llen = len / 2;
    int rlen = len - llen;
    vector <Path*> left;
    vector <Path*> right;
    for (int i = 0;i < llen;++i) {
        left.emplace_back(v[i]);
    }
    for (int i = 0;i < rlen;++i){
        right.emplace_back(v[i + llen]);
    }
    sort(left);
    sort(right);
    merge(v,left,right);
}

// Merge sort.
void merge(vector<Address*>& dest, vector<Address*>& left, vector<Address*>& right){
    int pos1 = 0;
    int pos2 = 0;
    int len1 = left.size();
    int len2 = right.size();
    for (int i = 0;i < len1 + len2;++i){
        if (pos1 == len1){
            right[pos2]->setNum(i);
            dest[i] = right[pos2];
            ++pos2;
        } else if (pos2 == len2){
            left[pos1]->setNum(i);
            dest[i] = left[pos1];
            ++pos1;
        } else {
            Info i1 = left[pos1]->getInfo();
            Info i2 = right[pos2]->getInfo();
            if (i2 < i1){
                right[pos2]->setNum(i);
                dest[i] = right[pos2];
                ++pos2;
            } else {
                left[pos1]->setNum(i);
                dest[i] = left[pos1];
                ++pos1;
            }
        }
    }
}

void sort (vector<Address*>& v){
    if (v.size() <= 1) return;
    int len = v.size();
    int llen = len / 2;
    int rlen = len - llen;
    vector <Address*> left;
    vector <Address*> right;
    for (int i = 0;i < llen;++i) {
        left.emplace_back(v[i]);
    }
    for (int i = 0;i < rlen;++i){
        right.emplace_back(v[i + llen]);
    }
    sort(left);
    sort(right);
    merge(v,left,right);
}

GameFailure::GameFailure(){  }

// readin(in,input) read istream "in" in by the given InputType input.
void Game::readin(istream& in, InputType input){
    if (input == InputType::Player){ // For loading saved game only.
        string s;
        getline(in,s);
        istringstream sss{s};
        sss.exceptions(ios::failbit);

        try{
            sss >> numTurn; // The first number if the curTurn.
        } catch(...){
            GameFailure gf;
            throw gf;
            return;
        }

        for (auto &it : players){// four loops 
            bool shouldthrow = true;
            getline(in,s);
            istringstream sss{s};
            int count = 0; // In order to know which resource it is reading now
            while (sss >> s){
                if (s == "r"){
                    break; // Housing read in begins.
                }else {
                    int resourceNum;
                    istringstream iss{s};
                    iss.exceptions(ios::failbit);
                    try{
                        iss >> resourceNum;
                        // the read in will continue, 
                        // only if resourceNum is successfully read in
                    } catch(...){
                       GameFailure gf;
                       throw gf; // throw a GameFailure
                       return; 
                    }
                    Resource r;
                    if (count == 0){
                        r = Resource::BRICK;
                    } else if (count == 1){
                        r = Resource::ENERGY;
                    } else if (count == 2){
                        r = Resource::GLASS;
                    } else if (count == 3){
                        r = Resource::HEAT;
                    } else { // Count is 4.
                        r = Resource::WIFI;
                    }
                    ++count;
                    it.second->receive(r,resourceNum);
                }
            }
            while(sss >> s){
                if (s == "h") {
                    shouldthrow = false; // house read means the read in is full
                    break; // Housing read in begins.
                }
                int j; 
                istringstream sss{s};
                sss.exceptions(ios::failbit);
                try{
                    sss >> j; // Turn s into an integer, which is the address that
                    // current player built a road.
                } catch(...){
                    GameFailure gf;
                    throw gf; // throw a GameFailure if read in fails
                    return; 
                }
                it.second->getRoad(paths[j]);// Receive the promised road
            }
            
            while (sss >> s){ // No need to add a haveRes 
                // since if not, ss >> s won't work
                int j; 
                istringstream iss{s};
                iss.exceptions(ios::failbit);
                try{
                    iss >> j;
                } catch(...){
                    GameFailure gf;
                    throw gf;
                    return;
                }
                it.second->getRes(addresses[j]);
                sss >> s; // Read housing type
                if (s == "T"){
                    addresses[j]->improve();
                    addresses[j]->improve();
                } else if (s == "H"){
                    addresses[j]->improve();
                } else if (s == "B"){
                    // nothing done in this case
                }else {
                    GameFailure gf;
                    throw gf;
                    return;
                }
            }
            if (shouldthrow){
                GameFailure gf;
                throw gf;
                return;
            }
        }
    }
    string s;
    int num;
    Resource r;
    for(int count = 0;count < 19;++count){// Board read in begins,
    // and make sure all 19 tiles are full read
        in.exceptions(ios::failbit);
        try{
            in >> s;
        } catch(...){
            GameFailure gf;
            throw gf;// if not enough tiles can be fully read, then throw
            return;
        }
        istringstream iss {s};
        iss.exceptions(ios::failbit);
        try{
            iss >> num;
        } catch(...){
            GameFailure gf;
            throw gf;
            return;
        }
        if (num == 0){
            r = Resource::BRICK;
        } else if (num == 1){
            r = Resource::ENERGY;
        } else if (num == 2){
            r = Resource::GLASS;
        } else if (num == 3){
            r = Resource::HEAT;
        } else if (num == 4){
            r = Resource::WIFI;
        } else {
            r = Resource::PARK;
        }
        try{
            in >> s;
        } catch(...){
            GameFailure gf;
            throw gf;
            return;
        }
        istringstream sss{s};
        sss.exceptions(ios::failbit);
        try{
            sss >> num;
        } catch(...){
            GameFailure gf;
            throw gf;
            return;
        }
        tiles[count]->setRV(r,num);
    }
}

// Text display the board.
void Game::print(){
    td->print();
}


Game::Game(istream& in, InputType i,const int& seed,const bool& seeded){
    this->numTurn = 0;
    this->seed = seed;
    this->seeded = seeded;
    const int playerNum = 4; // Only 4 players are allowed.
    td = new TextDisplay;
    for (int tmp = 0;tmp < playerNum;++tmp){
        string s;
        if (tmp == 0){
            s = "Blue";
        } else if (tmp == 1){
            s = "Red";
        } else if (tmp == 2){
            s = "Orange";
        } else { // Where tmp is 3.
            s = "Yellow";
        }
        players[s] = new Player(s);
    }
    
    for (int tmp = 0;tmp < 19;++tmp){// 19 tiles are generated.
        Tile* t = new Tile;
        t->attach(td);
        if (tmp == 0){
            t->setCoords(27,2); // By calcualtion.
        } else if (tmp == 18){
            t->setCoords(27,2 + 8 * 4); 
            t->addTile(Direction::RU,tiles[tmp - 1]);// RightUpper tile.
            t->addTile(Direction::LU,tiles[tmp - 2]);// LeftUpper tile.
            t->addTile(Direction::Up,tiles[tmp - 4]);// Upper tile.
        }else if (tmp % 5 == 1){// Midleft tiles.
            t->setCoords(17,8 * ((tmp / 5) + 1) - 2);
            if (tmp == 1){
                t->addTile(Direction::RU,tiles[0]);// Special case for 1.
            } else {
                t->addTile(Direction::RU,tiles[tmp - 2]); // By calculation.
                t->addTile(Direction::LU,tiles[tmp - 3]);
                t->addTile(Direction::Up,tiles[tmp - 5]);
            }
        }else if (tmp % 5 == 2){ // Midright tiles.
            t->setCoords(37,8 * ((tmp / 5) + 1) - 2);
            if (tmp == 2){
                t->addTile(Direction::LU,tiles[0]);
            }else {
                t->addTile(Direction::RU,tiles[tmp - 2]);
                t->addTile(Direction::LU,tiles[tmp - 3]);
                t->addTile(Direction::Up,tiles[tmp - 5]);
            }
        } else if (tmp % 5 == 0){// Right tiles.
            t->setCoords(47,2 + 8 * (tmp / 5));
            if (tmp == 5){
                t->addTile(Direction::LU,tiles[2]);
            }else {
                t->addTile(Direction::LU,tiles[tmp - 3]);
                t->addTile(Direction::Up,tiles[tmp - 5]);
            }
        } else if (tmp % 5 == 3){// Left tiles.
            t->setCoords(7,2 + 8 * ((tmp / 5) + 1));
            if (tmp == 3){
                t->addTile(Direction::RU,tiles[1]);
            }else {
                t->addTile(Direction::RU,tiles[tmp - 2]);
                t->addTile(Direction::Up,tiles[tmp - 5]);
            }
        } else {// Middle tiles.
            t->setCoords(27,2 + 8 * (1 + (tmp / 5)));
            t->addTile(Direction::RU,tiles[tmp - 2]);
            t->addTile(Direction::LU,tiles[tmp - 3]);
            if (tmp == 4){
                t->addTile(Direction::Up,tiles[0]);// Upper is first tile.
            }else{
                t->addTile(Direction::Up,tiles[tmp - 5]);
            }
        }
        t->setNum(tmp);
        t->fillAd(addresses,paths,td); // Fill all non-set addresses of this tile.
        tiles.emplace_back(t);
    }
    sort(addresses); // Sort them to get their corresponding number.
    sort(paths);
    
    readin(in,i); // Read in begins.
    if (i == InputType::Player){
        print();
    }
}

// Destructor.
Game::~Game(){
    for (auto p : paths){
        delete p; // Delete every paths.
    }
    for (auto a : addresses){
        delete a; // Delete every address.
    }
    for (auto t : tiles){
        delete t; // Delete every tile.
    }
    for (auto p : players){
        delete p.second; // Delete every player.
    }
    delete td; // Delete textdisplay.
}

// copy assignment operator operating same as move assignment operator
Game& Game::operator=(Game& other){
    td = other.td;
    other.td = nullptr;
    swap(other.seed,seed);
    swap(other.seeded,seeded);
    swap(other.players,players);
    swap(other.tiles,tiles);
    swap(paths,other.paths);
    swap(addresses,other.addresses);
    return *this;
}

// move assignment operator
Game& Game::operator=(Game&& other){
    cerr << "ee" << endl;
    td = other.td;
    other.td = nullptr;
    swap(other.seed,seed);
    swap(other.seeded,seeded);
    swap(other.players,players);
    swap(other.tiles,tiles);
    swap(paths,other.paths);
    swap(addresses,other.addresses);
    return *this;
}

// Roll fair dice.
Dice* Game::rollDice(){
    return new FairDice; // Get a fair dice.
}

// Roll loaded dice.
Dice* Game::rollLoadedDice(){
    return new LoadedDice;// Get a loaded dice.
}

void Game::geese(Tile* t, Player* p){
    for (auto it : tiles){
        if (it->getState()){
            it->geese(p); // Set the geese off.
            break;
        }
    }
    t->geese(p); // Set the geese on.
    for (auto &p : players){
        p.second->lose(); // Lose half.
    }
}

// Allow every player to build two basement at the beginning.
bool Game::beginPlay(Player* p){
    cout << "Builder " << p->getColour() << "'s turn." << endl;
    cerr << "Where do you want to build a basement?" << endl;
    while(true){
        int i;
        cerr << ">";
        cin >> i;
        cin.exceptions(ios::eofbit);
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cerr << "Wrong Input." << endl;
        } else if (i >- 1 && i < 54 && p->buildBase(addresses[i])){ 
        // If build in succeeds, then break 
            break;
        } else if (i < 0 || i > 53){
            cerr << "No such address exists" << endl;
        }                                                                                                                                                                   
    } return true;
}

// The term for player p to play.
bool Game::eachPlay(Player* p){
    cerr << p->getColour() <<"'s turn" << endl;
    p->printStatus();
    cerr << "Please choose a dice to roll, load or fair?" << endl;
    int i;
    string s;
    Dice *d;
    
    // Time for choosing a dice.
    while (true){
        cerr << ">";
        cin >> s;
        cin.exceptions(ios::failbit|ios::eofbit);
        if (s == "load"){// Choose loaded dice.
            d = rollLoadedDice();
            i = d->getValue();
            delete d;
            break;
        }
        if (s == "fair"){// Choose fair dice.
            d = rollDice();
            if (seeded){
                i = d->getValue(seed);
            } else {
                i = d->getValue();
            }
            delete d;
            break;
        }
        else cerr << "You haven't choose your dice." << endl;
    }
    
    // Roll the dice
    while (true){
        cerr << ">";
        cin >> s;
        cin.exceptions(ios::failbit|ios::eofbit);
        if (s == "roll"){// Get the result from the dice.
            cerr << "Your Result is " << i << endl;
            break;
        }
    }
    // When the number is 7.
    if (i == 7) {
        cout << "Choose where to place the GEESE." << endl;
        int j;
    // Put the geese to a tile.
        while (true){
            cerr << ">";
            cin >> j;
            cin.exceptions(ios::eofbit);
            if (cin.fail()){// Input is not a number.
                cin.clear();
                cin.ignore();
                cerr << "Invalid Input." << endl;
            } else if (j < 19 && j >= 0){
                geese(tiles[j],p);
                break;
            } else {// The number is out of range.
                cerr << s <<" is out of range" << endl;
            }
        }
    
    
        for (auto &t : tiles){
            if (t->getValue() == 7){
                t->TakePlayer(p,*this); // During the turn.
                break;
            }
        }
    } else {
        int t; // Planned tile to visit.
        while (true){
            cerr << "Please choose a tile to visit, and put a number." << endl;
            cerr << ">";
            cin >> t;
            cin.exceptions(ios::eofbit);
            if (cin.fail()){
                cerr << "Invalid input, try an integer." << endl;
                cin.clear();
                cin.ignore();
            } else if (i == tiles[t]->getValue()){
                tiles[t]->TakePlayer(p,*this); // During the turn.
                break;
            }
            cerr << "This tile is not avaliable, please change a tile to visit" << endl;
        }
    }
    if (p->isWon()){ // if won
        cerr << "Congraglation, you win." << endl;
        return true;
    }
    return false;
}

// Call the Player.
Player* Game::getPlayer(const string& s){
    return players[s];
}

// Begin the game.
bool Game::beginning(){
    for (auto it : players){ // building basement begins
        // in order to get the order from Blue, Red, Orange, Yellow
        if(beginPlay(it.second) == false) {
            return false;
        }
    }
    for (auto it = players.rbegin();it != players.rend();++it){
        // in order to get the order from Yellow, Orange, Red, Blue
        if (beginPlay(it->second) == false) return false;
    }
    print();
    return true;
}

// Clear all the paths, addresses, tiles and players.
void Game::restart(istream& in){
    // clear all part of the game
    for (auto p : paths){
        p->clear();
    }
    for (auto a : addresses){
        a->clear();
    }
    for (auto t : tiles){
        t->clear();
    }
    for (auto p : players){
        p.second->clear();
    }
    readin(in,InputType::Board);
}

// Return true if someone won.
bool Game::round(){
    cout << "Round: " << numTurn << endl;
    ++numTurn;
    for (auto &p : players){
        if (eachPlay(p.second)){
            return true;
        }
    }
    return false;
}

// Print the status of each palyer.
void Game::printStatus(){
    for (auto &p : players){
        p.second->printStatus();
    }
}

// Save the game to ostream.
ostream& Game::save(ostream& out){
    out << numTurn << endl;// The number of the round.
    
    for (auto &p : players){// Get all the players' status.
        p.second->getStatus(out);
    }
    char test = 'f'; // test char
    for (auto t : tiles){// Get the information of each tile.
        if (test == 't') out << " ";
        Info info = t->getInfo();
        if (info.s == "BRICK"){
            out << 0;
        } else if (info.s == "ENERGY"){
            out << 1;
        } else if (info.s == "GLASS"){
            out << 2;
        } else if (info.s == "HEAT"){
            out << 3;
        } else if (info.s == "WIFI"){
            out << 4;
        } else {
            out << 5;
        }
        out << " ";
        out << t->getValue();
        test = 't';
    }
    out << endl;
    return out;
}
