#include <iostream> 
#include <fstream>
#include <cstdint>
#include <vector> 
#include <sys/stat.h> 
#include <string> 
#include <string.h> 
#include <math.h>
#include <algorithm> 
#include "rs.h" 

#define PKGSIZE 980

using namespace std;

int main(){

	// encodeFile("origin", PKGSIZE);
	// repairbroken("origin.rs", "broken", 9, 980);
 	decodeFile("171.jpg", PKGSIZE, 9);
	// decodeFile("origin.rs", PKGSIZE, 9);
	cout<<"hello world!!!"<<endl;



}
