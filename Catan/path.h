#ifndef __PATH_H__
#define __PATH_H__
#include <vector>
#include "subscription.h"
#include "subject.h"

class Player;
class Address;
class Info;

class Path: public Subject{
	Player* owner;
	int num;
	int PosX, PosY;
	std::vector<Address*> Ads;
public:
	Path (const int& PosX, const int& PosY);
	bool buildableRoad(Player* p);
	bool buildableRes(Player* p);
	Info getInfo() override;
	void setNum(int i);
	int getNum (); // return the number of this path;
	void addAds(Address *a);
	bool noRes();// return true if all near address have no owner
	void purchase (Player *p); // Player builds a road on this path.
	void clear(); // clear owner
	Player* getOwner();
	~Path();
};

#endif
