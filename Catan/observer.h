#ifndef __OBSERVER_H__
#define __OBSERVER_H__
#include "subscription.h"

class Subject;


class Observer{
public:
	virtual void notify(Subject& s, SubscriptionType t) = 0;
	virtual ~Observer() = default;
};

#endif
