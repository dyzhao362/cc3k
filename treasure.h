#ifndef __TREASURE_H__
#define __TREASURE_H__
#include "item.h"

class Vec;
class Dragon;

class Treasure: public Item {
	friend class Dragon;	
	int value;
	bool canOpen;			//bool default false
	Vec loc;
  public:
	Treasure(Floor* f,int x,int y,int value=6);				//dragon treasure default value 6
	~Treasure();
	bool canPass(const Mob&);						//default false
	bool canPass(const Player&);						
	bool canUse();						//false
	bool specialEvent(Player&);
	void setCanPass();
	int getValue();
	Vec getLoc();
};

#endif
