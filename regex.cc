#include <iostream>  
#include <regex>  
#include <string>  
#include <time.h>
#include <cstdlib>
  
using namespace std;
int main(){  
  // std::string s ("=========================Mon Oct 30 21:40:44 2017");  
	// string s ("1513689830:3:2:263:113:1:-1:2:2:-1:-1:-1:-1:data:send");
	string s ("1509506869:3:5:-1:-1:1:-1:-1:255:-1:-1:-1:-1:hello:send");	
		
	smatch m;  
	regex e ("(\\d)+:.*hello:send");   

  
	regex_search (s,m,e);  
	cout<<m.begin()->str().c_str()<<endl;
	cout<<atoi(m.begin()->str().c_str())<<endl;
	time_t time1 = (time_t)atoi(m.begin()->str().c_str());

	cout<<ctime(&time1)<<endl;	



    for (auto x=m.begin();x!=m.end();x++)  
    cout << x->str() << " ";  
	
 cout<<m.size()<<std::endl;
} 
