//#include"iostream.h"
class CRandNum  
{
public:
	CRandNum();
	virtual ~CRandNum();

};

//////////////////////////////////////////////////////////////////////
/*
The following generator employs the linear-congruential method,
and specifically uses a choice of multiplier that was proposed
as a standard by Stephen K. Park et al. in "Technical correspondence,"
Communications of the ACM36(1993), number 7, 108-110
*/
/////////////////////////////////////////////////////////////////////
class CLCRandNum  
{
public:
	CLCRandNum();
	virtual ~CLCRandNum();

	void SetSeed(int flag);
	//void PrintState(FILE *fp);
	double Uniform();
	void Normal(double *nn, int len_nn);

private:
    long int state;

    static const int A;
    static const long M;
    static const int Q;
    static const int R;
};

/////////////////////////////////////////////////////////////
//The following generator employs the Wichman-Hill algorithm
/////////////////////////////////////////////////////////////



class CWHRandNum  
{
public:
	CWHRandNum();
	virtual ~CWHRandNum();

	void SetSeed(int flag);
//	void PrintState(FILE *fp);
	double Uniform();
	void Normal(double *nn, int len_nn);

private:
    int X, Y, Z;
};

