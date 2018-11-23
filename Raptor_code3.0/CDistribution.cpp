#include"stdafx.h"
#include"RandNum.h"
#include"CDistribution.h"
#include "RandNum1.h"
//extern CWHRandNum rndGen1;
 RandNum1     rndGen2;
int CDistribution::Robust_D(int len,double c,double p)
{   
	
	//srand( (unsigned)time( NULL ) );
    
	double rr_num;
	int i,j;
	int d;
	double min=0.0,max=0.0;
	double R;
	double B=0.0;
	                                               //double min,max;
	double*Ip;
	double*TT;

	Ip=new double[len];
	TT=new double[len];
    R = c*log(double(len)/p)*sqrt(double(len));

	Ip[0]=1/double(len);                         //理想孤波分布的度概率
	for(i=2;i<=len;i++)
		Ip[i-1]=1/double(i*(i-1));

	for(i=0;i<len;i++)                           //鲁棒孤波分布参数的计算                      
	{
		j=i+1;
		if(j<=int(double(len)/R-1.0))
			TT[i]=R/double(j*len);
		else
		{
		if(j==int(double(len)/R))
			TT[i]=R*log(R/p)/double(len);
		else
			TT[i]=0;
		}
		B+=Ip[i]+TT[i];
	}

	//rr_num=rndGen1.Uniform();                  //产生一个0~1的随机数，看随机数落在哪个度区间
    rr_num=rndGen2.Randomdouble(0,1);
	if(rr_num<=(Ip[0]+TT[0])/B)
		d=1;
	else
	{
	    for(i=2;i<=len;i++)
		{
		   min+=(Ip[i-2]+TT[i-2])/B;
	       max=min+(Ip[i-1]+TT[i-1])/B;
		   if(rr_num>min&&rr_num<max)
		   {
			     d=i;
			     break;
		   }
		}
	}
	delete[] Ip;
    delete[] TT;
//	cout<<d<<" ";
	return d;
}





int CDistribution::Ideal_D(int len)
{
	double rr_num;
    //int i;
	int d;
	//double min,max;
    
//	rr_num=rndGen1.Uniform ();
    rr_num=rndGen2.Randomdouble(0,1);
	if(rr_num<1/double(len))d=1;
	else
		d=int(1.0/(1.0+1.0/double(len)-rr_num))+1;
		/*for(i=2;i<=len;i++)
		{
			min=1/double(len)+1-1/double(i-1);
		    max=1/double(len)+1-1/double(i);
			if(rr_num>min&&rr_num<max)
			{
				d=i;
				break;
			}
		}*/
		return d;
}

int CDistribution::new_D()
{   
    
	int aa[10]={797,50154,66776,74041,82297,87903,91626,97185,99687,100000};
	int bb[10]={1,2,3,4,5,8,9,19,65,66};
	int num=rndGen2.RandomInt(1,100000);
	int t=0;
	while(num>aa[t])
	{
		t++;
	}
	return bb[t];
}