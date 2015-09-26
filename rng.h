#ifndef __RNG_H__
#define __RNG_H__

//#include <cstdio>
#include <cstdlib>
//#include <ctime>
#include <vector>
//pick monsters to spawn


//get a random int between a and b, or [0,a]
//randInt(5) or randInt(2,5)
int randInt(int a,int b=0);

//probability p/q
bool isSuccess(int p,int q);

int randDist(int arr[],int size);

int randDist(std::vector<int> &v); 
//take the sum of v[i], take rand[0,sum), find the interval i


#endif
