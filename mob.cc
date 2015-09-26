#include "character.h"
#include "floor.h"
#include <cmath>
#include "rng.h"
#include <string>
#include "stringconverter.h"
#include "mob.h"
#include "player.h"
#include <iostream>
#include "treasure.h"

using namespace std;

Mob::Mob(int hp,int atk,int def,int gold,int exp,int missChance,int x,int y,char c,Floor* f):Character(hp,atk,def,gold,missChance,x,y,c,f),exp(exp){

}

Mob::~Mob(){}

int Mob::getGoldDrop(){
	if (gold==-1){
		return randInt(1,2);
	}
	return gold;
}

//merchants by default has no aggro
bool Mob::hasAggro(){
	return true;
}


bool Mob::hitEnemy(Character& e){
	return Character::hitEnemy(e);
}

bool Mob::getHitBy(Character& e){
	return e.hitEnemy(*this);
}
/*
bool Mob::isPC(){
	return false;
}
*/
void Mob::collectCorpse(){
//do nothing, dragon override this
}

bool Mob::canStepOn(Tile& t){
	return t.canPass(*this);
}
bool Mob::canStepOn(Event& e){
	return e.canPass(*this);
}
int Mob::getExp(){
	return exp;
}

Human::Human(int x,int y,Floor* f):
	Mob(140,20,20,4,30,50,x,y,'H',f){
	spAbility["extra_gold_drop"]=1;
}

Human::~Human(){}

Dwarve::Dwarve(int x,int y,Floor* f):
	Mob(100,20,30,-1,22,50,x,y,'W',f){}

Dwarve::~Dwarve(){}

//needs to dispatch type for special behaviour 
bool Dwarve::getHitBy(Character& e){
	//cout<<"iambeing visited"<<endl;
	return e.hitEnemy(*this);
}

bool Merchant::aggro=false;

Merchant::Merchant(int x,int y,Floor* f):
	Mob(30,70,5,4,24,50,x,y,'M',f){
}
bool Merchant::hasAggro(){
	return aggro;
}
//when it gets hit, it has aggro to the player
bool Merchant::getHitBy(Character& e){
	aggro=true;
	//cout<<"whats wrong"<<endl;
	return e.hitEnemy(*this);
}
void Merchant::reset(){
	aggro=false;
}
Merchant::~Merchant(){}

//-1 in gold denote 1 or 2
Halfling::Halfling(int x,int y,Floor* f):
	Mob(100,15,20,-1,30,50,x,y,'L',f){
	spAbility["evasion"]=50;				//Halfling can cause player's attack to miss (50% chance)
}
Halfling::~Halfling(){}

Elf::Elf(int x,int y,Floor* f):
	Mob(140,30,10,-1,28,50,x,y,'E',f){
	spAbility["extra_atk"]=1;						//Elf gets an extra attack against every race except for drow
}
Elf::~Elf(){}
bool Elf::hitEnemy(Character& e){
	bool r;
	Drow* dptr=dynamic_cast<Drow*> (&e);
	
	if (dptr){ //its a drow
		spAbility["extra_atk"]=0;//lose its ability to atk twice
		r=Character::hitEnemy(e); 
		spAbility["extra_atk"]=1;	
	}
	else{ //default behaviour
		r=Character::hitEnemy(e);
	}
	return r;
}

Orc::Orc(int x,int y,Floor* f):
	Mob(180,30,25,-1,24,50,x,y,'O',f){}

bool Orc::hitEnemy(Character& e){
	bool r;
	Goblin* gptr=dynamic_cast<Goblin*> (&e);		//does 50% more dmg to goblins
	
	if (gptr){ //its a drow
		spAbility["dmg_amplify%"]=50;//gain extra atk damage
		r=Character::hitEnemy(e); 
		spAbility["dmg_amplify%"]=0;
	}
	else{
		r=Character::hitEnemy(e);
	}
	return r;
}

Orc::~Orc(){}

//0 in gold means does not show any message
//NULL for now
//change event to gold etc`
Dragon::Dragon(int x,int y,Floor* f,Treasure* t):
	Mob(150,20,20,0,18,50,x,y,'D',f),treasure(t){}

void Dragon::collectCorpse(){
	//g.release(); //
	treasure->canOpen=true;
}

bool Dragon::hasTreasure(){
	if (treasure==NULL)
		return false;
	return true;
}

void Dragon::setTreasure(Treasure *t){
	treasure=t;
}


Vec Dragon::getTreasureLoc(){
	if (treasure==NULL)
		return Vec(-1,-1);
	return treasure->getLoc();
}

Dragon::~Dragon(){}

Centaur::Centaur(int x,int y,Floor* f):
	Mob(130,18,18,2,32,0,x,y,'C',f){}

Centaur::~Centaur(){}


Robot::Robot(int x,int y,Floor* f):
	Mob(70,20,30,6,5,50,x,y,'R',f){}

Robot::~Robot(){}


