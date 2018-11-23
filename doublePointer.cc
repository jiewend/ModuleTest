#include <iostream> 

using namespace std;

int main(){
	int *p;
	p = new int[10];
	for (unsigned i = 0; i < 10; i++) { 
		cout<<*(p + i);
	} 
	cout<<endl;
}
