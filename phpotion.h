#ifndef __PHPOTION_H__
#define __PHPOTION_H__
#include "potion.h"
#include <string>

class PHPotion: public Potion {
	static bool used;				//static field to check if potion is used or not
	public:
	PHPotion(Floor*);				//Constructor
	~PHPotion();					//Destructor
	static void reset();			//static field to reset potion to unused
	bool specialEvent(Player&);			//specialEvent method
	std::string getDescription();		//displays the appropriate text description of the potion
};

#endif
