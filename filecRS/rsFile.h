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


void encodeFile(string fileName, int PkgSize);
void decodeFile(string fileName, int PkgSize, int Code_K);
