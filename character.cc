#include "character.h"
#include "floor.h"
#include <cmath>
#include "rng.h"
#include <string>
#include "stringconverter.h"

using namespace std;

Character::Character(int hp,int atk,int def,int gold,int missChance, int x,int y,char c,Floor* f):
	Event(c,f),hp(hp),atk(atk),def(def),gold(gold),missChance(missChance){
		xpos=x;
		ypos=y;
		maxHp=hp;
}						//Constructor

//this is PC only
//Character::makeMove(int direction){}

Character::~Character(){}				//destructor

bool Character::hitEnemy(Event& e){		
	//cout<<"hitting an event"<<endl;	
	return false;
}

bool Character::hitEnemy(Character& enemy){
	//cout<<"imhere"<<endl;
	int atkTimes=1+spAbility["extra_atk"]; //Halfing atk twice
	while(atkTimes--){
		int missRate=missChance; //the miss rate
		if (enemy.getSP("evasion")){
			missRate+=enemy.getSP("evasion");
			//logs.push_back(getDisplay()+" has a "+toStr(spAbility["evasion"])+"% evasion");
		}
		if (isSuccess(100-missRate,100)){
			//cout<<"id miss"<<endl;
			//hp+= sp["lifesteal"];
			//dmg=dmg+sp["extradmg"]/100.0*dmg
			int dmg=ceil((100.0/(100.0+enemy.getDef()))*getAtk());				//combat dmg calculation formula

			if (isSuccess(spAbility["crit"],100)){
				dmg*=2;															//20% chance of landing a critical strike
				logs.push_back(getName()+" striked a critical attack, base damge increased by 100%.");
			}
			//50% dmg amplify for orcs
			dmg=ceil(dmg*((100+spAbility["dmg_amplify%"]))/100.0);
			dmg+=spAbility["dmg_inc"];
			if (spAbility["dmg_amplify%"]){
				logs.push_back(getName()+"'s damage is amplified by "+toStr(spAbility["dmg_amplify%"]));
			}
			//special attack for Troll
			if (spAbility["dmg_inc"]){
				logs.push_back(getName()+"'s damage is increased by "+toStr(spAbility["dmg_inc"]));
			}
			//not used
			int dmgRed=enemy.getSP("dmg_reduction%");
			if (dmgRed){
				dmg=ceil(dmg*((100-dmgRed)/100.0));
				logs.push_back(enemy.getName()+"has reduced the damage by "+ toStr(dmgRed)+"%.");
			}
			//not used
			int dmgDec=enemy.getSP("dmg_dec");
			if (dmgDec){
				if (dmg-dmgDec<0)
					dmgDec=dmg;
				logs.push_back(enemy.getName()+"has decreased the damage by "+toStr(dmgDec));
				dmg=dmg-dmgDec;
			}
			enemy.takeDmg(dmg);

			logs.push_back(getName()+" dealt "+toStr(dmg)+" dmg to "+enemy.getName()+" ("+toStr(enemy.getHp())+").");	
			int lifesteal=spAbility["lifesteal"];
			lifesteal+=floor((spAbility["lifesteal%"]/100.0)*dmg);
			//lifesteal from Vampire
			if (lifesteal){
				hp+=lifesteal;
				logs.push_back(getName()+" has gained "+ toStr(lifesteal)+" hp from lifesteal.");
			}
		}else{
			logs.push_back(getName()+" tried to attack "+enemy.getName()+" but missed.");
		}

	}
	/*
	if (spAbility["regenerate_hp"]){
		int regen=spAbility["regenerate_hp"];
		regen=addHp(regen);
		if (regen){
			logs.push_back(getName()+" has regenerated "+toStr(regen)+" HP.");
		}
	}*/


	return true;
}

//default behaviour for getHit
bool Character::getHitBy(Character& c){
	return c.hitEnemy(*this);
}

int Character::getGoldDrop(){
	//return gold+spAbility["extra_gold_drop"]*gold;
	return 0;
}

//return the damage taken, limiting it to 0 hp
int Character::takeDmg(int dmg){
	hp-=dmg;
	if (hp<0){
		dmg+=hp;
		hp=0;
	}
	return dmg;
}

bool Character::isDead(){
	return hp<=0;
}
int Character::getHp(){
	return hp;
}

int Character::getAtk(){
	return atk;
}

int Character::getDef(){
	return def;
}

int Character::getSP(string sp){
	return spAbility[sp];
}

int Character::getX(){
	return xpos;
}
int Character::getY(){
	return ypos;
}
void Character::setXY(int x,int y){
	xpos=x;ypos=y;
}
bool Character::isMobile(){
	return true;
}

//make sure it doesnt go over max hp'
int Character::addHp(int amt){
	
	if (spAbility["no_maxlife"]==0){
		if (amt+hp>maxHp)
			amt=maxHp-hp;
	}
	hp+=amt;
	return amt;
}

string Character::getName(){
	return toStr(getDisplay());
}
