#ifndef __TILE_H__
#define __TILE_H__

#include "floor.h"
#include "event.h"
#include "character.h"

//class Player;
//class Character;

extern Vec direction[8];

class Tile{
	char displayChar;
	public:
	Tile();
	Tile(char);
	virtual ~Tile(); //always virtual
	char getDisplay();
	virtual bool canPass(const Player&);
	virtual bool canPass(const Mob&);
	//virtual bool canPass(const Player&); //default chamber tile., yes
	//virtual bool canPass(const Mob&); //yes
	//virtual bool canSpawn(); //yes
};

class Wall: public Tile{
	//place holder kinda thing
	public:
	Wall(char);
	~Wall();
	//bool canPass(const Player&);
	//using Tile::canPass;
	bool canPass(const Player&); //cant pass
	bool canPass(const Mob&); //cant pass
	//bool canSpawn(); //no
};

class Path: public Tile{
	public:
	Path();
	~Path();
	using Tile::canPass;
	//bool canPass(const Player&); //yes
	bool canPass(const Mob&); //no, they can't pass the door anyway
	bool canPass(const Player&); //yes	
	//bool canSpawn();//no
};

class Door: public Tile{
	public:
	Door();
	~Door();
	using Tile::canPass;
	bool canPass(const Player&); //true
	bool canPass(const Mob&); //false
	//bool canSpawn(); //no
};

//was a Tile
/*
class Stair: public Event{
	public:
	Stair();
	bool canPass(const Player&); //true
	bool canPass(const Mob&); //false
	bool specialEvent(); //go to next floor
};
*/
#endif
