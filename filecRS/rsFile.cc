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
#include "rsFile.h"

// #define PKGSIZE 980
using namespace std;


void encodeFile(string fileName, int PkgSize){
// 读入一个文件，对文件进行RS编码，将编码追加到文件末尾,另存为
	fstream file(fileName, ios::out|ios::in);
	file.seekg(0, ios::end);
	int filesize = file.tellg();
	file.seekg(0, ios::beg) ;

	cout<<"file size"<<filesize<<endl;
	int CODE_K = ceil(filesize / float(PkgSize));
	int CODE_N = pow(2, ceil(log10(CODE_K) / log10(2))) - 1;
	vector<string> fileMatrix;
	string temp(PkgSize, ' ');
	int byteLeft = filesize;

// read file loop	
	for (unsigned i = 0; i < CODE_K; i++) { 
		int readSize = min(PkgSize, filesize - (int)file.tellg());
		bzero((void*)temp.c_str(), temp.size());
		file.read((char*)temp.c_str(), readSize);
		fileMatrix.push_back(temp);
	}
	
	
	for (unsigned i = 0; i < CODE_N - CODE_K; i++)  
		fileMatrix.push_back(temp);

	cout<<"code init"<<endl;
	rs_init(CODE_N, CODE_K);
	cout<<"CODE_K : "<<CODE_K<<"  CODE_N : "<<CODE_N<<endl;
	uint8_t codeOriginal[CODE_N], codeCorrect[CODE_N];
	bzero(codeOriginal, sizeof(codeOriginal));
	bzero(codeCorrect, sizeof(codeCorrect));
// encode loop
	for (unsigned i = 0; i < PkgSize; i++) { 
		// cout<<"__________________ encode i : "<<i<<"______________"<<endl;
		for (unsigned j = 0; j < CODE_K; j++) { 
			codeOriginal[j] = (uint8_t)fileMatrix[j].c_str()[i];
		} 

		rs_encode(codeOriginal);

		for (unsigned j = CODE_K; j < CODE_N; j++) { 

			memcpy((char*)fileMatrix[j].c_str() + i, codeOriginal + j, 1);
			// cout<<hex<<(int)codeOriginal[j]<<"  "<<(int)(unsigned char)(fileMatrix[j].c_str()[i])<<dec<<endl;
		} 
	} 

	string rsFileName = fileName.append(".rs");
	cout<<".rs file name : "<<rsFileName<<endl;
	ofstream rsFile(rsFileName, ios::out|ios::in);
	for (unsigned i = 0; i < CODE_N; i++) { 
		// rsFile.seekg(PKGSIZE * i, ios::beg);
		rsFile.write(fileMatrix[i].c_str(), PkgSize);
	} 
	rsFile.close();
	fstream file1(rsFileName, ios::in|ios::out);
	file1.seekg(0, ios::end) ;
	int filesize1 = file1.tellg();
	cout<<"file size"<<filesize1<<endl;


}

void decodeFile(string fileName, int PkgSize, int Code_K){
// 读入一个文件，对文件进行RS解码，解码后将文件末尾的RS编码去除，另存为

	fstream file(fileName, ios::out|ios::in);
	file.seekg(0, ios::end);
	int filesize = file.tellg();
	file.seekg(0, ios::beg) ;

	cout<<"file size"<<filesize<<endl;
	int CODE_K = Code_K;
	int CODE_N = pow(2, ceil(log10(CODE_K) / log10(2))) - 1;

	rs_init(CODE_N, CODE_K);
	vector<string> fileMatrix;
	string temp(PkgSize, ' ');
	int byteLeft = filesize;

	for (unsigned i = 0; i < CODE_N; i++) { 
		int readSize = min(PkgSize, filesize - (int)file.tellg());
		file.read((char*)temp.c_str(), readSize);
		fileMatrix.push_back(temp);
	}

	int m[PKGSIZE] = {1};
	uint8_t codeOriginal[CODE_N], codeCorrect[CODE_N];
	bzero(codeOriginal, sizeof(codeOriginal));
	bzero(codeCorrect, sizeof(codeCorrect));

	for (unsigned i = 0; i < PkgSize; i++) { 
		// cout<<"================== decode i : "<<i<<"=============="<<endl;
		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy(codeCorrect + j, (char*)fileMatrix[j].c_str() + i, 1);
			// cout<<hex<<(int)codeCorrect[j]<<dec<<endl;
		} 
		
		// cout<<endl;
		m[i] = rs_decode(codeCorrect);

		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy((char*)fileMatrix[j].c_str() + i, codeCorrect + j, 1);
		} 

	} 

	string foutName = fileName + ".repair";
	cout<<"decode file : "<<foutName<<endl;
	ofstream fout(foutName, ios::binary);
	
	// string content(100, 'a');
	// fout.write(content.c_str(), 100);
	for (unsigned i = 0; i < CODE_K; i++) { 

		int nonZeroNum = 0;
		if(i == CODE_K - 1){
			for(int len = PkgSize; len > 0; len--){
				if(fileMatrix[i].c_str()[len] == 0)
					nonZeroNum = len;
					break;
			}
			cout<<"nonZeroNum : "<<nonZeroNum<<endl;
		fout.write(fileMatrix[i].c_str(), nonZeroNum);

		} else fout.write(fileMatrix[i].c_str(), PkgSize);

	} 
	fout.close();







}

