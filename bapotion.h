#ifndef __BAPOTION_H__
#define __BAPOTION_H__
#include "potion.h"
#include <string>

class BAPotion: public Potion {
	static bool used;			//static field to check if potion is used or not
	public:
	static void reset();		//static field to reset potion to unused
	BAPotion(Floor*);			//constructor
	~BAPotion();				//destructor
	bool specialEvent(Player&);				//specialEvent method
	std::string getDescription();			//displays appropriate text description of the potion
};

#endif
