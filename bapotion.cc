#include "bapotion.h"
#include <string>
#include "player.h"

using namespace std;

bool BAPotion::used = false;			//default is hidden from player

void BAPotion::reset(){					//reset potion used
	used=false;
}

BAPotion::BAPotion(Floor* f):Potion(f,5) {}			//BA potion value is 5

BAPotion::~BAPotion(){}

bool BAPotion::specialEvent(Player&p) {				//returns true if player drinks the potion
	used = true;					
	p.usePotion(*this);
	return true;
}

string BAPotion::getDescription() {
	if (used == true) {
		return "boost atk potion, effect: Increase ATK by 5!";				//display potion effect
	}
	else if (used == false) {												//if used = false, display unknown effect, mysterious potion
		return "mysterious Potion, unknown effect.";
	}
}
