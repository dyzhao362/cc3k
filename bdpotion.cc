#include "bdpotion.h"
#include <string>
#include "player.h"

using namespace std;

bool BDPotion::used = false;		//default is hidden from player

BDPotion::BDPotion(Floor* f): Potion(f,5) {}		//BD potion value is 5
BDPotion::~BDPotion(){}

bool BDPotion::specialEvent(Player& p) {			//returns true if player uses the potion
	used = true;
	p.usePotion(*this);
	return true;
}

void BDPotion::reset(){					//reset potion used
	used=false;
}
string BDPotion::getDescription() {
	if (used == true) {
		return "boost def potion, effect: Increase DEF by 5!";				//displays potion effect
	}
	else if (used == false) {
		return "mysterious potion, unknown effect.";					//if used=false, displays unknown effect, mysterious potion
	}
}
