#include "floor.h"
#include "event.h"
#include "tile.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "rng.h"
#include <algorithm>
#include "mob.h"
#include "player.h"
#include "character.h"
#include "item.h"
#include "potion.h"
#include "gold.h"
#include "treasure.h"
#include "bapotion.h"
#include "bdpotion.h"
#include "phpotion.h"
#include "rhpotion.h"
#include "wapotion.h"
#include "wdpotion.h"
#include <cmath>

using namespace std;


//vector is basically a pair
Vec::Vec(){}

Vec::Vec(int x,int y):x(x),y(y){}

void Vec::setXY(int x,int y){
	this->x=x;
	this->y=y;
}
int Vec::getX(){
	return x;
}
int Vec::getY(){
	return y;
}
//=operator for simple comparison
bool Vec::operator==(Vec &other){
	if (x==other.getX()&&y==other.getY())
		return true;
	return false;
}

//constructor for floor
Floor::Floor():maxMob(50){
	//resetFloor();
	initFloor();
	readFromFile=false;
}


//initialization of floor
void Floor::initFloor(){
//	ifstream fin("init.txt");
//	readFloorSetting(fin);
	resetFloor();
	//fin=NULL;
	PC=NULL;
	move=true; //mobs only do not move after going upstairs
	endGame=false;
	level=1;
}
/*
void Floor::initFloorZero(){
	mobNum=0;
	memset(cells,0,sizeof(cells));
	memset(mobs,0,sizeof(mobs));
	memset(vis,0,sizeof(vis));
}
*/

//set everything to zero (NULL)
void Floor::resetFloor(){
	mobNum=0;
	memset(cells,0,sizeof(cells));
	memset(mobs,0,sizeof(mobs));
	memset(vis,0,sizeof(vis));
}

//delete everything that is not PC
void Floor::clearFloor(){
	for (int i=0;i<MAXH;i++)
		for (int j=0;j<MAXW;j++){
			delete tiles[i][j];
			if (cells[i][j]!=NULL){
				if (cells[i][j]!=PC) //don't ever delete pc
					delete cells[i][j]; //pc should be deleted by now
			}
			tiles[i][j]=NULL;
			cells[i][j]=NULL;
		}

}

//go up a stair
void Floor::goUpStairs(){
	clearFloor();
	//dont reset PC here
	resetFloor(); //reset floor things
	PC->resetStats(); //set potion effects
	level++; //floor + 1
	if (level==6){
		endGame=true;
		return;
	}
	logs.push_back(string("You went up a floor."));	
	if (!readFromFile){ //read default setup and respawn
		ifstream init("init.txt");
		readFloorSetting(init);
		//spawnStuff
		floorRespawn();
	}
	else{ //read from given fstream
		readFloorSetting(fin);
	}
}

//destructor
Floor::~Floor(){
	clearFloor(); //clear floor and close fin if reading from file
	if (readFromFile){
		fin.close();
	}
}


//add floor tiles on to chamber[idx]
void Floor::addToChamber(int x,int y,int idx,int init){
	if (x<0||x>=MAXH||y<0||y>=MAXW||idx>=5)
		return;
	if (vis[x][y]) //not visited
		return;
	if (tiles[x][y]==NULL){
		cerr<<"tiles null"<<x<<" "<<y<<endl;
	}
	if (init&&tiles[x][y]->getDisplay()=='+'){ //starting from a door
		vis[x][y]=1; //visited
		for (int i=0;i<8;i++){ //try adding these tiles
			addToChamber(x+direction[i].getX(),y+direction[i].getY(),idx,0);
		}
		return;
	}
	if (tiles[x][y]->getDisplay()=='.'){
		vis[x][y]=1; //this is a valid chamber tile
		Vec coord(x,y);
		chamber[idx].push_back(coord); //use default copy constructor
		for (int i=0;i<8;i++){
			addToChamber(x+direction[i].getX(),y+direction[i].getY(),idx,0);
		}
		return;
	}
	vis[x][y]=1;
}

//figure out waht coordinates are in each chamber
void Floor::setupChamber(){
	int idx=0;
	for (int i=0;i<MAXH;i++){
		for (int j=0;j<MAXW;j++){
			//look for unvisited doors
			if (!vis[i][j]&&tiles[i][j]->getDisplay()=='+'){
				addToChamber(i,j,idx++,1); //recursion to find the tiles
			}
		}
	}
	/*
	for (int i=0;i<5;i++){
		for (int j=0;j<chamber[i].size();j++){
			cout<<chamber[i][j].getX()<<" "<<chamber[i][j].getY()<<endl;
		}
		cout<<endl;
	}
	*/
}

//set to read from file, and then read for the 1st floor
void Floor::setFin(string fileName){
	fin.open(fileName.c_str());
	readFromFile=true;
	readFloorSetting(fin);
}

//read setup and mobs
void Floor::readFloorSetting(ifstream& file){
	//ifstream fin(file.c_str());
	bool hasPlayer=false;
	for (int i=0;i<MAXH;i++){
		for (int j=0;j<MAXW;j++){
			char c=file.get(); 
			switch(c){
				case ' ':tiles[i][j]=new Wall(c);break;
				case '|':tiles[i][j]=new Wall(c);break;
				case '-':tiles[i][j]=new Wall(c);break;
				case '#':tiles[i][j]=new Path();break;
				case '.':tiles[i][j]=new Tile();break;
				case '+':tiles[i][j]=new Door();break;
				case '\\':tiles[i][j]=new Tile();
						 cells[i][j]=new Stair(this);break;
				case '0':cells[i][j]=new RHPotion(this);
						 tiles[i][j]=new Tile();break;
				case '1':cells[i][j]=new BAPotion(this);
						 tiles[i][j]=new Tile();break;
				case '2':cells[i][j]=new BDPotion(this);
						 tiles[i][j]=new Tile();break;
				case '3':cells[i][j]=new PHPotion(this);
						 tiles[i][j]=new Tile();break;
				case '4':cells[i][j]=new WAPotion(this);
						 tiles[i][j]=new Tile();break;
				case '5':cells[i][j]=new WDPotion(this);
						 tiles[i][j]=new Tile();break;
				case '6':cells[i][j]=new Gold(this,2);
						 tiles[i][j]=new Tile();break;
				case '7':cells[i][j]=new Gold(this,1);
						 tiles[i][j]=new Tile();break;
				case '8':cells[i][j]=new Gold(this,4);
						 tiles[i][j]=new Tile();break;
				case '9':cells[i][j]=new Treasure(this,i,j,6);
						 tiles[i][j]=new Tile();break;
				case 'H':case 'W':case 'D':case 'L':case 'E':case 'M':case 'O':case 'R':case 'C':
						 tiles[i][j]=new Tile();spawnMobAt(i,j,c);break;
				case '@':playerLoc.setXY(i,j);hasPlayer=true;break;
			}
			//display[i][j]=c;
		}
		file.get(); //read the newline char
	}
	if (hasPlayer){ //if a player appeared in the setup
		//to figure out which tile type the player is standing on
		int sideWallCount=0;
		int doorCount=0;
		int blankWallCount=0;
		int pathCount=0;
		int tileCount=0;

		//count the number of each type of cell around the player
		int playerX=playerLoc.getX();
		int playerY=playerLoc.getY();
		for (int i=0;i<8;i++){
			int nx=playerX+direction[i].getX();
			int ny=playerY+direction[i].getY();
			if (nx>=0&&nx<MAXH&&ny>=0&&ny<MAXW){
				char thisTile=tiles[nx][ny]->getDisplay();
				if (thisTile=='+')
					doorCount++;
				if (thisTile=='|'||thisTile=='-')
					sideWallCount++;
				if (thisTile=='#')
					pathCount++;
				if (thisTile=='.')
					tileCount++;
				if (thisTile==' ')
					blankWallCount++;
			}
		}
		/*
		   cout<<"side: "<<sideWallCount<<endl;
		   cout<<"door: "<<doorCount<<endl;
		   cout<<"blank: "<<blankWallCount<<endl;
		   cout<<"path: "<<pathCount<<endl;
		   cout<<"tile: "<<tileCount<<endl;
		   */
		//limitations no path between 2 doors => cant figure out its a door
		
		if (doorCount==1&&pathCount>=1) //1 door 1+ path
			tiles[playerX][playerY]=new Path();
		else if(sideWallCount>=2&&tileCount>=3&&pathCount>=1) //2+ wall 3+ tile 1+ path
			tiles[playerX][playerY]=new Door();
		else if(pathCount>=2||doorCount>=2) //2+ path 2+ door
			tiles[playerX][playerY]=new Path();
		else
			tiles[playerX][playerY]=new Tile();
		matchDragonGold();
	}
	setupChamber(); //finish the setup
}

//match dragon with its gold when reading from file
void Floor::matchDragonGold(){
	vector<Vec> treasures;
	for (int i=0;i<MAXH;i++){
		for (int j=0;j<MAXW;j++){
			Treasure *t=dynamic_cast<Treasure*> (cells[i][j]);
			if (t){ //read in all the location of treasures
				treasures.push_back(Vec(i,j));
			}
		}
	}
	//cout<<"lets go"<<endl;
	vector<Vec>::iterator it;
	for (it=treasures.begin();it!=treasures.end();it++){
		for (int i=0;i<8;i++){
			int curX=it->getX();
			int curY=it->getY();
			int nx=curX+direction[i].getX();
			int ny=curY+direction[i].getY();
			//look for dragons around each treasure
			if (nx>=0&&nx<MAXH&&ny>=0&&ny<MAXW){
				Dragon *d=dynamic_cast<Dragon*> (cells[nx][ny]); //place holder
				if (d&&!(d->hasTreasure())){ //if a dragon is found, and it does not have treasure yet
					//could have used static_cast
					Treasure *t=dynamic_cast<Treasure*> (cells[curX][curY]);
					if (t){
						d->setTreasure(t); //let dragon have its treasure
					}
					else{
						cerr<<"not possible"<<endl;
					}
					//cast it to a treasure pointer
					//cells[nx][ny]=new Dragon(nx,ny,this,cells[curX][curY]);
					break;
				}
			}
		}	
	}
}

//spawnMob at the given location
//third argument is optional (for dragon only, default NULL)
void Floor::spawnMobAt(int x,int y,char m,Treasure* t){
	Mob* newMob=NULL;
	//Gold* dragonGold;
	switch(m){
		case 'H':newMob=new Human(x,y,this);break;
		case 'W':newMob=new Dwarve(x,y,this);break;
		case 'D':newMob=new Dragon(x,y,this,t);break;
		case 'M':newMob=new Merchant(x,y,this);break;
		case 'L':newMob=new Halfling(x,y,this);break;
		case 'O':newMob=new Orc(x,y,this);break;
		case 'E':newMob=new Elf(x,y,this);break;
		case 'C':newMob=new Centaur(x,y,this);break;
		case 'R':newMob=new Robot(x,y,this);break;
	} //if its a valid character
	if (newMob!=NULL){
		cells[x][y]=newMob;
		mobs[mobNum++]=newMob;
	}
}

Event* Floor::getEvent(int x,int y){
	return cells[x][y];
};

//please call canStepOn first
void Floor::moveCharacter(Player& p,int x,int y){
	int oldLevel=level; //check for floor number
	if (cells[x][y]!=NULL){
		//cout<<"special event called"<<endl;
		if (cells[x][y]->specialEvent(p)){ //call special event
			if (oldLevel==level){ //dont delete if went up a floor
				delete cells[x][y]; //delete the event (gold/potion etc)
				cells[x][y]=NULL;
			}
		}

	}
	if (endGame){ //if game ended after calling the event
		return;
	}
	//if went up a floor dont do anything (to the mobs)
	if (oldLevel==level){ //did not go up a floor
		int curX=p.getX();
		int curY=p.getY();
		cells[x][y]=&p;
		cells[curX][curY]=NULL;
		p.setXY(x,y); //set to new destination

		//printing information for finding potion
		for (int i=0;i<8;i++){
			int nx=direction[i].getX()+x;
			int ny=direction[i].getY()+y;
			if (nx>=0&&nx<MAXH&&ny>=0&&ny<MAXW){
				Potion* p=dynamic_cast<Potion*> (cells[nx][ny]);
				if (p){
					logs.push_back(string("You found a ")+p->getDescription());
				}
			}
		}
	}
	else{
		//tell mobs to not move
		move=false;
		//playerLoc will be reset, goto that location
		int px=playerLoc.getX();
		int py=playerLoc.getY();
		p.setXY(px,py);
		cells[px][py]=&p;
	}
}

//move for mobs
void Floor::moveCharacter(Mob& m,int x,int y){
	int curX=m.getX();
	int curY=m.getY();
	cells[x][y]=&m;
	cells[curX][curY]=NULL;
	m.setXY(x,y);
}


//please call canStepOn before this function, this function not used for now
void Floor::moveEvent(int i,int j,int x, int y){
	if (cells[x][y]!=NULL){
		cerr<<"movement err"<<endl;
		return;
	}
	cells[x][y]=cells[i][j];
	cells[i][j]=NULL;
	//display is useless for now
	//display[x][y]=display[i][j];
	//display[i][j]=tiles[i][j]->getDisplay();
}

//T is either mob or pc
//use template
template <class T> bool Floor::canStepOn(int x,int y,T& t){
	//double dispatch
	if (!t.canStepOn(*tiles[x][y]))
		return false;
	//if the tile can be stepped on
	//check if the event is NULL, or can it be stepped on
	if (cells[x][y]==NULL||t.canStepOn(*cells[x][y])){
		return true;
	}
	if (cells[x][y]==NULL)
		return true;
	return false;
}
/*
   bool Floor::canStepOn(int x,int y,Character& c){
   return c.canStepOn(*tiles[x][y]);
   }
   */

/*
   bool Floor::canStepOn(int x,int y,Mob& m){
   if (!tiles[x][y]->canPass(m))
   return false;
   if (cells[x][y]==NULL||cells[x][y]->canPass(m)){
   return true;
   }
   return false;
   }
   */
//used to set pc when starting the game
void Floor::setPC(Player* p){
	int x=p->getX();
	int y=p->getY();
	if (canStepOn(x,y,*p)){
		cells[x][y]=p;
	}
	PC=p;
}

//remove all dead mobs (and delete)
void Floor::removeDead(){
	for (int i=0;i<maxMob;i++)
		if (mobs[i]!=NULL&&mobs[i]->isDead()){
			cells[mobs[i]->getX()][mobs[i]->getY()]=NULL;
			delete mobs[i];
			mobNum--;
			mobs[i]=NULL;
		}
}

//< function for comparing mob pointers
//so that mob are sorted in row-col fashion
bool cmpMobPtr(Mob* a,Mob* b){
	if (a->getX()<b->getX())
		return true;
	else if (a->getX()==b->getX()){
		return a->getY()<b->getY();
	}
	return false;
}

//move mobs around or atk etc
void Floor::mobsInAction(){
	//nonNullMobs contains all mobs that are not null
	if (!move){ //if went up a floor, dont move
		move=true;
		return;
	}
	Mob* nonNullMobs[maxMob]; //get mobs that are not NULL
	int p=0;
	for (int i=0;i<maxMob;i++){
		if (mobs[i]!=NULL)
			nonNullMobs[p++]=mobs[i];
	}
	//sort them, so they can be moved in the specified order
	sort(nonNullMobs,nonNullMobs+p,cmpMobPtr);

	//sort(mobs,mobs+mobNum,cmpMobPtr);
	for (int i=0;i<p;i++){
		Mob* mob=nonNullMobs[i];
		//Mob* mob=mobs[i];	
		bool atked=false;
		//cout<<"whats wrong"<<endl;
		vector<int> possibleDir; //store the possible dirs
		//has aggro towards the PC, means it will try to atk pc
		//if (mob->hasAggro()){
		for (int j=0;j<8;j++){ //traverse all 8 direction
			int nx=mob->getX()+direction[j].getX();
			int ny=mob->getY()+direction[j].getY();
			//dragon specialized behaviour
			Dragon* d=dynamic_cast<Dragon*> (mob);
			if (d){
				if ((nx>=0&&nx<MAXH&&ny>=0&&ny<MAXW)&&cells[nx][ny]!=NULL&&cells[nx][ny]==PC){
					mob->hitEnemy(*PC); //try to see if it can atk
					break;
				}
				//the treasure's radius is also in his attack range
				Vec treasureLoc=d->getTreasureLoc();
				int tx=treasureLoc.getX()+direction[j].getX();
				int ty=treasureLoc.getY()+direction[j].getY();
				if (cells[tx][ty]!=NULL&&cells[tx][ty]==PC){
					mob->hitEnemy(*PC);
					break;
				}
			}
			else{
				//default behaviour
				//they can move
				if (nx>=0&&nx<MAXH&&ny>=0&&ny<MAXW){
					//cout<<nx<<" "<<ny<<endl;
					if (canStepOn(nx,ny,*mob)){
						//store all possible dirs that they can move to
						possibleDir.push_back(j);
						//cout<<"nx "<<nx<<" ny "<<ny<<endl;
					}
					if (mob->hasAggro()){ //merchants has no aggro
						if (cells[nx][ny]!=NULL&&cells[nx][ny]==PC){
							mob->hitEnemy(*PC);
							atked=true;
						}
					}
				}
			}
		}
		//}
		//did not attack
		if (!atked){ //choose a random direction to move
			if (possibleDir.size()==0)
				continue;
			//get random index
			int dirIdx=randInt(0,possibleDir.size()-1);
			Robot *r=dynamic_cast<Robot*> (mob);
			if (r){ //robots chase human
				int newX;
				int newY;
				double minDist=1<<30; //some max num
				//find min distance for each direction
				for (int k=0;k<possibleDir.size();k++){
					int x1=PC->getX();
					int y1=PC->getY();
					int x2=mob->getX()+direction[possibleDir[k]].getX();
					int y2=mob->getY()+direction[possibleDir[k]].getY();
					//do i need sqrt?
					//do a simple distance calculation (not best, but simple)
					double dist=sqrt(double((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
					//cout<<dist<<endl;
					if (dist<minDist){ //min dist
						minDist=dist;
						newX=x2;
						newY=y2;
					}
				}
				moveCharacter(*mob,newX,newY);
			}
			else{ //default behaviour, just random a spot and go
				int mobX=mob->getX(); //get current location
				int mobY=mob->getY();
				//new location
				int newX=mobX+direction[possibleDir[dirIdx]].getX();
				int newY=mobY+direction[possibleDir[dirIdx]].getY();
				moveCharacter(*mob,newX,newY);//move it
			}
		}
	}
}

//attack the target
//skill indicating if its using a special attack
bool Floor::atkThis(int x,int y,bool skill){
	if (skill){
		Mob *m=dynamic_cast<Mob*> (cells[x][y]);
		if (m){ //useSkillOn is not double dispatched due to time limitation
			return PC->useSkillOn(*m);
		}
		else{
			return false;
		}
	}
	//getHitBy double dispatch
	return cells[x][y]->getHitBy(*PC);

}

//collect exp and gold
Vec Floor::collectCorpse(int x,int y){
	Mob* m=dynamic_cast<Mob*> (cells[x][y]);
	//event is a mob..., saving time not to write every method in the inheritance class
	if (m){
		if (m->isDead()){
			logs.push_back(m->getName()+" has been slain");
			m->collectCorpse(); //do whatever (dragon release treasure)
			return Vec(m->getGoldDrop(),m->getExp());
		}
	}

	return Vec(0,0);
}

//check if its valid
bool Floor::isValidEventOnFloor(int x,int y){
	if (x>=0&&x<MAXH&&y>=0&&y<=MAXW&&cells[x][y]!=NULL)
		return true;
	return false;
}
/*
   bool Floor::triggerEventOnPC(int x,int y){
   return cells[x][y]->specialEvent(*PC);
   }
   */
/*
   bool Floor::isMob(int x,int y){
   return cells[x][y]->isMobile();
   }
   */

//use an item
//return true if its a valid use
bool Floor::useItem(int x,int y){
	if (!cells[x][y]->canUse())
		return false;
	//
	bool r =cells[x][y]->specialEvent(*PC);
	if (r){ //if special event success
		delete cells[x][y];
		cells[x][y]=NULL;
	}
	return r;
}

Vec Floor::getPlayerLoc(){
	return playerLoc;
}

ostream& operator<<(ostream& os,const Floor& f){

//dlc part if outputting some more info on coordinates
#ifdef DLC
	os<<" ";
	for (int j=0;j<79;j++)
		os<<j%10;
	os<<endl;
#endif
	for (int i=0;i<25;i++){
#ifdef DLC
		os<<i%10;
#endif
		for (int j=0;j<79;j++){
			if (f.cells[i][j]==NULL){
				/*
				if (f.tiles[i][j]==NULL){
					cout<<"iamerror "<<i<<" "<<j<<endl;
				}
				*/
				os<<f.tiles[i][j]->getDisplay();
			}
			else
				os<<f.cells[i][j]->getDisplay();
		}
		if (i!=24)
			os<<endl;
	}
	return os;
}

bool Floor::reachedEnd(){
	return endGame;
}
int Floor::getLevel(){
	return level;
}

//random generation
void Floor::floorRespawn(){
	//remains stand for locations that are not occupied
	for (int i=0;i<5;i++)
		remains[i]=chamber[i];
	generatePCStair();
	generatePotion();
	generateGold();
	generateMobs();
}


void Floor::generatePCStair() {
	int random = randInt(0,4); //random chamber
	int temp = randInt(0,remains[random].size()-1); //random a cell on the chamber
	playerLoc = remains[random][temp]; //get player location
	remains[random].erase (remains[random].begin() + temp); //take it out from remains
	int newRan = randInt(0,4); //get rand for stairs
	while (newRan == random) { //different from playerloc
		newRan = randInt(0,4);
	}
	temp = randInt(0,remains[newRan].size()-1); //random a cell on the chamber
	//where to store coordinates of stair?
	Vec stairLoc=remains[newRan][temp]; //just generate it
	cells[stairLoc.getX()][stairLoc.getY()]=new Stair(this);
	remains[newRan].erase (remains[newRan].begin() + temp);
}

void Floor::generatePotion() {
	for (int i = 0; i < 10; i++) {
		//get random chamber
		int chamberIdx = randInt(0,4);
		//get random chamber cell
		int cellIdx = randInt(0,remains[chamberIdx].size()-1);
		//get random potion type
		int potType = randInt(1,6);
		int px=remains[chamberIdx][cellIdx].getX();
		int py=remains[chamberIdx][cellIdx].getY();
		switch (potType) {
			case 1: 
				//RH POT
				cells[px][py]=new RHPotion(this);
				break;

			case 2: 
				//BA POT
				cells[px][py]=new BAPotion(this);
				break;

			case 3: 
				//BD POT
				cells[px][py]=new BDPotion(this);
				break;

			case 4: 
				//PH POT
				cells[px][py]=new PHPotion(this);
				break;

			case 5: 
				//WA POT
				cells[px][py]=new WAPotion(this);
				break;

			default: 
				//WD POT
				cells[px][py]=new WDPotion(this);
				break;

		}
		//store/display coordinates of potions
		remains[chamberIdx].erase (remains[chamberIdx].begin() + cellIdx);
	}
}

void Floor::generateGold() {
	for (int i = 0; i < 10; i++) {
		//get random chamber
		int chamberIdx = randInt(0,4);
		//get random cell
		int cellIdx = randInt(0,remains[chamberIdx].size()-1);
		int gx=remains[chamberIdx][cellIdx].getX();
		int gy=remains[chamberIdx][cellIdx].getY();
		//get random gold type based on distribution
		int goldDist[]={5,1,2}; //revert back to 1 512
		int goldType = randDist(goldDist,3);
		if (goldType==0)
			cells[gx][gy]=new Gold(this,2);
		else if(goldType==1){
			vector<Vec> dragonXYs; //generate a dragon idx
			vector<int> delIdx; //idxes that needs to be deleted
			for (int j=0;j<8;j++){ //find a position for dragon to reside
				Vec pos(gx+direction[j].getX(),gy+direction[j].getY());
				for (int k=0;k<remains[chamberIdx].size();k++){
					if (pos==remains[chamberIdx][k]){
						dragonXYs.push_back(pos);
						delIdx.push_back(k);
						break;
					}
				}
			}
			//no place...
			if (dragonXYs.size()==0){ //try generating something else
				i--;
				if (goldDist[0]==0&&goldDist[2]==0){
					cerr<<"expected error"<<endl;
				}
			}
			else{
				int dragIdx=randInt(0,dragonXYs.size()-1); //spawn dragon as well
				Treasure *t = new Treasure(this,gx,gy);
				spawnMobAt(dragonXYs[dragIdx].getX(),dragonXYs[dragIdx].getY(),'D',t);
				//delete the location used by dragon
				remains[chamberIdx].erase(remains[chamberIdx].begin()+delIdx[dragIdx]);
				//deleting 1 idx might affect cellIdx
				if (cellIdx>delIdx[dragIdx])
					cellIdx--;
				
				cells[gx][gy]=t;
			}
		}
		else if (goldType == 2) {
			cells[gx][gy]=new Gold(this,1);
		}
		remains[chamberIdx].erase (remains[chamberIdx].begin() + cellIdx);
	}
}

void Floor::generateMobs() {
	int spawningMobNum=20;
#ifdef DLC
	spawningMobNum=10+4*level; //DLC, num mob on each floor..
#endif
	for (int i = mobNum; i <spawningMobNum; i++) {
		//get random chamber
		int chamberIdx = randInt(0,4);
		//get random location
		int cellIdx = randInt(0,remains[chamberIdx].size()-1);
		int mx=remains[chamberIdx][cellIdx].getX();
		int my=remains[chamberIdx][cellIdx].getY();
		//get random mob type
		int mobDist[]={4,3,5,2,2,2};
		int mobType;
		mobType = randDist(mobDist,6);
#ifdef DLC
		//new mobs
		int mobDistX[]={4,3,5,2,2,2,2,4};
		mobType=randDist(mobDistX,8);
#endif
		if (mobType ==0) {		//2/9 Human
			spawnMobAt(mx,my,'H');
		}
		else if (mobType==1) {		//3/18 Dwarf  
			spawnMobAt(mx,my,'W');
		}
		else if (mobType==2) {		//5/18 Halfling
			spawnMobAt(mx,my,'L');

		}
		else if (mobType ==3) {		//1/9 Elf
			spawnMobAt(mx,my,'E');

		}
		else if (mobType ==4) {			//1/9 Orc
			spawnMobAt(mx,my,'O');
		}
		else if (mobType==5){									//1/9 Merchant
			spawnMobAt(mx,my,'M');	
		}
		else if (mobType==6){
			spawnMobAt(mx,my,'C');
		}
		else if (mobType==7){
			spawnMobAt(mx,my,'R');
		}
		//add new type that does not miss
		//Robots that chase the main char
		remains[chamberIdx].erase (remains[chamberIdx].begin() + cellIdx);
	}
}
