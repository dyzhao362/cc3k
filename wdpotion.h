#ifndef __WDPOTION_H__
#define __WDPOTION_H__
#include "potion.h"
#include <string>

class WDPotion: public Potion {
	static bool used;			//static field to check if potion is used or not
	public:
	WDPotion(Floor*);			//constructor
	~WDPotion();				//destructor
	static void reset();			//static field to reset potion to unused
	bool specialEvent(Player&);			//specialEvent method
	std::string getDescription();		//displays appropriate text description of the potion
};

#endif
