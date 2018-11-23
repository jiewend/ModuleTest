#include"RandNum.h"
#include"RandNum1.h"
//#include"Encode.h"
#include"Decode.h"
#include"stdafx.h"
using namespace std;
//extern CWHRandNum rndGen1;   
// RandNum1   rndGen2;


int CDecode::Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int** H1,double sigma,
		            int** Row,int* Row_weight,int** Row_pos,int** Col,int* Col_weight,int** Col_pos,
					int** LT_Row,int* LT_Row_weight,int** LT_Row_pos,int** LT_Col,int* LT_Col_weight,int** LT_Col_pos,
					double** QQ,double** RR,double** QQ1,double** RR1)
{
	int i,j,k,m;                                
	int dd=0;   
	int suc_num=0; 
	double del,t,min;                   	          
	int *ccr;
	ccr=new int[ld_len];                                                      //用译出的码与校验矩阵相乘得出的结果
    double *ff;                                   
	ff=new double[lt_len];                          
    del=0.5*sigma*sigma;
	double *fc;
	fc=new double[ld_len];	
	for(j=0;j<lt_len;j++)                           
	{   
		ff[j]=tanh(0.5*c_rap1[j]/del);  
	}
	double* Q=new double[ld_len];
	double* Q1=new double[ld_len];

	double* LT_Qtemp=new double[1000];             //用于存储一些临时用于计算的信息
	double* LT_Rtemp=new double[1000];
	double* Qtemp=new double[1000];
	double* Rtemp=new double[1000];
	double* Rtemp1=new double[1000];                

	for(i=0;i<lt_len;i++)
	{
		for(j=0;j<LT_Row_weight[i];j++)
		{
			RR1[i][j]=0.0;
		}
	}


	for(i=0;i<ld_len;i++)
	{
		for(j=0;j<LT_Col_weight[i];j++)
		{
			QQ1[i][j]=0;
		}
	}

	for(i=0;i<M;i++)
	{
		for(j=0;j<Row_weight[i];j++)
		{
			RR[i][j]=0;
		}
	}

	for(i=0;i<ld_len;i++)
	{
		for(j=0;j<Col_weight[i];j++)
		{
			QQ[i][j]=0;
		}
	}
	for(i=0;i<ld_len;i++)
		Q1[i]=0;

//------------------------------------开始进行迭代译码-----------------------------

	for(k=0;k<iter;k++)
	{   

		for(i=0;i<ld_len;i++)
			Q[i]=0;
		for(i=0;i<ld_len;i++)                                
		{   
			
			for(j=0;j<LT_Col_weight[i];j++)
			{
				LT_Qtemp[j]=RR1[LT_Col[i][j]][LT_Col_pos[i][j]];
			}
			t=0;
			for(j=0;j<LT_Col_weight[i];j++)
				t+=LT_Qtemp[j];
			for(j=0;j<LT_Col_weight[i];j++)
			{
				QQ1[i][j]=(t-LT_Qtemp[j]+Q1[i]);
			}
		}
		
//------------------------------------------------------------------

		for(i=0;i<lt_len;i++)
		{
			for(j=0;j<LT_Row_weight[i];j++)
			{
				LT_Rtemp[j]=QQ1[LT_Row[i][j]][LT_Row_pos[i][j]];
			}
			for(j=0;j<LT_Row_weight[i];j++)
			{  
			   t=1.0;
			   for(m=0;m<LT_Row_weight[i];m++)
			   {
			    if(m!=j)
				  t=t*tanh(0.5*LT_Rtemp[m]);
			   }
			   RR1[i][j]=log((1+ff[i]*t)/(1-ff[i]*t));
			}
			for(j=0;j<LT_Row_weight[i];j++)
				Q[LT_Row[i][j]]+=RR1[i][j];
		}
	
		
//-----------------------------------------------------
		
		for(i=0;i<ld_len;i++)
		{   
			for(j=0;j<Col_weight[i];j++)
			{
				Qtemp[j]=RR[Col[i][j]][Col_pos[i][j]];
			}
			t=0.0;
			for(j=0;j<Col_weight[i];j++)
				t+=Qtemp[j];
			for(j=0;j<Col_weight[i];j++)
			{
				QQ[i][j]=(t-Qtemp[j]+Q[i]);
			}
		}

//-------------------------------------------------------------------
	
		for(i=0;i<ld_len;i++)
			Q1[i]=0;
		for(i=0;i<M;i++)
		{
			for(j=0;j<Row_weight[i];j++)
			{
				Rtemp[j]=tanh(0.5*QQ[Row[i][j]][Row_pos[i][j]]);
			}
			
			for(j=0;j<Row_weight[i];j++)
			{
				t=1.0;
			   for(m=0;m<Row_weight[i];m++)
			   {
			    if(m!=j)
				  t=t*Rtemp[m];
			   }
			   RR[i][j]=log((1+t)/(1-t));
			}
			for(j=0;j<Row_weight[i];j++)
				Q1[Row[i][j]]+=RR[i][j];
		}

//----------------------------------------------------------min-sum译码------------------	

	/*	
		for(i=0;i<M;i++)
		{
			for(j=0;j<Row_weight[i];j++)
			{
				Rtemp[j]=QQ[Row[i][j]][Row_pos[i][j]];
				Rtemp1[j]=fabs(Rtemp[j]);
	//			cout<<Rtemp[j]<<" ";
			}
			for(j=0;j<Row_weight[i];j++)
			{
				if(j==0)
				{
					min=Rtemp1[1];
					if(Rtemp[1]<0)
						t=-1;
					else
						t=1;
			        for(m=2;m<Row_weight[i];m++)
					{
						if(Rtemp1[m]<min)
							min=Rtemp1[m];
						if(Rtemp[m]<0)
							t=t*(-1);
					}
				
				}
				else
				{
					min=Rtemp1[0];
					if(Rtemp[0]<0)
						t=-1;
					else
						t=1;
					 for(m=1;m<Row_weight[i];m++)
					{
						if(m!=j)
						{
						  if(Rtemp1[m]<min)
							  min=Rtemp1[m];
						  if(Rtemp[m]<0)
							t=t*(-1);
						}
					}
					
				}
				RR[i][j]=min*t;
			}
		}
*/
//-----------------------------------------判决------------------------

		for(i=0;i<ld_len;i++)
		{
			fc[i]=Q[i]+Q1[i];
			if(fc[i]>0)
				ccr[i]=0;
			else
				ccr[i]=1;
		}

		suc_num=0;
		for(i=0;i<ld_len;i++)
		{
			if(ccr[i]==c_ld[i])
				suc_num++;
		}
		
//		cout<<"第"<<k+1<<"次迭代译码正确数为"<<suc_num<<endl;
		if(suc_num==ld_len)
			break;
	}


   
	delete []fc;
	delete []ff;
	delete []Q;
	delete []Q1;
	delete []Qtemp;
	delete []Rtemp;
	delete []Rtemp1;
	delete []LT_Qtemp;
	delete []LT_Rtemp;




	return suc_num;
	}



		
				

     
				

				




	


	



























       