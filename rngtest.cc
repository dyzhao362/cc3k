#include "rng.h"
#include <iostream>


using namespace std;

int main(){
	int a;
	while(cin>>a){
		cout<<randInt(1,a)<<endl;
	}
}
