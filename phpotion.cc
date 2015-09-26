#include "phpotion.h"
#include <string>
#include "player.h"

using namespace std;

bool PHPotion::used = false;				//default is hidden from player

void PHPotion::reset(){						//reset potion used
	used=false;
}
PHPotion::PHPotion(Floor* f): Potion(f,10) {}				//PH potion value is 10
PHPotion::~PHPotion(){}

bool PHPotion::specialEvent(Player& p) {					//returns true if player uses the potion
	used = true;
	p.usePotion(*this);
	return true;
}

string PHPotion::getDescription() {
	if (used == true) {
		return "poison health potion, effect: Lose up to 10 HP!";				//displays poison effect
	}
	else if (used == false) {
		return "mysterious potion, unknown effect.";					//used=false, display unknown effect, mysterous potion
	}
}
