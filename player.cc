#include "character.h"
#include "floor.h"
#include <cmath>
#include "rng.h"
#include <string>
#include "stringconverter.h"
#include "player.h"

using namespace std;

Player::Player(int hp,int atk,int def,int gold,int missChance, int x,int y,Floor* f):			//constructor
	Character(hp,atk,def,gold,missChance,x,y,'@',f){
	initialAtk=atk;
	initialDef=def;
	exp=0;
	mp=0;
	maxMp=10;
	abilityPara=0;
	level=1;
	spAbility["crit"]=20;
}

Player::~Player(){}

void Player::resetStats(){						//function to reset stats everytime in a new floor because effect of temporary potions
	atk=initialAtk;
	def=initialDef;
}
/*
bool Player::isPC(){
	return true;
}
*/
bool Player::makeMove(int dir){
	int newx=xpos+direction[dir].getX();
	int newy=ypos+direction[dir].getY();
	 if (theFloor->canStepOn(newx,newy,*this)){
		 theFloor->moveCharacter(*this,newx,newy);
		//xpos=newx;
		//ypos=newy;
		endOfTurn();
		return true;
	 }
	return false;
}

void Player::endOfTurn(){
	int hpInc=spAbility["regenerate_hp"];							//troll's ability
	if (hpInc){
		hpInc=addHp(hpInc);
		if (hpInc)
			logs.push_back(getName()+" has regenerated "+toStr(hpInc)+" hp.");
	}
}


bool Player::hitEnemy(Character& e){
	//add special effects
	bool r=Character::hitEnemy(e);
	mp+=1;
	if (mp>maxMp)
		mp=maxMp;
	endOfTurn();
	return r;
}

bool Player::useSkillOn(Mob& e){
	if (mp<5){
		return false;
	}
	mp-=5;
	int dmg=getAtk();
	dmg=e.takeDmg(dmg); //default skill
	//vampire get 50% lifesteal
	//goblin get 5 goldsteal
	//troll get +flat 10 damage 
	//human drow get default effect
	logs.push_back(getName()+" used skill on "+e.getName()+", dealt "+toStr(dmg)+" damage.");
	endOfTurn();
	return true;
}

bool Player::fly(int fx,int fy){
	if (mp<7)
		return false;
	if (theFloor->canStepOn(fx,fy,*this)){
		theFloor->moveCharacter(*this,fx,fy);
		mp-=7;
		endOfTurn();
		return true;
	}
	return false;
}

bool Player::hasMp(int x){
	return mp>=x;
}

bool Player::getHitBy(Character& e){
	return e.hitEnemy(*this);
}

string Player::getName(){
	return "PC";
}

void Player::gainGold(int amt){
	amt+=spAbility["gold_steal"];
	gold+=amt;
	if (amt)
		logs.push_back(string("You received ")+toStr(amt)+" gold.");
}
void Player::getGold(int amt){
	gold+=amt;
	logs.push_back(string("You picked up ")+toStr(amt)+" gold.");

}

bool Player::canStepOn(Tile& t){
	return t.canPass(*this);
}

bool Player::canStepOn(Event& e){
	return e.canPass(*this);
}

void Player::gainExp(int amt){
	//cout<<"Iamgained"<<endl;
	exp+=amt;
#ifdef DLC
	//exp+=99;
	logs.push_back(string("You have gained ")+toStr(amt)+" exp.");
	if (exp>=50+level*50){
		exp-=(50+level*50);
		//level up
		levelUp();
		level++;
	}
#endif
}

void Player::levelUp(){
	logs.push_back("You have leveled up.");
	logs.push_back("You gained 1 atk, 1 def, and regained max life.");
	atk+=1;
	initialAtk+=1;
	def+=1;
	initialDef+=1;
	hp=maxHp;
}

void Player::usePotion(RHPotion &p){							//RH potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	amt=addHp(amt);
	logs.push_back(string("You have restored ")+toStr(amt)+" hp.");
}

void Player::usePotion(BAPotion &p){							//BA potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	atk+=amt;
	logs.push_back(string("Your attack has been increased by ")+toStr(amt)+".");
}

void Player::usePotion(BDPotion &p){								//BD potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	def+=amt;
	logs.push_back(string("Your defense has been increased by ")+toStr(amt)+".");
}

void Player::usePotion(PHPotion &p){								//PH potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	amt=takeDmg(amt);
	logs.push_back(string("Poison health decreased your hp by ")+toStr(amt)+".");
}

void Player::usePotion(WAPotion &p){									//WA potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	atk-=amt;
	if (atk<0){
		amt+=atk;
		atk=0;
	}
	logs.push_back(string("Wound Atk decreased your attack by ")+toStr(amt)+".");
}

void Player::usePotion(WDPotion &p){									//WD potion effect
	int amt=p.getVal();
	amt=ceil(amt*((100+spAbility["potion_amplify"])/100.0));
	def-=amt;
	if (def<0){
		amt+=def;
		def=0;
	}
	logs.push_back(string("Wound Def decreased your defense by ")+toStr(amt)+".");
}

int Player::getScore(){														//calculates player score
	return floor(gold*((spAbility["score_multiplier"]+100)/100.0));
}		

ostream& operator<<(ostream& os,const Player& p){
	os<<"HP: "<<p.hp;
#ifdef DLC
	os<<" MP: "<<p.mp<<"/"<<p.maxMp;
#endif
	os<<" ATK: "<<p.atk<<" DEF: "<<p.def;
	os<<" Gold: "<<p.gold;
#ifdef DLC
	os<<" Exp: "<<p.exp<<"/"<<50+p.level*50;
	os<<" Level: "<<p.level;
#endif
	os<<" Floor: "<<p.theFloor->getLevel();
	os<<endl;
	return os;
}

Shade::Shade(int x,int y,Floor* f):Player(125,25,25,0,0,x,y,f){
	spAbility["score_multiplier"]=50;							//shade class, has gold mutiplier of 1.5
}

/*
bool Shade::getHitBy(Character& e){
	return e.hitEnemy(*this);
}
*/
void Shade::levelUp(){
	atk+=2;
	def+=2;
	int regHp=20;
	regHp=addHp(regHp);
	logs.push_back("You have leveled up.");
	logs.push_back("You gained 2 atk, 2 def, and regenerated "+toStr(regHp)+ " hp.");

}


Shade::~Shade(){}


Drow::Drow(int x,int y,Floor* f):Player(150,25,15,0,0,x,y,f){
	abilityPara=50;
	spAbility["potion_amplify"]=abilityPara;							//drow class, all potions effect magnified by 1.5
	
}
void Drow::levelUp(){
	atk+=2;
	def+=1;
	initialAtk+=2;
	initialDef+=1;
	hp=maxHp;
	logs.push_back("You have leveled up.");
	logs.push_back("You gained 2 atk, 1 def, and regained max hp.");
}


Drow::~Drow(){}

Vampire::Vampire(int x,int y,Floor* f):Player(50,25,25,0,0,x,y,f){
	abilityPara=5;
	spAbility["lifesteal"]=abilityPara;									//vampire class, gains 5HP every attack and no max HP
	spAbility["no_maxlife"]=1;
	maxHp=65536; //just to makesure
}
void Vampire::levelUp(){
	atk+=1;
	def+=1;
	initialAtk+=1;
	initialDef+=1;
	abilityPara+=1;
	hp+=50;
	logs.push_back("You have leveled up.");
	logs.push_back("You gained 1 atk, 1 def, 1 lifesteal, 50 hp.");
}

/*
bool Vampire::hitEnemy(Dwarve &d){
	cout<<"need to call this"<<endl;
	spAbility["lifesteal"]=-5;
	bool r=Character::hitEnemy(d);
	spAbility["lifesteal"]=5;
	return r;
}
*/

bool Vampire::hitEnemy(Character& d){
	bool r;
	Dwarve* dptr=dynamic_cast<Dwarve*> (&d);
	
	if (dptr){ //its a dwarve
		spAbility["lifesteal"]=-5;
		r=Player::hitEnemy(d);
		spAbility["lifesteal"]=5;	
	}
	else{
		r=Player::hitEnemy(d);
	}
	return r;
}


bool Vampire::useSkillOn(Mob& e){
	if (mp<5){
		return false;
	}
	mp-=5;
	logs.push_back(getName()+" has gained 50% lifesteal from spell usage");
	spAbility["lifesteal%"]+=50;
	Player::hitEnemy(e);
	spAbility["lifesteal%"]-=50;
	return true;
}

Vampire::~Vampire(){}



Troll::Troll(int x,int y,Floor* f):Player(120,25,15,0,0,x,y,f){
	abilityPara=5;
	spAbility["regenerate_hp"]=abilityPara;							//troll class, regains 5HP every turn
	spAbility["crit"]=0;
}

void Troll::levelUp(){
	atk+=1;
	def+=1;
	initialAtk+=1;
	initialDef+=1;
	abilityPara+=1;
	maxHp+=5;
	hp+=5;
	logs.push_back("You have leveled up");
	logs.push_back("You gained 1 atk, 1 def, 1 life regeneration and 5 max hp");
}



Troll::~Troll(){}

bool Troll::useSkillOn(Mob& e){
	if (mp<5){
		return false;
	}
	mp-=5;
	logs.push_back(getName()+" has gained 10 flat damage increase");
	spAbility["dmg_inc"]+=10;
	Player::hitEnemy(e);
	spAbility["dmg_inc"]-=10;
	return true;
}


Goblin::Goblin(int x,int y,Floor* f):Player(110,15,20,0,0,x,y,f){
	abilityPara=5;
	spAbility["gold_steal"]=abilityPara;						//goblin class, steals 5 gold from every slain enemy.
}
void Goblin::levelUp(){
	atk+=1;
	def+=1;
	initialAtk+=1;
	initialDef+=1;
	abilityPara+=1;
	hp=maxHp;
	logs.push_back("You have leveled up");
	logs.push_back("You gained 1 atk, 1 def, 1 gold steal, and regained max hp");
}

Goblin::~Goblin(){}
