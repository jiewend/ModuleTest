#include "stdio.h"   
#include "stdlib.h"   
#include "time.h"   
#include <iostream> 
using namespace std;
int main( void )   
{   
long i = 10000000L;   
clock_t start, finish;   
double duration;   
/* ����һ���¼�������ʱ��*/   
printf( "Time to do %ld empty loops is ", i );   
start = clock();   
while( i-- ) ;   
finish = clock();   
duration = (double)(finish - start) / CLOCKS_PER_SEC;   
// printf("%f seconds n", duration );   
cout<<CLOCKS_PER_SEC<<endl;
} 
