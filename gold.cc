#include "gold.h"
#include "character.h"
#include <iostream>
#include "player.h"

using namespace std;

Gold::Gold(Floor* f,int value): Item('G',f),value(value) {}			//constructor

Gold::~Gold(){}							//destructor

bool Gold::canPass(const Mob& m) {
	return false;
}

bool Gold::canPass(const Player& p) {			//Player character can pass
	return true;
}

bool Gold::canUse() {
	return false;
}

bool Gold::specialEvent(Player& p) {			//Event of picking of gold pile
	p.getGold(getValue());
	return true;
}

int Gold::getValue() {								//Get gold amount
	return value;
}			
