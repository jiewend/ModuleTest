
#include"RandNum1.h"
#include"Decode.h"
#include"Encode.h"
#include"Source.h"
#include"stdafx.h"
#include <fstream>
#include "GauChannel.h"
#include"sstream"
using namespace std;
#define CHANNEL  0
#define TIMES 5000                          //�ظ��������

long long processNum = 0;

RandNum1  rndGen0;
//CLCRandNum rndGen0;                       //����һ�����������randnum.h�ж��壬���������
//CWHRandNum rndGen1;                       //����һ�������






#define M  504
#define N  1008
//#define M1 499
//#define M 500
//#define N 10000
int iter_len=100;                       

int main()
{  
	cout<<"begin main"<<endl;
    double Snr;
    double sigma=0;
	int k,c,i,j,success_num,m,n,t;
    clock_t start1,end;
    start1=clock();    
    string *row_weight;
    row_weight=new string[M]; 

    int **H;
    H=new int*[M];
    for(k=0;k<M;k++)
      H[k]=new int[N];
    int **H1;
    H1=new int*[M];
    for(k=0;k<M;k++)
     H1[k]=new int[N];

   int *KK;
   int *uu;
   int ss_len,ld_len,lt_len;               //�ֱ�Ϊ��Դ�볤��LDPC�볤��LT�볤
   ss_len=N-M;
  // ss_len=N-M1;                          //500*10000����ʱ
   ld_len=N;
   lt_len=(int)(N*2);

   uu=new int[ss_len];                    //��Դ
   for(c=0;c<ss_len;c++)
	  uu[c]=0;
   int *c_ld;                             //LDPC��
    c_ld=new int[N];
   int *c_rap;                             //Raptor��
   c_rap=new int[lt_len];
   for(i=0;i<lt_len;i++)                 
	  c_rap[i]=0;
   double *c_rap1;
   c_rap1=new double[lt_len];

   KK=new int[M];                            //���ڱ������
   for( c=0;c<M;c++)
	 KK[c]=0;
//   int *c_rap2;
 //  c_rap2=new int[lt_len];
   int **H2;
   H2=new int*[lt_len];
   for(k=0;k<lt_len;k++)
     H2[k]=new int[ld_len];
   for( m=0;m<M;m++)                 //��ʼ��H����
	 for(n=0;n<N;n++)
	 {
		 H[m][n]=0;
         H1[m][n]=0;
	 }
 for(m=0;m<lt_len;m++)
	 for(n=0;n<ld_len;n++)
		 H2[m][n]=0;
 //---------------------------------------------------------
 int* Row_weight=new int[M];
 int* Col_weight=new int[N];
 int** Row=new int*[M];
 int** Col=new int*[N];
 int** Row_pos=new int*[M];
 int** Col_pos=new int*[N];
   
 cout<<"read H matrix"<<endl;
//------------------------------��ȡH����------------------------------------

 string line,num;
 ifstream fin;
 const char* tmp;
 fin.open("row_weight_distribution1.txt",ios::in);
  i=0;
  m=0;
  n=0;
 
 while(getline(fin,line))
 { 
  istringstream record(line);             //istringstream������԰�һ���ַ�����Ȼ���Կո�Ϊ�ָ����Ѹ��зָ�������
  record>>num;
  tmp=num.c_str();
  Row_weight[m]=atoi(tmp);
  Row[m]=new int[atoi(tmp)];
  Row_pos[m]=new int[atoi(tmp)];
  m++;
  j=0;
  while(record>>num)
     {
	   tmp = num.c_str();     //��string����ת��Ϊconst char*
	   int b=atoi(tmp);
//	   cout<<b<<" ";
       H[i][b]=1;
	   H1[i][b]=1;
	   Row[i][j++]=b;
     }
 // cout<<endl;
  i++;
 }
 fin.close();
 fin.open("column_weight_distribution1.txt",ios::in);
 i=0;
 while(getline(fin,line))
 { 
  istringstream record(line);             //istringstream������԰�һ���ַ�����Ȼ���Կո�Ϊ�ָ����Ѹ��зָ�������
  record>>num;
  tmp=num.c_str();
  Col_weight[n]=atoi(tmp);
  Col[n]=new int[atoi(tmp)];
  Col_pos[n]=new int[atoi(tmp)];
  n++;
  j=0;
  while(record>>num)
     {
	   tmp = num.c_str();     //��string����ת��Ϊconst char*
	   int b=atoi(tmp);     
	   Col[i][j++]=b;
     }
  i++;
 }
 fin.close();

 double** QQ=new double*[N];
 for(i=0;i<N;i++)
	 QQ[i]=new double[Col_weight[i]];
 double** RR=new double*[M];
 for(i=0;i<M;i++)
	 RR[i]=new double[Row_weight[i]];
 for(i=0;i<M;i++)
 {
	 for(j=0;j<Row_weight[i];j++)
	 {
		 t=0;
		 while(Col[Row[i][j]][t]!=i)
		 {
			 t++;
		 }
		 Row_pos[i][j]=t;
	 }
 }

 for(i=0;i<N;i++)
 {
	 for(j=0;j<Col_weight[i];j++)
	 {
		 t=0;
		 while(Row[Col[i][j]][t]!=i)
		 {
			 t++;
		 }
		 Col_pos[i][j]=t;
	 }
 }
	 





 int* LT_Row_weight=new int[lt_len];
 int* LT_Col_weight=new int[ld_len];
 int** LT_Row=new int*[lt_len];
 int** LT_Col=new int*[ld_len];
 int** LT_Row_pos=new int*[lt_len];
 int** LT_Col_pos=new int*[ld_len];


 



	CSource SS;                            //����CSource����SS
	CEncode C_encode;                      //����CEncode����C_encode
	CDecode D_CC;                          //����CDecode����D_CC
	Gau_Channel Gau1;                      //�����˹�ŵ�����
    int fail_time=0;



cout<<"encode process"<<endl;	
//--------------------------------------�������-----------------------------	
  
	   
	   double biterr;
	   int times;
	
	     SS.GetBitStr(uu,ss_len);                          //������Դ
		 cout<<"uu : ";
		 for (unsigned i = 0; i < ss_len; i++) { 
			 cout<<*(uu+i);
		 } 
		 cout<<endl;
		 for(i=0;i<ld_len;i++)
			 c_ld[i]=0;
         C_encode.Encode_ldpc(H,M,N,KK,ld_len,uu,c_ld);    //LDPC�������
		 cout<<"c_ld : ";
		 for (unsigned i = 0; i < N; i++) { 
			 cout<<*(c_ld+i);
		 } 
		 cout<<endl;
	//	  for(i=0;i<ld_len;i++)          //ȫ0��������
	//		 c_ld[i]=0;
		 C_encode.Encode_LT(c_ld,c_rap,ld_len,lt_len,H2);  //LT��������
		 cout<<"c_rap : ";
		 for (unsigned i = 0; i < lt_len; i++) { 
			 cout<<*(c_rap+i);
		 } 
		 cout<<endl;




cout<<"store infor LT need"<<endl;		 
//------------------------------------�洢LT�������������Ϣ--------------------------------------
		
		 for(i=0;i<lt_len;i++)
		 {   
			 LT_Row_weight[i]=0;
			 t=0;
			 for(j=0;j<ld_len;j++)
			 {  
				 if(H2[i][j]==1)
				 {
					 LT_Row_weight[i]++;
				 }
			 }
			 LT_Row[i]=new int[LT_Row_weight[i]];
			 LT_Row_pos[i]=new int[LT_Row_weight[i]];
			 for(j=0;j<ld_len;j++)
			 {
				 if(H2[i][j]==1)
				 {
					 LT_Row[i][t++]=j;
				 }
			 }
		 }
		 for(j=0;j<ld_len;j++)
		 {   
			 LT_Col_weight[j]=0;
			 t=0;
			 for(i=0;i<lt_len;i++)
			 {  
				 if(H2[i][j]==1)
				 {
					 LT_Col_weight[j]++;
				 }
			 }
			 LT_Col[j]=new int[LT_Col_weight[j]];
			 LT_Col_pos[j]=new int[LT_Col_weight[j]];
			 for(i=0;i<lt_len;i++)
			 {
				 if(H2[i][j]==1)
				 {
					 LT_Col[j][t++]=i;
				 }
			 }
		 }
         
		 for(i=0;i<lt_len;i++)
       {
	     for(j=0;j<LT_Row_weight[i];j++)
	     {
		     t=0;
		     while(LT_Col[LT_Row[i][j]][t]!=i)
		     {
			  t++;
		     }
		     LT_Row_pos[i][j]=t;
	     }
       }

        for(i=0;i<N;i++)
      {
	    for(j=0;j<LT_Col_weight[i];j++)
	   {
		 t=0;
		 while(LT_Row[LT_Col[i][j]][t]!=i)
		 {
			 t++;
		 }
		 LT_Col_pos[i][j]=t;
	   }
      }
		
		 double** QQ1=new double*[ld_len];
		 for(i=0;i<ld_len;i++)
		 {
			 QQ1[i]=new double[LT_Col_weight[i]];
		 }
		 double** RR1=new double*[lt_len];
		 for(i=0;i<lt_len;i++)
		 {
			 RR1[i]=new double[LT_Row_weight[i]];
		 }

cout<<"throught Gau_Channel and decode"<<endl;		 
//------------------------------������˹�ŵ�����������-------------------------------------------------
	  if(CHANNEL==0)
      {   
	     for(Snr=4.5;Snr<=7;Snr=(Snr+0.5))               //�˴�Snr��ΪEbN0
	    {  
		 fail_time=0;
		 times=0;
		 sigma=(double)sqrt(1.0*(double)lt_len/((pow(10.0,(Snr/10))*(double)ss_len)));
         while((fail_time<100)&&(times<TIMES))           //�Ѽ�100���������֣���������5000��
		 { 
             Gau1.addnoise(Snr,c_rap,c_rap1,ss_len,lt_len);
			 success_num=D_CC.Raptor_AWGN(c_ld, M, ld_len, lt_len, iter_len,c_rap1, H1,sigma,
			   Row,Row_weight,Row_pos,Col,Col_weight,Col_pos,
			   LT_Row,LT_Row_weight,LT_Row_pos,LT_Col,LT_Col_weight,LT_Col_pos,
			   QQ, RR,QQ1, RR1 );   
           if(success_num!=ld_len)
			   fail_time++;
		   cout<<"��"<<times+1<<"�ν�������-----�ɹ�������Ϊ"<<success_num<<endl;
		   times++;
		 }
		   biterr=(double)fail_time/(double)times;
           ofstream fout("raptor-awgn1.txt",ios::app|ios::out);
		//   fout<<EbN0<<" ";
		   fout<<Snr<<" "<<biterr<<endl;
		   fout.close();
	  }
      
		 //
	 
	

	
	delete []QQ;
	delete []QQ1;
	delete []RR;
	delete []RR1;
   }


	delete[] uu;
	delete[] H;
	delete[] H1;
	delete[] H2;
	delete[] KK;
	delete[] c_ld;
	delete[] c_rap;
	delete[] c_rap1;
///	delete[] c_rap2;
	
	end=clock();
	cout<<"runnning time is"<<((double)(end-start1))<<"S";
	
	return 0;
	}




