#include <string>
#include "address.h"
#include "path.h"
#include "level.h"
#include "info.h"
#include "player.h"
using namespace std;

// ctor
Address::Address (const int& PosX,const int& PosY): owner{nullptr}, level{Level::None}, num{0}, 
PosX{PosX}, PosY{PosY}{}

// Check if one of the adjacent road is built by myself.
bool Address::buildableRoad (Player *p) {
    if (owner == p) return true;
    for (auto r : paths) {
        if (r->getOwner() == p) return true;
    } return false;
}

// Check if current address and its adjacent address are all empty.
bool Address::buildableRes () {
    if (this->owner == nullptr) {
        for (auto r : paths) {
            if (!r->noRes()) return false;
        } return true;
    } return false;
}




Info Address::getInfo(){
    Info i;
    i.PosX = PosX;
    i.PosY = PosY;
    i.num = num;
    i.state = (owner != nullptr);
    if (owner != nullptr){
        string s = owner->getColour();
        i.s += s[0];
        if (level == Level::Basement){
            i.s += 'B';
        } else if (level == Level::Tower){
            i.s += 'T';
        } else{
            i.s += 'H';
        }
    }
    return i;
}


void Address::setNum(const int& i){
    num = i;
    notifyObserver(SubscriptionType::Set);
}

int Address::getNum () {return num;}

void Address::addPath(Path *p){
    paths.emplace_back(p);
}

// Show current level
Level Address::getLevel () {
    return level;
}

// Show who built this Res (if any)
Player *Address::getOwner () {
    return owner;
}

// Improve current Res.
void Address::improve () {
    if (level == Level::Basement) level = Level::House;
    else if (level == Level::House) level = Level::Tower;
    notifyObserver(SubscriptionType::Improve);
}

// build a basement at current address.
void Address::purchase (Player *p) {
    owner = p;
    level = Level::Basement;
    notifyObserver(SubscriptionType::Build);
}

bool Address::havePath (Path *p) {
    for (auto it : paths) {
        if (it == p) return true;
    } return false;
}

Path *Address::sharedPath(Address *a) {
    for (auto it : paths) {
        if (a->havePath (it)) return it;
    } return nullptr;
}


// clear owner and back to none level.
void Address::clear () {
    owner = nullptr;
    level = Level::None;
    notifyObserver(SubscriptionType::Clear); 
}

// dtor 
Address::~Address () {}

