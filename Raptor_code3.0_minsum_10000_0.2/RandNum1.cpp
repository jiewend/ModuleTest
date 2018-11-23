
#include "stdafx.h"
#include "RandNum1.h"

//-----------------  Parameters for generating Gaussian random variables -------------------
#define FALSE 0
#define TRUE  1
double  uuu[97],ccc,ccdd,ccmm;
int     i97,j97;
int     test_Gaussian = FALSE;




RandNum1::RandNum1()
{

}

RandNum1::~RandNum1()
{

}

//------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////
// Some functions
////////////////////////////////////////////////////////////////////////////////////////////////
//=============================================================================================
void RandNum1::RandomInitialise(int ij,int kl)
{
   double s,t;
   int ii,i,j,k,l,jj,m;

   /*
      Handle the seed range errors
         First random number seed must be between 0 and 31328
         Second seed must have a value between 0 and 30081
   */
   if (ij < 0 || ij > 31328 || kl < 0 || kl > 30081) {
		ij = 1802;
		kl = 9373;
   }

   i = (ij / 177) % 177 + 2;
   j = (ij % 177)       + 2;
   k = (kl / 169) % 178 + 1;
   l = (kl % 169);

   for (ii=0; ii<97; ii++) {
      s = 0.0;
      t = 0.5;
      for (jj=0; jj<24; jj++) {
         m = (((i * j) % 179) * k) % 179;
         i = j;
         j = k;
         k = m;
         l = (53 * l + 1) % 169;
         if (((l * m % 64)) >= 32)
            s += t;
         t *= 0.5;
      }
      uuu[ii] = s;
   }

   ccc    = 362436.0 / 16777216.0;
   ccdd   = 7654321.0 / 16777216.0;
   ccmm   = 16777213.0 / 16777216.0;
   i97  = 97;
   j97  = 33;
   test_Gaussian = TRUE;
}

/* 
   This is the random number generator proposed by George Marsaglia in
   Florida State University Report: FSU-SCRI-87-50
*/
double RandNum1::RandomUniform(int ij, int kl)
{
   double uni;

   /* Make sure the initialisation routine has been called */
   if (!test_Gaussian) 
   	 RandomInitialise(ij,kl);   // Original test case: RandomInitialise(1802,9373);

   uni = uuu[i97-1] - uuu[j97-1];
   if (uni <= 0.0)
      uni++;
   uuu[i97-1] = uni;
   i97--;
   if (i97 == 0)
      i97 = 97;
   j97--;
   if (j97 == 0)
      j97 = 97;
   ccc -= ccdd;
   if (ccc < 0.0)
      ccc += ccmm;
   uni -= ccc;
   if (uni < 0.0)
      uni++;

   return(uni);
}

/*
  ALGORITHM 712, COLLECTED ALGORITHMS FROM ACM.
  THIS WORK PUBLISHED IN TRANSACTIONS ON MATHEMATICAL SOFTWARE,
  VOL. 18, NO. 4, DECEMBER, 1992, PP. 434-435.
  The function returns a normally distributed pseudo-random number
  with a given mean and standard devaiation.  Calls are made to a
  function subprogram which must return independent random
  numbers uniform in the interval (0,1).
  The algorithm uses the ratio of uniforms method of A.J. Kinderman
  and J.F. Monahan augmented with quadratic bounding curves.
*/
double RandNum1::RandomGaussian(double mean,double stddev, int ij, int kl)
{
   double  q,u,v,x,y;

	/*  
		Generate P = (u,v) uniform in rect. enclosing acceptance region 
      Make sure that any random numbers <= 0 are rejected, since
      gaussian() requires uniforms > 0, but RandomUniform() delivers >= 0.
	*/
   do {
      u = RandomUniform(ij,kl);
      v = RandomUniform(ij,kl);
   	if (u <= 0.0 || v <= 0.0) {
       	u = 1.0;
       	v = 1.0;
   	}
      v = 1.7156 * (v - 0.5);

      /*  Evaluate the quadratic form */
      x = u - 0.449871;
   	y = fabs(v) + 0.386595;
      q = x * x + y * (0.19600 * y - 0.25472 * x);

      /* Accept P if inside inner ellipse */
      if (q < 0.27597)
			break;

      /*  Reject P if outside outer ellipse, or outside acceptance region */
    } while ((q > 0.27846) || (v * v > -4.0 * log(u) * u * u));

    /*  Return ratio of P's coordinates as the normal deviate */
    return (mean + stddev * v / u);
}

/*
   Return random integer within a range, lower -> upper INCLUSIVE
*/
int RandNum1::RandomInt(int lower,int upper)
{
	int ij,kl;
    ij=rand();
	while(ij < 0 || ij > 31328)
        ij = rand();
	kl = rand();
	while(kl < 0 || kl > 30081||(kl==ij))
       kl = rand();
   return((int)(RandomUniform(ij,kl) * (upper - lower + 1)) + lower);
}

/*
   Return random double within a range, lower -> upper
*/
double RandNum1::Randomdouble(double lower,double upper)
{  
	int ij,kl;
    ij=rand();
	while(ij < 0 || ij > 31328)
        ij = rand();
	kl = rand();
	while(kl < 0 || kl > 30081||(kl==ij))
       kl = rand();
   return((upper - lower) * RandomUniform(ij,kl) + lower);
}

// the function to generate a random distribution of nodes 
// on a 100X100 plane;

//=============================================================================================

