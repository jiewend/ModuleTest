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


	ofstream outfile;
	ifstream infile;
	fstream  file("origin", ios::out|ios::in);
	file.seekg(0, ios::end);
	int filesize = file.tellg();

	file.seekg(0, ios::beg) ;
	int columns = ceil(filesize / float(PKGSIZE));
	int CODE_N = pow(2, ceil(log10(columns) / log10(2))) - 1;
	int CODE_K = columns;
	vector<string> fileMatrix, fileMatrix1;
	string temp(PKGSIZE, ' '), temp1(columns, ' ');
	int byteLeft = filesize;
	for (unsigned i = 0; i < columns; i++) { 
		int readSize = min(PKGSIZE, filesize - (int)file.tellg());
		file.read((char*)temp.c_str(), readSize);
		// cout<<"readSize : "<<readSize<<endl;
		// cout<<"temp size : "<<temp.size()<<endl;
		fileMatrix.push_back(temp);
	}

	// temp.resize(0);
	for (unsigned i = 0; i < CODE_N - CODE_K; i++)  
		fileMatrix.push_back(temp);

	cout<<"code init"<<endl;
	rs_init(CODE_N, CODE_K);
	cout<<"CODE_K : "<<CODE_K<<"  CODE_N : "<<CODE_N<<endl;
	uint8_t codeOriginal[CODE_N], codeCorrect[CODE_N];
	bzero(codeOriginal, sizeof(codeOriginal));
	bzero(codeCorrect, sizeof(codeCorrect));
	//**************************** encode loop *************************************
	for (unsigned i = 0; i < 2; i++) { 
		cout<<"__________________ encode i : "<<i<<"______________"<<endl;
		for (unsigned j = 0; j < CODE_K; j++) { 
			codeOriginal[j] = (uint8_t)fileMatrix[j].c_str()[i];
		} 

		rs_encode(codeOriginal);

		for (unsigned j = CODE_K; j < CODE_N; j++) { 

			memcpy((char*)fileMatrix[j].c_str() + i, codeOriginal + j, 1);
			cout<<hex<<(int)codeOriginal[j]<<"  "<<(int)(unsigned char)(fileMatrix[j].c_str()[i])<<dec<<endl;
		} 
	} 




	
	//**************************** decode loop ************************************
	int m[PKGSIZE] = {1};
	for (unsigned i = 0; i < 2; i++) { 
		cout<<"================== decode i : "<<i<<"=============="<<endl;
		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy(codeCorrect + j, (char*)fileMatrix[j].c_str() + i, 1);
			cout<<hex<<(int)codeCorrect[j]<<dec<<endl;
		} 
		
		cout<<endl;
		m[i] = rs_decode(codeCorrect);

		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy((char*)fileMatrix[j].c_str() + i, codeCorrect + j, 1);
		} 

	} 




	cout<<"file size : "<<filesize<<endl;
	cout<<"fileMatrix size : "<<fileMatrix.size()<<endl;
	cout<<"columns : "<<columns<<endl;

	cout<<"hello world!"<<endl;
	return 0;
}
