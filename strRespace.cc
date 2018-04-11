#include <string.h> 
#include <iostream> 
using namespace std;


void replaceSpace(char *str,int length) {
	int spaceNum = 0;
	char respace[3] = {'%', '2', '0'};
	for(int i = 0; i < length; i++){
		if(*(str + i) == ' ')
			spaceNum++;
	}
	cout<<"spaceNum : "<<spaceNum<<endl;
	char *newStr = new char[length + spaceNum * 2];
	int j = 0;
	for(int i = 0;i < length; i++){
		if(*(str + i) == ' '){
			memcpy(newStr + j, respace, 3);
			j = j + 3;
		} else {
				memcpy(newStr + j, str + i, 1);
				j++;
			}
		}
	cout<<"newStr : "<<newStr<<endl;
	*str = *newStr;
}

int main(){
	string ll("hello world");
	char * str = new char[12];
	memcpy(str, ll.c_str(), ll.size());
	cout<<str<<endl;
	replaceSpace(str, ll.size());
	
	cout<<str<<endl;

}
