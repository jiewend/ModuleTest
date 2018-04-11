#include <fstream> 
#include <iostream> 
#include <string> 
#include <string.h> 

using namespace std;

int main(){
	
	string fileName("aa.rs");
	cout<<fileName<<endl;
	string content(100, 'a');
	ofstream file(fileName, ios::binary);
	cout<<content<<endl;
	file.write(content.c_str(), 100);
	file.close();
	
}
