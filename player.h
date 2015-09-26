#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "event.h"
#include <vector>
#include <map>
#include <string>
#include "character.h"
#include "mob.h"
#include "item.h"
#include "potion.h"
#include "bapotion.h"
#include "bdpotion.h"
#include "phpotion.h"
#include "rhpotion.h"
#include "wapotion.h"
#include "wdpotion.h"

class Player: public Character{
	//std::string className;
	int exp;

	public:
	Player(int hp,int atk,int def,int gold,int missChance,int x,int y,Floor* f);
	virtual ~Player()=0;
	void resetStats();
	//bool isPC(); //yes
	bool makeMove(int dir);
	//virtual bool hitEnemy(Event&);
	//using Character::hitEnemy;
	void gainGold(int a);
	void getGold(int a);
	void gainExp(int a);
	int getScore();
	virtual bool hitEnemy(Character&);
	virtual bool useSkillOn(Mob&);
	virtual bool getHitBy(Character&);
	std::string getName();
	bool canStepOn(Tile&);
	bool canStepOn(Event&);
	//bool hasPotion;
	void usePotion(RHPotion&);
	void usePotion(BAPotion&);
	void usePotion(BDPotion&);
	void usePotion(PHPotion&);
	void usePotion(WAPotion&);
	void usePotion(WDPotion&);
	bool fly(int,int);							//bonus command fly
	bool hasMp(int);							//bonus feature mp

	friend std::ostream& operator<<(std::ostream& os,const Player& p);

	protected:
	int mp;
	int maxMp;
	void endOfTurn();
	virtual void levelUp();							//player can level up
	int abilityPara;
	int initialAtk;
	int initialDef;
	int level;
};

/*
   class Vampire: public Player{
   public:
   Vampire();
   bool hitEnemy(Dwarf&); 
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
*/

class Shade: public Player{
	public:
		Shade(int x,int y,Floor* f);
		~Shade();
		void levelUp();

		//sp[goldpoint_mult]=1
		//bool getHitBy(Character& e);
};

class Drow: public Player{
	public:
		Drow(int x,int y,Floor* f);
		~Drow();
		void levelUp();
};

class Vampire: public Player{
	public:
		Vampire(int x,int y,Floor* f);
		~Vampire();
		using Player::hitEnemy;
		bool hitEnemy(Character&);
		bool useSkillOn(Mob&);
		void levelUp();

		//bool hitEnemy(Dwarve&);
		//bool hitEnemy(Character&);
};

class Troll: public Player{
	public:
		Troll(int x,int y,Floor* f);
		bool useSkillOn(Mob&);
		void levelUp();
		~Troll();
};

class Goblin: public Player{
	public:
		Goblin(int x,int y,Floor* f);
		void levelUp();
		~Goblin();
};

#endif

