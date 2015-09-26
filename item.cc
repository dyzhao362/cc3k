#include "item.h"
#include "event.h"
#include "character.h"

using namespace std;

Item::Item(char disp, Floor* f):Event(disp,f){}			//constructor

Item::~Item(){}

bool Item::canUse() {
	return true;
}

bool Item::canPass(const Mob& e) {
	return false;
}

bool Item::canPass(const Player& p) {
	return false;
}
bool Item::specialEvent(Player& c) {
	return false;
}#include "item.h"
#include "event.h"
#include "character.h"

using namespace std;

Item::Item(char disp, Floor* f):Event(disp,f){}			//constructor

Item::~Item(){}

bool Item::canUse() {
	return true;
}

bool Item::canPass(const Mob& e) {
	return false;
}

bool Item::canPass(const Player& p) {
	return false;
}
bool Item::specialEvent(Player& c) {
	return false;
}
