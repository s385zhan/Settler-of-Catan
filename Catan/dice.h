#ifndef __DICE_H__
#define __DICE_H__

class Dice{
public:
	virtual int getValue() = 0;
	virtual int getValue(const int& seed) = 0; 
	virtual ~Dice() = 0;
};

class FairDice: public Dice{
public:
	int getValue() override;
	int getValue(const int& seed) override;
	~FairDice();
};

class LoadedDice: public Dice{
public:
	int getValue() override;
	int getValue(const int& seed) override;
	~LoadedDice();
};

#endif


