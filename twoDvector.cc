#include <iostream> 
#include <vector> 

using namespace std;

int main(){

	vector<vector<unsigned int> >vv(5);
	vector<unsigned int> v(4);
	for (unsigned i = 0; i < vv.size(); i++) { 
	    for (unsigned j = 0; j < v.size(); j++) {
		   v[j] = i + j;
	    }  
		vv[i] = v;
	} 
	
	cout<<"vv.size : "<<vv.size()<<" v.size: "<<v.size()<<endl;
	for (unsigned i = 0; i < vv.size(); i++) { 
	    for (unsigned j = 0; j < v.size(); j++) { 
	         cout<<(unsigned int)vv[i][j]<<", ";
	    } 
		cout<<endl;
	} 

	

}
