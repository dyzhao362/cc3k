#include "wdpotion.h"
#include <string>
#include "player.h"

using namespace std;

bool WDPotion::used = false;				//default is hidden from player

WDPotion::WDPotion(Floor* f): Potion(f,5) {}		//WD Potion value is 5
WDPotion::~WDPotion(){}
bool WDPotion::specialEvent(Player& p) {			//returns true if player uses the potion
	used = true;
	p.usePotion(*this);
	return true;
}

void WDPotion::reset(){						//reset potion used
	used=false;
}
string WDPotion::getDescription() {
	if (used == true) {
		return "wound def potion, effect: Decrease DEF by 5!";					//display potion effect
	}
	else if (used == false) {
		return "mysterious potion, unknown effect.";						//used=false, display unknown effect, mysterious potion
	}
}
