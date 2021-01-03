#include "player.h"
#include <iostream>
#include <string>
#include "address.h"
#include "path.h"

using namespace std;

// Constructor.
Player::Player (string colour) :colour {colour}{
    Procession.emplace(Resource::BRICK,0);
    Procession.emplace(Resource::ENERGY,0);
    Procession.emplace(Resource::GLASS,0);
    Procession.emplace(Resource::HEAT,0);
    Procession.emplace(Resource::WIFI,0);
}


// Trade between the palyers.
void Player::Trade (Player *others, Resource take, Resource give) {
    if (others->haveNothing(take)) {
        cout << "Player " << others->getColour() << " does not have this resource." << endl;
        return ;
    } else if (haveNothing(give)) {
        cout << "You have nothing to trade." << endl;
        return ;
    }
    Procession[take]++;
    Procession[give]--;
    others->stolen(take);
    others->receive(give, 1);
    cout << "Trade succeeded" << endl;
}

// Get the residence.
void Player::getRes (Address *a) {
    ownedRes.emplace_back(a);
    a->purchase(this);
}


void Player::getRoad (Path *p) {
    ownedRoad.emplace_back (p);
    p->purchase(this);
}

// To check if this path has an adjacent road or adjacent Residence of mine.
bool Player::buildable (Path *p) {
    if (p->buildableRoad(this) || p->buildableRes(this)) return true;
    else return false;
}

// To check if this address has an adjacent road of mine and does not have an adjacent others' residences.
bool Player::buildable (Address *a) {
    if (a->buildableRoad (this) && a->buildableRes()) return true;
    else return false;
}

// To build a base.
bool Player::buildBase (Address *a) {
    if (a->getOwner()) {
        cerr << "This address has already been taken." << endl;
        return false;
    }
    if (buildPoints() < 2) {
        a->purchase(this);
        ownedRes.emplace_back (a);
        return true;
    } else if (buildable (a)) {
        if (Procession[Resource::BRICK] >= 1 && Procession[Resource::ENERGY] >= 1 &&
            Procession[Resource::GLASS] >= 1 && Procession[Resource::WIFI] >= 1) {
            a->purchase(this);
            ownedRes.emplace_back(a);
            Procession[Resource::BRICK]--;
            Procession[Resource::ENERGY]--;
            Procession[Resource::GLASS]--;
            Procession[Resource::WIFI]--;
            return true;
        } else {
            cout << "You do not have enough resources" << endl;
            return false;
        }
    }else {
        cout << "You can not build here" << endl;
        return false;
    }
}

// To build a road.
void Player::buildRoad (Path *p) {
    if (buildable(p)) {
        if (Procession[Resource::HEAT] >= 1 && Procession[Resource::WIFI] >= 1){
            p->purchase (this);
            ownedRoad.emplace_back (p);
            Procession[Resource::HEAT]--;
            Procession[Resource::WIFI]--;
        } else cout << "You do not have enough resources" << endl;
    } else cout << "You can not build here" << endl;
}

// To improve your exist residence.
void Player::improve (Address *a) {
    if (a->getOwner() == this) {
        if (a->getLevel() == Level::Basement) {
            if (Procession[Resource::GLASS] >= 2 && Procession[Resource::HEAT] >= 3) {
                a->improve ();
                Procession[Resource::GLASS] -= 2;
                Procession[Resource::HEAT] -= 3;
            } else cout << "You do not have enough resources" << endl;
        } else if (a->getLevel() == Level::House) {
            if (Procession[Resource::BRICK] >= 3 && Procession[Resource::ENERGY] >= 2 &&
                Procession[Resource::GLASS] >= 2 && Procession[Resource::HEAT] >= 2 && Procession[Resource::WIFI] >= 1) {
                a->improve();
                Procession[Resource::BRICK] -= 3;
                Procession[Resource::ENERGY] -= 2;
                Procession[Resource::GLASS] -= 2;
                Procession[Resource::HEAT] -= 2;
                Procession[Resource::WIFI] -= 1;
            } else cout << "You do not have enough resources" << endl;
        }
    } else cout << "You can not build here" << endl;
}

// Receive resource at this Tile.
void Player::receive (Resource r,const int& i) {
    Procession[r] += i;
}



// When meet geese all kinds of Poceesion which are greater than 10 will be lost arbitrary number,
// and total amount of these Pocession becomes half of before.
void Player::lose () {
    map <Resource, int> greater10;
    int total = 0;
    for (auto &r : Procession) {
        if (r.second >= 10) {
            greater10[r.first] = r.second;
            total += r.second;
        }
    } int aHalf = total / 2;
    int largest = 0;
    if (greater10.size () == 0) return;
    for (auto &r : greater10) {
        if (r.second > largest) largest = r.second;
    } int min = 0;
    while (true) {
        if (greater10.size() == 1) {
            auto p = greater10.begin();
            p->second -= aHalf;
            Procession[p->first] = p->second;
            greater10.clear();
            break;
        } if (aHalf > largest) min = aHalf - largest;
        auto p = greater10.begin();
        int lose = rand() % (p->second - min + 1);
        Procession[p->first] -= lose;
        aHalf -= lose;
        greater10.erase (p->first);
    }
}


ostream &operator<< (ostream &out, Resource r) {
    if (r ==  Resource::BRICK) out << "BRICK";
    if (r == Resource::ENERGY) out << "ENERGY";
    if (r == Resource::GLASS) out << "GLASS" ;
    if (r == Resource::HEAT) out << "HEAT" ;
    if (r == Resource::WIFI) out << "WIFI";
    return out;
}

// Return whether the player have nothing.
bool Player::haveNothing (Resource r) {
    if (r == Resource::ALL) {
        for (auto &it : Procession) {
            if (it.second) return false;
        } return true;
    } else {
        if (Procession[r] == 0) return true;
        else return false;
    }
}


Resource Player::stealWhat () {
    map <Resource, int> range;
    int low = 0;
    for (auto &it : Procession) {
        range[it.first] = it.second + low;
        low = range[it.first];
    } int prob = rand() % low + 1;
    for (auto &it : range) {
        if (!Procession[it.first]) continue;
        int leftEnd = range[it.first] - Procession[it.first] + 1;
        int rightEnd = range[it.first];
        if (prob >= leftEnd && prob <= rightEnd) {
            Procession[it.first]--;
            cout << it.first << endl;
            return it.first;
        }
    }
    return Resource::ALL;
}

// Steal a resource r from Player p.
void Player::steal (Player *others) {
    if (others->haveNothing(Resource::ALL)) {
        cout << "You have nothing to steal." << endl;
        return ;
    }
    Resource r = others->stealWhat();
    Procession[r]++;
    cout << "Builder " << colour << " steals " << r << " from builder " << others->getColour() << endl;
}




// Be stolen a resource r by others.
void Player::stolen (Resource r) {
    Procession[r]--;
}

// Show my colour.
string Player::getColour () {
    return colour;
}

// Show what I have currently.
ostream& Player::getStatus (ostream& out) {
    out << Procession[Resource::BRICK] << " " << Procession[Resource::ENERGY] << " "
    << Procession[Resource::HEAT] << " " << Procession[Resource::GLASS] << " " << Procession[Resource::WIFI] << " ";
    out << "r ";
    for (auto it : ownedRoad) {
        out << it->getNum() << " ";
    } out << "h";
    for (auto it : ownedRes) {
        out << " " << it->getNum() << " ";
        if (it->getLevel() == Level::Basement) out << "B";
        if (it->getLevel() == Level::House) out << "H";
        if (it->getLevel() == Level::Tower) out << "T";
    } out << endl;
    return out;
}

int Player::buildPoints () {
    int total = 0;
    for (auto it : ownedRes) {
        if (it->getLevel () == Level::Basement) total++;
        else if (it->getLevel () == Level::House) total += 2;
        else if (it->getLevel () == Level::Tower) total += 3;
    } return total;
}

// Print the status of the player.
void Player::printStatus () {
    cout << colour << " has " << buildPoints() << " building points, "
    << Procession[Resource::BRICK] << " brick, " << Procession[Resource::ENERGY] << " energy, "
    << Procession[Resource::GLASS] << " glass, " << Procession[Resource::HEAT] << " heat, and "
    << Procession[Resource::WIFI] << " WiFi." << endl;
}

// Print the residence that the palyer has.
void Player::printRes () {
    cout << "h";
    for (auto it : ownedRes) {
        cout << " " << it->getNum() << " ";
        if (it->getLevel() == Level::Basement) cout << "B";
        if (it->getLevel() == Level::House) cout << "H";
        if (it->getLevel() == Level::Tower) cout << "T";
    } cout << endl;
}

// Retrun whether the player is win.
bool Player::isWon(){
    return (buildPoints() >= 10);
}

// Clear the player.
void Player::clear() {
    vector <Path*> pv;
    vector <Address*> av;
    for (auto &mp : Procession){
        mp.second = 0;
    }
    ownedRoad = pv;
    ownedRes = av;
}

// Destructor.
Player::~Player () {}
