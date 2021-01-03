#include "subject.h"
#include "observer.h"
void Subject::attach (Observer *o) {
	observer = o;
}

void Subject::notifyObserver(SubscriptionType t) {
	observer->notify(*this, t);
}
Subject::~Subject () {}
