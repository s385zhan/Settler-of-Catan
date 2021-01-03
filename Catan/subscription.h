#ifndef __SUBSCRIPTION_H__
#define __SUBSCRIPTION_H__

enum class SubscriptionType{
	Geese, Improve, Build, Set, Clear,
	SetPH,// Set path horizontally.
	SetPV, // Set path vertically.
	RV, // Set resource and value.
	SetAd, // Set address.
	CL // Clear Tile.
};

#endif
