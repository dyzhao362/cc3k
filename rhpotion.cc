#include "rhpotion.h"
#include <string>
#include "player.h"

using namespace std;

bool RHPotion::used = false;		//default is hidden from player

RHPotion::RHPotion(Floor* f): Potion(f,10) {}				//RH potion value is 10
RHPotion::~RHPotion(){}


void RHPotion::reset(){							//reset potion used
	used=false;
}

bool RHPotion::specialEvent(Player& p) {				//returns true if player uses the potion
	used = true;
	p.usePotion(*this);
	return true;
}

string RHPotion::getDescription() {
	if (used == true) {
		return "restore health potion, effect: restore up to 10 HP!";		//displays potion effect
	}
	else if (used == false) {
		return "mysterious potion, unknown effect.";						//used = false, displays unknown effect, mysterious potion
	}
}

