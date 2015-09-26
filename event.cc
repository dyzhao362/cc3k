#include "event.h"
#include "character.h"
#include <iostream>

using namespace std;

Event::Event(char disp,Floor* f){				//constructor
	displayChar=disp;
	theFloor=f;
}

Event::~Event(){} //empty implementation


char Event::getDisplay(){
	return displayChar;
}

bool Event::isMobile(){							//false by default
	return false;
}

/*
bool Event::isPC(){
	return false;
}
*/

bool Event::canUse(){
	return false;
}

bool Event::getHitBy(Character &e){
	return e.hitEnemy(*this);
}


//for potions, call this
//upstair as well
bool Event::specialEvent(Player& p){
	//override this and do something
	return false; //return true iff there is actually an event
	//possible events are 1.stair 2.potion
}

bool Event::canPass(const Mob& e){
	return false;
}
bool Event::canPass(const Player& p){
	return false;
}

bool Event::canAtk(const Event& e){
	return false;

}

Stair::Stair(Floor* f):Event('\\',f){}						//Stair part of event, represented by '\\'

Stair::~Stair(){}

bool Stair::specialEvent(Player&){
	theFloor->goUpStairs();
	return true;
}

bool Stair::canPass(const Player&){ //yes
	return true;
}
