#include "floor.h"
#include "event.h"
#include "tile.h"
#include "character.h"
#include "player.h"
#include "mob.h"
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "stringconverter.h"

using namespace std;


//const int MAXH=25;
//const int MAXW=79;

map<string,int> dirMap;
Vec direction[8];
vector<string> logs;
string dirs[]={"nw","no","ne","ea","se","so","sw","we"};
string dirDisplay[]={"North West","North","North East","East","South East","South","South West","West"};


void displayRaceMsg(){
	cout<<"Please choose a race (e.g. s<enter> for shade)"<<endl;
#ifdef DLC
	cout<<"Non-troll races get a 20% crit chance"<<endl;
#endif
	cout<<"s - shade: 125HP, 25ATK, 25DEF; 1.5x end game gold point"<<endl;
	cout<<"d - drow: 150HP, 25ATK, 15DEF; 1.5x Potion effect"<<endl;
	cout<<"v - vampire: 50HP, 25ATK, 25DEF; 5HP lifesteal"<<endl;
	cout<<"t - troll: 120HP, 25ATK, 15DEF; regains 5HP per turn, HP capped at 120HP"<<endl;
	cout<<"g - goblin: 110HP, 15ATK, 20DEF; steals 5 extra gold"<<endl;
	cout<<"q - quit game"<<endl;
}


Vec getValidXY(string parameter,Floor& theFloor,Player* p){
	for (int i=0;i<8;i++){
		if (dirs[i]==parameter){
			int X=p->getX()+direction[i].getX();
			int Y=p->getY()+direction[i].getY();
			if (theFloor.isValidEventOnFloor(X,Y)){
				return Vec(X,Y);
			}
		}
	}
	return Vec(-1,-1);
}

int main(int argc,char* argv[]){
	srand(time(NULL));
	//set the directions
	direction[0].setXY(-1,-1); //nw
	direction[1].setXY(-1,0); //no
	direction[2].setXY(-1,1); //ne
	direction[3].setXY(0,1); //ea
	direction[4].setXY(1,1); //se
	direction[5].setXY(1,0); //so
	direction[6].setXY(1,-1); //sw
	direction[7].setXY(0,-1); //we

	for (int i=0;i<8;i++){
		dirMap[dirs[i]]=i;
	}


	while(1){
		Floor theFloor;
		Player *p;
		Vec loc; //player location
		//debug
		//read setting everytime up a floor
		if (argc==2){
			theFloor.setFin(argv[1]); //and read floor setting
			loc=theFloor.getPlayerLoc();
		}
		else{
			ifstream fin("init.txt"); //read default setting
			theFloor.readFloorSetting(fin);
			theFloor.floorRespawn();
			loc=theFloor.getPlayerLoc();
		}


		displayRaceMsg();
		string cmd;
		while(1){
			if (!getline(cin,cmd))
				return 0;
			//Player *p = new Vampire(6,6,&theFloor);
			stringstream strParser;
			strParser<<cmd;
			strParser>>cmd;

			if (cmd=="s"){
				p=new Shade(loc.getX(),loc.getY(),&theFloor);
				break;
			}
			else if(cmd=="d"){
				p=new Drow(loc.getX(),loc.getY(),&theFloor);
				break;
			}
			else if (cmd=="v"){
				p=new Vampire(loc.getX(),loc.getY(),&theFloor);
				break;
			}
			else if (cmd=="t"){
				p=new Troll(loc.getX(),loc.getY(),&theFloor);
				break;
			}
			else if (cmd=="g"){
				p=new Goblin(loc.getX(),loc.getY(),&theFloor);
				break;
			}
			else if (cmd=="q"){
				return 0;
			}
		}
		theFloor.setPC(p);
		logs.clear();

		//theFloor.setPC(p);
		cout<<theFloor<<endl;
		cout<<"Player Character has spawned"<<endl;

		//change to getline, so cmd u/a can be used
		while(getline(cin,cmd)){
			bool invalid=true;
			stringstream ss;
			ss<<cmd;
			string cmdType;
			string parameter;
			ss>>cmdType;

			if (cmdType=="u"){
				ss>>parameter;
					Vec targetXY=getValidXY(parameter,theFloor,p);
					int itemX=targetXY.getX();
					int itemY=targetXY.getY();
					if (itemX!=-1||itemY!=-1){
						if (theFloor.useItem(itemX,itemY))
							invalid=false;
					}
					if (invalid){
						logs.push_back("invalid usage");
					}
				
			}
			else if(cmdType=="a"){
				ss>>parameter;
					Vec targetXY=getValidXY(parameter,theFloor,p);
					int mobX=targetXY.getX();
					int mobY=targetXY.getY();
					if (mobX!=-1||mobY!=-1){
						if (theFloor.atkThis(mobX,mobY)){
							invalid=false;
							Vec gexp=theFloor.collectCorpse(mobX,mobY);
							if (gexp.getX()){
								p->gainGold(gexp.getX());
								p->gainExp(gexp.getY());
							}		
						}
					}
					if (invalid){
						logs.push_back("invalid attack");
					}
				
			}
#ifdef DLC

			else if(cmdType=="fly"){
				int fx;int fy;
				if (ss>>fx>>fy){
					if (p->hasMp(7)){
						if (p->fly(fx,fy)){
							logs.push_back("PC costed 7 mp and flew to "+toStr(fx)+" "+toStr(fy)+".");
							invalid=false;
						}
						else{
							logs.push_back("invalid coordinates");
						}
					}
					else{
						logs.push_back("Not enough mp, you need 7 mp to fly.");
					}
				}
			}
			else if(cmdType=="s"){ //use skill on a mob
				ss>>parameter;
					Vec targetXY=getValidXY(parameter,theFloor,p);
					int mobX=targetXY.getX();
					int mobY=targetXY.getY();
					if (mobX!=-1||mobY!=-1){
						if (theFloor.atkThis(mobX,mobY,true)){
							invalid=false;
							Vec gexp=theFloor.collectCorpse(mobX,mobY);
							if (gexp.getX()){
								p->gainGold(gexp.getX());
								p->gainExp(gexp.getY());
							}	
						}

					}
					if (invalid){
						logs.push_back("invalid speacial ttack usage");
					}
				
			}
#endif
			else if (cmdType=="r"){
				cout<<"restarting game"<<endl;
/*
				bapotion.cc  phpotion.cc  rhpotion.cc  wdpotion.cc
	bdpotion.cc  potion.cc    wapotion.cc
*/
				BAPotion::reset();
				PHPotion::reset();
				RHPotion::reset();
				WDPotion::reset();
				BDPotion::reset();
				WAPotion::reset();
				Merchant::reset();
				break;
			}
			else{
				//register a move command
				for (int i=0;i<8;i++){
					if (dirs[i]==cmdType){
						logs.push_back(string("PC moves ")+dirDisplay[i]+".");
						if (p->makeMove(i)){
							invalid=false;
						}
						else{
							logs.pop_back(); //remove the move message
							logs.push_back("Invalid Move");
						}
						break;
					}
				}
			}
			if (theFloor.reachedEnd()){															//display score when the player character reaches stairs on floor 5
				cout<<"!@#$%^&*()---------------------------------)(*&^%$#@!"<<endl;
				cout<<"Congratulations, you have reached the end of the game!"<<endl;
				cout<<"Your score is: "<<p->getScore()<<endl;
				cout<<endl;
				break;
			}
			if (!invalid){
				//these are now called by player
				theFloor.removeDead();
				theFloor.mobsInAction(); //each mob move or atk

				cout<<theFloor<<endl;
				cout<<*p;
			}
			if (p->isDead()){
				logs.push_back("PC has been slain, game over.");
				logs.push_back(string("Your score is: ")+toStr(p->getScore()));							//displays score when the character's health reaches 0
			}

			//log is global
			for (int i=0;i<logs.size();i++){
				cout<<logs[i]<<endl;
			}
			logs.clear();
			if (p->isDead()){
				cout<<endl;
				break;
			}

		}
		delete p;
	}
}

