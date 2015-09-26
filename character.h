#ifndef __CHARACTER_H__
#define __CHARACTER_H__
//#include "floor.h"
#include "event.h"
#include <vector>
#include <map>
#include <string>
#include "tile.h"

class Floor;

extern Vec direction[8];
extern std::vector<std::string> logs;

class Character: public Event{
	protected:
	std::map<std::string,int> spAbility; //default value will be false
	int hp;
	int atk;
	int def;
	int gold;
	int missChance; //50 for mobs
	int xpos;
	int ypos;
	int maxHp;
	int addHp(int hp);
	//bool mobile; //dragons are not mobile
	//default Non-zero spAbilities
	//droprate?
	//dmg_per_amp?

	public:
 	Character(int hp,int atk,int def,int gold,int missChance,int x,int y,char c,Floor* f);			//constructor
	virtual ~Character()=0;																			//virtual destructor
	//virtual bool isPC()=0;
	//virtual void makeMove(int direction);
	//virtual void makeMove(); //dragons dont make move
	virtual bool hitEnemy(Event&); //consider all the special effects
	virtual bool hitEnemy(Character&);
	virtual bool getHitBy(Character&); //some class specific effects
 	int getGoldDrop();
	bool isDead();
	int getHp();
	int getAtk();
	int getDef();
	int getSP(std::string);
	int getX();
	int getY();
	int takeDmg(int);
	void setXY(int,int);
	virtual bool isMobile(); //dragons are not mobile
	virtual std::string getName();
	virtual bool canStepOn(Tile&)=0;
	virtual bool canStepOn(Event&)=0;
	//virtual ~Character(); //

};



/*
class Dragon: public Mob{
	
	public:
	Event* treasure; //gold
	bool isDragon();
};

*/


#endif
