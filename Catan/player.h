#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <map>
#include <vector>
#include <string>
#include "subscription.h"
#include "resource.h"

class Path;
class Address;
class Game;

class Player {
    std::string colour;
    std::map <Resource, int> Procession;
    std::vector <Path *> ownedRoad;
    std::vector <Address *> ownedRes;
    
    bool buildable (Path *p);
    bool buildable (Address *a);
    bool agreeToTrade();
    
public:
    Player (std::string colour);
    void Trade (Player *others, Resource take, Resource give);// take is the resource received
    // give is the one taken
    bool buildBase (Address *a);
    void improve (Address *a);
    void buildRoad (Path *p);
    void getRes (Address *a);
    void getRoad (Path *p);
    void receive (Resource r,const int& i); // Stay at a tile,
    // receive corresponding resources according to adjacent residences' level
    void lose (); // When met geese
    void clear();
    std::ostream& getStatus (std::ostream &out); // print all owned pocessions, and return out at the end.
    bool isWon(); // true if has more than 10 residence
    void printStatus ();
    void printRes(); // print all the residences of this player
    bool haveNothing (Resource r);
    Resource stealWhat();
    void steal (Player *others);
    void stolen (Resource r);
    int buildPoints ();
    std::string getColour ();
    ~Player();
};

#endif
