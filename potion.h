#ifndef __POTION_H__
#define __POTION_H__
#include "item.h"
#include <string>

class Potion: public Item {
	int value;					//potion has value of either 5 or 10
	public:
	Potion(Floor*,int value);		//constructor
	virtual bool specialEvent(Player&);
	int getVal();
	bool canUse();									//true for all potions
	virtual std::string getDescription()=0;				//virtual method
};

#endif
