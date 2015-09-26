#ifndef __GOLD_H__
#define __GOLD_H__
#include "item.h"

class Character;

class Gold: public Item {
	int value;							//stores gold value
  public:
	Gold(Floor* f,int value=2);				//regular gold value is 2
	~Gold();
	bool canPass(const Mob&);				//default is false
	bool canPass(const Player&);			//Player can pass
	bool canUse();							//default is false
	bool specialEvent(Player&);
	int getValue();							//get gold amount
};

#endif
