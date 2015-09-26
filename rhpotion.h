#ifndef __RHPOTION_H__
#define __RHPOTION_H__
#include "potion.h"
#include <string>

class RHPotion: public Potion {
	static bool used;				//static field to check if potion is used or not
	public:
	RHPotion(Floor*);				//constructor 
	~RHPotion();					//destructor
	static void reset();			//static field to reset potion to unused
	bool specialEvent(Player&);			//speicalEvent method
	std::string getDescription();		//displays the appropriate text description of the potion
};
#endif
		
