#include <string>
#include "path.h"
#include "player.h"
#include "info.h"
#include "address.h"
#include "subscription.h"
using namespace std;

// Consturctor
Path::Path (const int& PosX, const int& PosY):
owner{nullptr}, num{0}, PosX{PosX}, PosY{PosY}{}

// Check if one of the adjacent Road is built by myself.
bool Path::buildableRoad (Player *p) {
    for (auto address : Ads) {
        if (address->buildableRoad(p)) return true;
    }
    return false;
}

// Check if one of the adjacent Res is empty.
bool Path::buildableRes (Player *p) {
    for (auto address : Ads) {
        if (address->getOwner() != p) return false;
    }
    return true;
}



// Retrun whether there are no residence near this path.
bool Path::noRes() {
    for (auto it : Ads) {
        if (it->getOwner ()) return false;
    } return true;
}

// Show who buit this road.
Player *Path::getOwner () {
    return owner;
}

// Get information of this path.
Info Path::getInfo() {
    Info i;
    i.PosX = PosX;
    i.PosY = PosY;
    i.num = num;
    i.state = (owner != nullptr);
    if (owner != nullptr){
        string s = owner->getColour();
        i.s += s[0];
        i.s += 'R';
    }
    return i;
}

// Set number of the path.
void Path::setNum(int i) {
    num = i;
    notifyObserver(SubscriptionType::Set);
}

// Return current number of this address.
int Path::getNum () {return num;}


// Push back address in the vector.
void Path::addAds (Address *a) {
    Ads.emplace_back(a);
}

// Build a roda on this path.
void Path::purchase(Player *p) {
    owner = p;
    notifyObserver(SubscriptionType::Build);
}

// Clear owner.
void Path::clear () {
    owner = nullptr;
    notifyObserver(SubscriptionType::Clear);
}

// Destructor.
Path::~Path () {}
