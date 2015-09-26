#ifndef __BDPOTION_H__
#define __BDPOTION_H__
#include "potion.h"
#include <string>

class BDPotion: public Potion {
	static bool used;			//static field to check if potion is used or not
	public:
	BDPotion(Floor*);			//constructor
	~BDPotion();				//destructor	
	static void reset();				//static field to reset potion to unused
	bool specialEvent(Player&);			//specialEvent method
	std::string getDescription();		//displays the appropriate text description of the potion
};

#endif
