#include"Encode.h"
#include"stdafx.h"
#include"CDistribution.h"
#include "RandNum1.h"
const double Constant=0.03;
const double P=0.5;
using namespace std;


void CEncode::Encode_ldpc(int **a,int m,int n ,int *kk,int cc_len,int *uu,int *cc)
{
	int i=0,j=0,v=0;
	int t,r,s,p;
	for(int c=0;c<m;c++)
	 kk[c]=0;
//	for(i=0;i<cc_len;i++)
//		cc[i]=0;
	while(i<m&&j<n)
	{
		if(a[i][j]==0)
		{
			for(t=i+1;t<m;t++)
			{
				if(a[t][j]==1)
				{
					for(s=0;s<n;s++)
					{
						p=a[i][s];
                        a[i][s]=a[t][s];
                        a[t][s]=p;
					}
					break;
				}
			}
		}

		if(a[i][j]==0)
		{
			j++;
			continue;
		}

		for(r=i+1;r<m;r++)
		{
			if(a[r][j]==1)
				for(s=0;s<n;s++)
				{
					a[r][s]+=a[i][s];
                    a[r][s]=a[r][s]%2;
				}
		}

        if(i>=1)
		{
			for(r=i-1;r>=0;r--)
			{
			if(a[r][j]==1)
				for(s=j;s<n;s++)
				{
					a[r][s]+=a[i][s];
					a[r][s]=a[r][s]%2;
				}
			}
		}

		kk[v++]=j;
		i++;
		j++;
	}
 
    int	k=0;
	v=0;
	for(j=0;j<cc_len;j++)
	{
		if(j!=kk[v])                  //�ж��Ƿ�����У����
		{
			cc[j]=uu[k];
		//	i++;
		//	k++;
//			for(int d=0;d<(m-1);d++)             //500*10000ʱ
           for(int d=0;d<m;d++)
			{   
				
				if(a[d][j]==1)
				{
				cc[kk[d]]=cc[kk[d]]+uu[k];         //�˴���k��ʾ��k����Ϣλ
				cc[kk[d]]=cc[kk[d]]%2;
				}
				
			}
			k++;
		}
		else
			v++;
	}
	
//   for(i=30;i<31;i++)
//   {
//	 for(j=0;j<1008;j++)
//	 {
//		 cout<<a[i][j];
	//	 if(i==j&&a[j][j]!=1)
	//   cout<<"���ִ���"<<i<<endl<<j;
	 
//	 }
  //  cout<<endl;
//  }
 //   for(i=0;i<m;i++)
//		cout<<kk[i]<<" ";
//	cout<<endl;
  

}

   

	void CEncode::Encode_LT (int M,int *c_ld,int *c_rap,int ld_len,int lt_len,int **HH,int **HH_LDPC, int** Row, int* Row_weight, int** Row_pos, int** Col, int* Col_weight, int** Col_pos)
{  
	int *FLAG;
	int i,j,m=0;
	int k;
	int d_d;
	int k_l;
   
    for(i=0;i<lt_len;i++)
		for(j=0;j<ld_len;j++)
			HH[i][j]=0;
    for(i=0;i<lt_len;i++)
		c_rap[i]=0;
	CDistribution D_D;
	 RandNum1     rndGen5;
    
    
	for(i=0;i<lt_len;i++)
	{   
		
		FLAG=new int[ld_len];
		d_d=D_D.new_D();
//		d_d=D_D.Robust_D(ld_len,0.03,0.5);
		j=d_d;
//	    cout<<d_d;

        int *cho_out=new int[j*60*3];
		for (int ij = 0;ij<j * 60*3;ij++)
			cho_out[ij] = -1;
		int count = 0;
		
		while(j>0)
		{
			

			int aa = 1;
			//	k=rand()%ld_len;
			k=rndGen5.RandomInt(0,ld_len-1);
			k = rndGen5.RandomInt(0, ld_len - 1);
			if(FLAG[k]==1)
				continue;                     //ֱ���������ѭ��ִ����һ��ѭ��
			if ((d_d!= 1)&&(j!=d_d)&&(d_d!=2)&&(d_d != 3))
			{

                     for (int ii = 0;ii < Col_weight[k_l];ii++)
			             for (int jj = 0;jj < Row_weight[Col[k_l][ii]];jj++)
				         {
							 cho_out[count] = Row[Col[k_l][ii]][jj];
					         count++;
				         }
					 for (int ij = 0;ij<count;ij++)
						// cout<<d_d<<" "<<k<<" "<<count<<" "<<cho_out[ij]<<endl;

					 while (aa)
					 {
						 //cout << aa;
						 aa = 0;
						 for (int kk = 0;kk < count;kk++)
						 {
							 if (k == cho_out[kk])
							 {
								 aa = 1;
								 k = rndGen5.RandomInt(0, ld_len - 1);
								 break;
							 }
						 }
					         
					 }

		    }
			
		    k_l = k; 

			c_rap[i]+=c_ld[k];
			c_rap[i]=(int)c_rap[i]%2;
			j--;
			FLAG[k]=1;
			HH[i][k]=1;
			
		}
        delete []cho_out;
		delete []FLAG;
		
	}
 
}
	

	
	
	
	
	
	
	
	
	


	



