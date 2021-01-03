#ifndef __TILE_H__
#define __TILE_H__
#include <map>
#include <vector>
#include "subscription.h"
#include "resource.h"
#include "subject.h"
#include "direction.h"


class Player;
class Address;
class Path;
class TextDisplay;
class Game;

class Tile: public Subject{
    bool Geese;
    Resource resource;
    int value, num;
    int PosX, PosY;
    
    std::map <Direction,Tile*> nearTiles;
    std::map <Direction,Address*> nearAd;
    std:: vector <Path*> nearPaths;
    void addDTile(Tile *t, Direction d);
    
public:
    Tile();
    void notifyObserver(SubscriptionType t) override;
    Info getInfo() override;
    int getValue();
    bool getState();
    void setRV(Resource r,const int& i);// set resource and value
    void setCoords(const int& x,const int& y);
    void setNum(const int& i);
    void addTile(Direction d, Tile *t);
    void clear(); // clear geese resource and value
    void geese(Player* p);
    void addAd(Address *a, Direction d);
    Tile* getNearTile(Direction d);
    Address* getNearAd(Direction d);
    void fillAd(std::vector <Address*>& va, std::vector <Path*>& vp, TextDisplay* td);
    ~Tile();
    void TakePlayer(Player *p, Game &g);// where nearly all input are taken,
};

 #endif
