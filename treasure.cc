#include "treasure.h"
#include "character.h"
#include <iostream>
#include "player.h"
#include "floor.h"

using namespace std;

Treasure::Treasure(Floor* f,int x,int y,int value): Item('G',f),value(value),canOpen(false) {		//constructor
	loc.setXY(x,y);
}

Treasure::~Treasure(){}			//destructor

bool Treasure::canPass(const Mob& m) {
	return false;
}

bool Treasure::canPass(const Player& p) {				//player character can pass after dragin is slain
	return canOpen;
}

bool Treasure::canUse() {
	return false;
}
//get the gold
bool Treasure::specialEvent(Player& p) {					
	p.getGold(getValue());
	return true;
}

//not used, dragon is a friend of Treasure, access private field directly
void Treasure::setCanPass() {
}

int Treasure::getValue() {
	return value;
}
Vec Treasure::getLoc(){
	return loc;
}
