#include "stdafx.h"
//#include "RandNum.h"
#include "RandNum1.h"
#include "GauChannel.h"
#define GAUSS_RANDOM       1
using namespace std;
/*

Gau_Channel::Gau_Channel()
{

}

Gau_Channel::~Gau_Channel()
{

}
*/                                
void Gau_Channel::addnoise(double Snr ,int *cc_array,double *ccc,int kkk,int nnn)
{   
	double ww;
	for(int i=0;i<nnn;i++)
	{
		if(cc_array[i]==0)
			ccc[i]=1;
		else
			ccc[i]=-1;
	}
	int t;
    double mean, Es, sigma, N0, Rate;
	Rate = (double)kkk/(double)nnn;
	mean = 0;
    Es = 1;
	N0 = Es/((double) ( pow(10.0, (Snr / 10)) * Rate ));
    sigma =  (double) sqrt (N0/2);
	RandNum1  rand1;
	int GAUSS_Ini_ij, GAUSS_Ini_kl;

	if (GAUSS_RANDOM==0)
	{
		GAUSS_Ini_ij = 1802;
		GAUSS_Ini_kl = 9373;
	}
	else
	{
	    srand( (unsigned)time( NULL ) );

        GAUSS_Ini_ij = rand();
		while(GAUSS_Ini_ij < 0 || GAUSS_Ini_ij > 31328)
            GAUSS_Ini_ij = rand();
	//	std::cout<<"ÖÖ×ÓÎª:"<<GAUSS_Ini_ij<<endl;
		GAUSS_Ini_kl = rand();
		while(GAUSS_Ini_kl < 0 || GAUSS_Ini_kl > 30081||(GAUSS_Ini_kl==GAUSS_Ini_ij))
            GAUSS_Ini_kl = rand();
       // GAUSS_Ini_ij=rand1.RandomInt(0,31328);
	//	GAUSS_Ini_kl = rand1.RandomInt(0,31328);

	}

		
    for (t = 0; t < nnn; t++) 
	{    ww=rand1.RandomGaussian(mean, sigma, GAUSS_Ini_ij, GAUSS_Ini_kl);     
        //ccc[t]=ccc[t] + rand1.RandomGaussian(mean, sigma, GAUSS_Ini_ij, GAUSS_Ini_kl);
	    ccc[t]=ccc[t]+ww;	
		//std::cout<<ww<<" ";
    }
}

