#ifndef __ITEM_H__
#define __ITEM_H__
#include <vector>
#include "event.h"

class Item: public Event{
	public:
	Item(char,Floor*);
	virtual ~Item()=0;								//virtual destructor
	virtual bool canPass(const Mob&);				//virtual method
	virtual bool canPass(const Player&);				//virtual method
	virtual bool canUse();								//virtual method
	virtual bool specialEvent(Player&);					//virtual method
};

#endif
