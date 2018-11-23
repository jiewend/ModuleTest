#include <iostream> 
#include <stdlib.h> 
#include <math.h> 
using namespace std;

void quickSort(int* arr, int l, int r){
	int i, j, x, temp;
	if(l<r){
		i = l;
		j = r;
		x = arr[(l + r) / 2];
		while(1){
			while(i <= r && arr[i] < x)
				i++;
			while(j >= l && arr[j] > x)
				j--;
			if(i >= j)
				break;
			else{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
		quickSort(arr, l, i - 1);
		quickSort(arr, j+1, r);
	}
	
}

int main(){
	float toal;
    int num;
	cout<<"intput the total money and num"<<endl;
	cin>>toal;
	cin>>num;
	int scalTotal = ceil(toal * 100);
	cout<<"scalTotal : "<<scalTotal<<endl;

	int arry[scalTotal];
	int eachMoney[num];
	for (unsigned i = 0; i < scalTotal; i++)  
		arry[i] = i;
	for (unsigned i = 0; i < num - 1; i++) { 
		int index = rand() % scalTotal;
		eachMoney[i] = arry[index];
		if(index != scalTotal - 1)
			arry[index] = arry[scalTotal - 1];
		scalTotal--;
	} 
	eachMoney[num - 1] = ceil(toal * 100);
	quickSort(eachMoney, 0, num-1);
	float sum = eachMoney[0] / float(100);
	cout<<eachMoney[0] / float(100)<<", ";
	for (unsigned i = 1; i < num; i++) { 
		cout<<(eachMoney[i] - eachMoney[i - 1]) / float(100)<<", ";
		sum += (eachMoney[i] - eachMoney[i - 1]) / float(100);
	} 
	cout<<endl;
	cout<<"all : "<<sum<<endl;


}
