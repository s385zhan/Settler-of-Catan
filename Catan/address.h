#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include <vector>
#include "subject.h"
#include "subscription.h"
#include "level.h"

class Player;
class Path;

class Address: public Subject{
    Player* owner;
    Level level;
    int num;
    int PosX, PosY;
    std::vector<Path*> paths;
    
public:
    Address (const int& PosX,const int& PosY);
    bool buildableRoad(Player *p);  // To check if one of the adjacent roads is built by this player.
    bool buildableRes(); // To check if this current address and its adjacent adresses are "empty"
    Info getInfo() override;
    //void setCoords(int x,int y);
    void setNum(const int& i);
    int getNum ();
    void addPath(Path *p);
    Level getLevel (); // To see what Level of current residence is. (One of BASE, HOUSE TOWER).
    Player* getOwner (); // To see who build this residence.
    void improve (); // Improve current residence.
    void purchase (Player *p); // Player builds a base at this address.
    bool havePath (Path *p); // See if this path is adjacent to the address.
    Path* sharedPath (Address *a); // See if this address shares a same path p with address a.
    void clear(); // clear owner and back to none level
    ~Address();
};


#endif
