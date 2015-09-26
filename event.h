#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>
#include "floor.h"

//class Player;
class Character;

extern Vec direction[8];
extern std::vector<std::string> logs;

class Event{
	char displayChar;
protected:
	Floor* theFloor;
	public:
	Event(char,Floor*);				//constructor
	virtual ~Event()=0;					//desctructor
	char getDisplay();
	virtual bool isMobile(); //default not mob
	virtual bool specialEvent(Player&); //doing nothing by default
	virtual bool canPass(const Mob&); //no, by default, only Gold can
	virtual bool canPass(const Player&);
	virtual bool canAtk(const Event&); //false, default
	//virtual bool isPC(); //default false
	virtual bool canUse(); //default false, potions should override this
	virtual bool getHitBy(Character&);
};

class Stair:public Event{
	public:
		Stair(Floor*);
		~Stair();
	bool specialEvent(Player&);
	bool canPass(const Player&); //yes
};

//to be implemented
/*
class Character: public Event{
	protected:
	std::Map<std::string,int> spAbilities; //default value will be false
	int atk;
	int def;
	int hp;
	int gold;
	int missChance; //50 for mobs
	Vec position;
	//Floor* fl; //the floor the character is on
	std::string generalGetHit(Character&);
	bool mobile; //dragons are not mobile

	
	//default Non-zero spAbilities
	//droprate?
	//dmg_per_amp?

	public:
	virtual Character(int hp,int atk,int def,int gold,int missChance,char c);
	virtual bool isPC()=0;
	virtual void makeMove(int direction);
	virtual void makeMove()=0; //dragons dont make move
	virtual std::string hitEnemy(Character&); //consider all the special effects
	virtual std::string getHitBy(Character&); //some class specific effects
	int getGoldDrop();
	bool isDead();
	virtual ~Character(); //

};

class Mob: public Character{
	void makeMove(); //try to find a place to move
	std::string
	public:
		Mob(int hp,int atk,int def,int gold,int missChance,char c);
};

//drop 2x normal gold
class Human: public Mob{
	public:
		void makeMove(); //generate a possible move
		Human(); //call Character(..), then set spAbilities[droprate]=2

};

//allergy
class Dwarf: public Mob{
	public:
		Dwarf(); //call Character(..),
};
class Orcs: public Mob{
	public:
		Orcs();
	std::string hitEnemy(Goblins&);
	//sp[dmg_perc_amp] = 50 
	//sp[dmg_perc_amp] = 0
};

class Elf: public Mob{
	public:
		Elf(); //sp[addtional_atk]=1
	std::string hitEnemy(Drow&);
	//set additional_atk=0
};

class Merchant: public Mob{
	static bool neutral; //set to true, once atked by player set to false
};

class Dragon: public Mob{
	Gold* treasure;
	public:
	Dragon(Gold&); 
	~Dragon(); //tell Gold that it is not being guarded
};

class Player: public Character{
	std::string className;
	int initialAtk;
	int initialDef;

	public:
	Player(int hp,int atk,int def,int gold,int missChance); 
	void resetStats();

};

class Vampire: public Player{
	public:
	Vampire();
	std::string hitEnemy(Dwarf&); 
	//lose lifesteal, sp[lifesteal]-=5, sp[self_dmg]+=5
	//after the getHitBy, regain sp[lifesteal]+=5, sp[self_dmg]-=5
};

class Drow: public Player{
	//sp[magnify_potion] 50
	Drow();
};

class Troll: public Player{
	//sp[hp_cap]=120, 0 means no cap
	//sp[hp_gain]=5
	Troll();

};

class Goblin: public Player{
	//sp[gold_steal]=5
	Goblin();
}

class Shade: public Player{
	Shade();
	//sp[goldpoint_mult]=1
}


class Item: public Event{
	public:
	Item();
	bool canAtk(Event&); //no
	virtual bool canPass(const Event&); //gold and pc -yes, other no
};
*/
#endif
