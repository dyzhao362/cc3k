#include "potion.h"
#include "player.h"

using namespace std;

Potion::Potion(Floor* f,int value):Item('P',f),value(value) {}			//Constructor

bool Potion::specialEvent(Player& c) {
	return false;
}

bool Potion::canUse(){								//true for all potions
	return true;
}


int Potion::getVal() {
	return value;
}
