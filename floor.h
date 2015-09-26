#ifndef __FLOOR_H__
#define __FLOOR_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

const int MAXH=25;
const int MAXW=79;

class Tile;
class Event;
class Mob;
class Character;
class Player;
class Treasure;

class Vec{
	int x,y;
	public:
	Vec();
	Vec(int x,int y);
	int getX();
	int getY();
	void setXY(int,int);
	bool operator==(Vec&);
};


extern Vec direction[8];
extern std::vector<std::string> logs;

class Floor{
	//static
	//char display[MAXH][MAXW];
	Tile* tiles[MAXH][MAXW]; //tiles do not change
	Event* cells[MAXH][MAXW];			//cells in the chamber
	Mob* mobs[50];						
	const int maxMob;
	int mobNum;
	bool move; //are the mobs going to move
	bool endGame;
	Player* PC;
	Vec playerLoc;							//store the coordinates of the PC
	std::vector<Vec> chamber[5]; //store the coordinates of the tiles in the chamber
	std::vector<Vec> remains[5]; //store whatever is left
	//Square* chamberCoords[5]; //the boundary of the chamber
	int vis[MAXH][MAXW];
	void setupChamber(); //read in the tiles in chamber
	void addToChamber(int x,int y,int idx,int init);
	void spawnMobAt(int x,int y,char m,Treasure *t=NULL);			//Generate Mobs coordinates
	void matchDragonGold();
	int level;
	std::ifstream fin;
	bool readFromFile;
	void generateMobs();									//random generation of mobs
	void generatePotion();								//random generation of potions
	void generateGold();								//random generation of gold
	void generatePCStair();								//random generation of player character location and stair location
	//void spawnItemAt(int x,int y,Item*);
	//bool cmpMobPtr(Mob*,Mob*);

	public:
	Floor();
	~Floor();
	void initFloor();
	void clearFloor();
	void resetFloor();
	void floorRespawn();
	void readFloorSetting(std::ifstream&); //for initialization
	Event* getEvent(int,int); //get the event on r c
	void moveEvent(int i,int j,int x,int y); //move Event from ij to xy
	//bool canStepOn(int x,int y,Character&);
	template <class T> bool canStepOn(int x,int y,T&);
	void setPC(Player*);
	void moveCharacter(Player& p,int x,int y);
	void moveCharacter(Mob& m,int x,int y);
	void mobsInAction();
	bool isValidEventOnFloor(int x,int y);
	bool atkThis(int,int,bool skill=false);
	bool useItem(int,int);
	void removeDead();
	Vec getPlayerLoc();
	void goUpStairs();
	Vec collectCorpse(int,int);
	void setFin(std::string);
	bool reachedEnd();
	int getLevel();

	friend std::ostream& operator<<(std::ostream &os,const Floor& f);
};


#endif
