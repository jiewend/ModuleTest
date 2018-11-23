
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
#define TIMES 100000


RandNum1  rndGen0;
//CLCRandNum rndGen0;                       //定义一个类对象，类在randnum.h中定义，产生随机数
//CWHRandNum rndGen1;                       //定义一个类对象





//#define M  2000
//#define N  4000

//#define M  504
//#define N  1008

#define M  250
#define N  5000

//#define M  252
//#define N  504

//#define M1 499
//#define M 500
//#define N 10000
int iter_len=80;                       

int main()
{  
//	double EbN0;
	double ber;
	double wer;
	int e_num;
	int flag;
    double Snr;
    double sigma=0;
	double pp=1.5;
	 int fail_time;
	int k,c,i,j,success_num,m,n,t;
    clock_t start1,end;
    start1=clock();    
 //   string *row_weight;
 //   row_weight=new string[M]; 

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
   int ss_len,ld_len,lt_len;
   ss_len=N-M;
   //ss_len=N-M1;                          //500*10000矩阵时
   ld_len=N;
   uu=new int[ss_len];
   for(c=0;c<ss_len;c++)
	  uu[c]=0;
   int *c_ld;
    c_ld=new int[N];
 
   KK=new int[M];
   for( c=0;c<M;c++)
	 KK[c]=0;
  
//   int *c_rap2;
 //  c_rap2=new int[lt_len];
  
   for( m=0;m<M;m++)                 //初始化H矩阵
	 for(n=0;n<N;n++)
	 {
		 H[m][n]=0;
         H1[m][n]=0;
	 }
 
 //-------------------------------------------------------------------------
 int* Row_weight=new int[M];
 int* Col_weight=new int[N];
 int** Row=new int*[M];
 int** Col=new int*[N];
 int** Row_pos=new int*[M];
 int** Col_pos=new int*[N];
 

    CSource SS;                            //定义CSource对象SS
	CEncode C_encode;                      //定义CEncode对象C_encode
	CDecode D_CC;                          //定义CDecode对象D_CC
	Gau_Channel Gau1;
//------------------------------读取H矩阵------------------------------------

 string line,num;
 ifstream fin;
 const char* tmp;
 //fin.open("row_weight_distribution3.txt",ios::in);
fin.open("New_row_weight_distribution.txt",ios::in);
  i=0;
  m=0;
  n=0;
 
 while(getline(fin,line))
 { 
  istringstream record(line);             //istringstream对象可以绑定一行字符串，然后以空格为分隔符把该行分隔开来。
  record>>num;
  tmp=num.c_str();
  Row_weight[m]=atoi(tmp);
  Row[m]=new int[atoi(tmp)];
  Row_pos[m]=new int[atoi(tmp)];
  m++;
  j=0;
  while(record>>num)
     {
	   tmp = num.c_str();     //将string类型转化为const char*
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
  //fin.open("column_weight_distribution3.txt",ios::in);
fin.open("New_column_weight_distribution.txt",ios::in);
 i=0;
 while(getline(fin,line))
 { 
  istringstream record(line);             //istringstream对象可以绑定一行字符串，然后以空格为分隔符把该行分隔开来。
  record>>num;
  tmp=num.c_str();
  Col_weight[n]=atoi(tmp);
  Col[n]=new int[atoi(tmp)];
  Col_pos[n]=new int[atoi(tmp)];
  n++;
  j=0;
  while(record>>num)
     {
	   tmp = num.c_str();     //将string类型转化为const char*
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
	 



 lt_len=(int)ss_len*pp;

  int *c_rap;
   c_rap=new int[lt_len];
   for(i=0;i<lt_len;i++)
	  c_rap[i]=0;
   double *c_rap1;
   c_rap1=new double[lt_len];
 int **H2;
 H2=new int*[lt_len];
 for(k=0;k<lt_len;k++)
   H2[k]=new int[ld_len];
 for(m=0;m<lt_len;m++)
   for(n=0;n<ld_len;n++)
	 H2[m][n]=0;

 int* LT_Row_weight=new int[lt_len];
 int* LT_Col_weight=new int[ld_len];
 int** LT_Row=new int*[lt_len];
 int** LT_Col=new int*[ld_len];
 int** LT_Row_pos=new int*[lt_len];
 int** LT_Col_pos=new int*[ld_len];


 //------------------------------获取信源---------------------------

//int flag0, flag1;                                      
 //   flag0 = -1;                            //default，默认值
//    rndGen0.SetSeed(flag0);                //设置种子state=17

 //   flag1 = -1;
 //   rndGen1.SetSeed(flag1);                // x=13,y=37,z=91




	
   

/*	 

	//-------------------------encode------------------------------

	C_encode.Encode_ldpc(H,M,N,KK,cc_len,uu,cc);
	C_encode.Encode_LT(cc,cc1,cc_len,lt_len,cc1_len,H2);
	
       int pp=0;


		for(i=0;i<M;i++)              
		{   
			
			int w=0;
			for(j=0;j<cc_len;j++)
			{
				if(cc[j]==1&&H[i][j]==1)
				w++;
				w=w%2;
	
			}
			
			if(w!=0)
				pp++;
		}
		cout<<"码字和校验矩阵相乘结果为"<<pp<<endl;
*/

//--------------------------------------raptor AWGN信道-----------------------------	
  
	   
	 
	     int times;
	
	     SS.GetBitStr(uu,ss_len); 
		 for(i=0;i<ld_len;i++)          //此步骤必不可少
			 c_ld[i]=0;
         C_encode.Encode_ldpc(H,M,N,KK,ld_len,uu,c_ld);
	//	  for(i=0;i<ld_len;i++)          //全0码字译码
	//		 c_ld[i]=0;
		 C_encode.Encode_LT(M,c_ld,c_rap,ld_len,lt_len,H2,H, Row, Row_weight, Row_pos, Col, Col_weight, Col_pos);
		 //cout << "d";
//--------------------------------------------------------------------------
		
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
//-------------------------------------------------------------------------------
			 for(Snr=1.8;Snr<=3.2;Snr+=0.2)
	  {
		 e_num=0;
		 fail_time=0;
		 times=0;
		 sigma=0.5*(double)lt_len/((pow(10.0,(Snr/10))*(double)ss_len));
//		 cout<<"xigema:"<<sigma*sigma<<endl;
		 flag=0;
         while((flag==0)&&(times<TIMES))
		 { 	    
             Gau1.addnoise(Snr,c_rap,c_rap1,ss_len,lt_len);
			 success_num=D_CC.Raptor_AWGN(c_ld, M, ld_len, lt_len, iter_len,c_rap1, H1,sigma,
			   Row,Row_weight,Row_pos,Col,Col_weight,Col_pos,
			   LT_Row,LT_Row_weight,LT_Row_pos,LT_Col,LT_Col_weight,LT_Col_pos,
			    QQ, RR,QQ1,RR1);   
             if(success_num!=ss_len)
		    {
			   fail_time++;
			   e_num=e_num+(ss_len-success_num);
		    }
		   cout<< Snr <<"  "<<"第"<<times+1<<"次进行译码-----成功译码数为"<<success_num+M<<endl;
		   times++;
		   if(fail_time>=50)
			   flag=1;
//		   if(pp==1.5||pp==1.55)
//		   {
//			   if(fail_time>=100)
//				   flag=1;
//		   }
//		   else
//		   {
//			   if(fail_time>=20)
//				   flag=1;
//		   }
		 }
		   wer=(double)fail_time/(double)times;
	       ber=(double)e_num/(times*ss_len);
           ofstream fout("raptor-awgn(5000,4750).txt",ios::app|ios::out);
		
//		   fout<<Snr<<" "<<biterr<<endl;
		   fout<<Snr<<" "<<wer<<" "<<ber<<endl;
		   fout.close();
	  }
	
      
		 //
	 
	
    delete[] H2;
	delete []QQ1;
	delete []RR1;

	delete[] c_rap;
	delete[] c_rap1;

	delete[] LT_Row;
	delete[] LT_Col;
	delete[] LT_Row_weight;
	delete[] LT_Col_weight;
	delete[] LT_Row_pos;
	delete[] LT_Col_pos;




   	delete []QQ;
	delete []RR;
    delete[] Row;
	delete[] Col;
	delete[] Row_weight;
	delete[] Col_weight;
	delete[] Row_pos;
	delete[] Col_pos;
    

	



	delete[] uu;
	delete[] H;
	delete[] H1;
	
	delete[] KK;
	delete[] c_ld;
	
///	delete[] c_rap2;
	
	end=clock();
	cout<<"runnning time is"<<((double)(end-start1)/CLK_TCK)<<"S";
	
	return 0;
	}




