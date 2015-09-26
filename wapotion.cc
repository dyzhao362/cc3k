#include "wapotion.h"
#include <string>
#include "player.h"

using namespace std;

bool WAPotion::used = false;				//default is hidden from player

WAPotion::WAPotion(Floor* f): Potion(f,5) {}			//WA potion value is 10
WAPotion::~WAPotion(){}

bool WAPotion::specialEvent(Player& p) {				//returns true if player uses the potion
	used = true;
	p.usePotion(*this);
	return true;
}

void WAPotion::reset(){								//reset potion used
	used=false;
}
string WAPotion::getDescription() {
	if (used == true) {
		return "wound atk potion, effect: Decrease ATK by 5!";				//display potion effect
	}
	else if (used == false) {
		return "mysterious potion found, unknown effect.";				//used =false, displays unknown effect, mysterious potion
	}
}
