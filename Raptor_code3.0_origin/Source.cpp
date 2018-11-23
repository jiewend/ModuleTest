// SourceSink.cpp: implementation of the CSourceSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Source.h"

#include "RandNum.h"
#include "RandNum1.h"

extern CLCRandNum rndGen0;
//extern CWHRandNum rndGen1;
 RandNum1   rndGen3;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSource::CSource()
{

}

CSource::~CSource()
{

}

/////////////////////////////////////////////////////////////////////////////
/*
void CSource::GetSymStr(int *uu, int q_ary, int len)
{
	int t; 

	for (t = 0; t < len; t++)
	 	uu[t] = (int) (rndGen1.Uniform()*q_ary);       //*是乘号的意思
//	    uu[t] = 0;
	
	return;
}
*/
/////////////////////////////////////////////////////////////////////////////
void CSource::GetBitStr(int *uu, int len)
{
    int t;
	for (t = 0; t < len; t++)
	{  
		
		uu[t] = (rndGen3.Randomdouble(0,1)< 0.5?0:1);
		
	}
	//cout<<endl;
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
/*
void CSource::GetSparseStr(int *uu, double p, int len)
{
	int t; 

	for (t = 0; t < len; t++)
		uu[t] = (rndGen1.Uniform() < p?0:1);
//	    uu[t] = 0;
	
	return;
}
*/
//////////////////////////////////////////////////////////////////////////////////
void CSource::ClrCnt()
{
	m_num_tot_blk = 0;
	m_num_tot_sym = 0;
	m_num_err_blk = 0;
	m_num_err_sym = 0;
	
	return;
}

//////////////////////////////////////////////
void CSource::CntErr(int *uu, int *uu_hat, int len, int flag)        //计算错误
{
	int t;

	m_temp_err = 0;
	for (t = 0; t < len; t++){
		if (uu_hat[t] != uu[t])
			m_temp_err++;
	}

	if (flag == 1){
		if (m_temp_err > 0){
			m_num_err_sym += m_temp_err;
			m_num_err_blk += 1;
		}
		
		m_num_tot_blk += 1.0;
		m_num_tot_sym += len;

		m_ser = m_num_err_sym / m_num_tot_sym;
		m_fer = m_num_err_blk / m_num_tot_blk;
	}

	return;
}

