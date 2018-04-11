#include <iostream> 
#include <vector> 
#include <map> 
#include <math.h> 

using namespace std;

int main(){

	multimap<int, pair<int, int> > nkt;//<k, <m, t>>
	
	int a = 1;
	int b = 2;

	// nkt.insert({1 ,pair<int, int>(1, 2)});
	// nkt.insert({1 ,pair<int, int>(3, 4)});
	auto counts = nkt.count(1);
	auto iter = nkt.find(1);

	for (int m = 3; m < 8; m++) { 
		int n = pow(2, m) - 1;
		int mink = m + 1;

		for (int k = n - m; k >= mink ; k = k - m) { 
			int t = (n - k) / m;
			nkt.insert({n, pair<int, int>(k, t)});
		} 
	} 
	for(auto it = nkt.begin(); it != nkt.end(); it++){
		cout<<it->first<<", "<<it->second.first<<", "<<it->second.second<<endl;
	
	}







}
