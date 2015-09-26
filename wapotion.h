#ifndef __WAPOTION_H__
#define __WAPOTION_H__
#include "potion.h"
#include <string>

class WAPotion: public Potion {
	static bool used;			//static field to check if potion is used or not
	public:
	WAPotion(Floor*);				//constructor
	~WAPotion();					//destructor
	static void reset();			//static field to reset potion to unused
	bool specialEvent(Player&);		//specialEvent method
	std::string getDescription();		//displays the appropriate text description of the potion
};

#endif
