#ifndef __MOB_H__
#define __MOB_H__

#include "event.h"
#include <vector>
#include <map>
#include <string>
#include "character.h"
#include "tile.h"

class Floor;

class Mob: public Character{
	//std::string name;
	int exp;
	public:
	Mob(int hp,int atk,int def,int gold,int exp,int missChance,int x,int y,char c,Floor* f);
	virtual ~Mob();
	using Character::hitEnemy;
	virtual bool hitEnemy(Character&); //consider all the special effects
	virtual bool getHitBy(Character&); //some class specific effects
	virtual int getGoldDrop();
	virtual void collectCorpse(); //dragons release gold
	virtual bool hasAggro();			//for merchant
	//bool isPC();
	bool canStepOn(Tile&);
	bool canStepOn(Event&);
	int getExp();
};

class Human: public Mob{
	public:
		Human(int x,int y,Floor* f);
		//int getGoldDrop();
		~Human();
};

class Dwarve: public Mob{
	public:
		Dwarve(int x,int y,Floor *f);
		~Dwarve();
		bool getHitBy(Character&);
};

class Merchant: public Mob{
	static bool aggro;
	public:
		Merchant(int x,int y,Floor *f);
		~Merchant();
		bool getHitBy(Character&);
		bool hasAggro();
		static void reset();
};

class Orc: public Mob{
	public:
		Orc(int x,int y,Floor *f);
		~Orc();
		bool hitEnemy(Character&);
};

class Elf: public Mob{
	public:
		Elf(int x,int y,Floor *f);
		~Elf();
		bool hitEnemy(Character&);
};

class Halfling: public Mob{
	public:
		Halfling(int x,int y,Floor *f);
		~Halfling();
};


class Dragon: public Mob{
	//should be gold here
	Treasure* treasure;
	public:
	Dragon(int x,int y,Floor *f,Treasure* t=NULL);
	~Dragon();
	void collectCorpse();
	bool hasTreasure();
	void setTreasure(Treasure*);
	Vec getTreasureLoc();

	//should be deleted
};


class Centaur: public Mob{
	public:
		Centaur(int x,int y,Floor *f);
		~Centaur();

};

class Robot: public Mob{
	public:
		Robot(int x,int y,Floor *f);
		~Robot();
};









#endif
