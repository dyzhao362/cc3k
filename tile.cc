#include "tile.h"

Tile::Tile():displayChar('.'){}
Tile::Tile(char displayChar):displayChar(displayChar){}

Tile::~Tile(){} //empty implementation

char Tile::getDisplay(){
	return displayChar;
}

bool Tile::canPass(const Mob&){
	return true;
}
bool Tile::canPass(const Player&){
	return true;
}

//bool Tile::canSpawn(){
//	return true;
//}

Wall::Wall(char wallType):Tile(wallType){}
bool Wall::canPass(const Player&){ //cant pass the a wall
	return false;
}
bool Wall::canPass(const Mob&){
	return false;
}
Wall::~Wall(){}

Path::Path():Tile('#'){}
Path::~Path(){}
/*
Path::canPass(const Player&){
	return true;
}
*/
bool Path::canPass(const Mob&){
	return false;
}
bool Path::canPass(const Player&){
	return true;
}


Door::Door():Tile('+'){}
Door::~Door(){}

bool Door::canPass(const Player&){
	return true;
}

bool Door::canPass(const Mob&){
	return false;
}


