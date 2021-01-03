#include "subscription.h"
#ifndef __SUBJECT_H__
#define __SUBJECT_H__


class Observer;
class Info;

class Subject{
protected:
	Observer* observer;
public:
	virtual void notifyObserver(SubscriptionType t);
	virtual Info getInfo() = 0;
	void attach(Observer* o);
	virtual ~Subject () = 0;
};

#endif
