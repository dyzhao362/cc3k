#include "rng.h"
#include <algorithm>

using namespace std;

//randInt(100) randInt(1,100)
int randInt(int a,int b){
	int lower=a<b?a:b;
	int upper=a<b?b:a;
	return rand()%(upper-lower+1)+lower;
}

bool isSuccess(int p,int q){
	if (randInt(1,q)<=p)
		return true;
	return false;
}

//{4,3,5,2,2,2}
//v is a distribution such as {3,1,3,4}
//return the randomed index
int randDist(vector<int> &v){
	int sum=0;
	vector<int>::iterator it;
	for (it=v.begin();it!=v.end();it++) //get sum
		sum+=(*it);
	if (sum==0) //avoid zero
		return 0;
	int randVal=randInt(1,sum); //get random value
	for (int i=0;i<v.size();i++){
		if (randVal<=v[i]) //see which index does it land in
			return i;
		randVal-=v[i];
	}
	return 0;
}
//same as above but using array
int randDist(int v[],int size){
	int sum=0;
	for (int i=0;i<size;i++)
		sum+=v[i];
	if (sum==0) //avoid zero
		return 0;
	int randVal=randInt(1,sum);
	for (int i=0;i<size;i++){
		if (randVal<=v[i])
			return i;
		randVal-=v[i];
	}
	return 0;
}
