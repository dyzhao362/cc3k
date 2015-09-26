#include "stringconverter.h"

using namespace std;

string toStr(int i){		//converts int to string
	stringstream ss;
	ss<<i;
	return ss.str();
}

string toStr(char c){			//converts char to string
	stringstream ss;
	ss<<c;
	return ss.str();
}
