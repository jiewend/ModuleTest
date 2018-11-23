// RandNum.cpp: implementation of the CRandNum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RandNum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandNum::CRandNum()
{

}

CRandNum::~CRandNum()
{

}


//////////////////////////////////////////////////////////////////////
/*
The following generator employs the linear-congruential method,
and specifically uses a choice of multiplier that was proposed
as a standard by Stephen K. Park et al. in "Technical correspondence,"
Communications of the ACM36(1993), number 7, 108-110
*/
/////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLCRandNum::CLCRandNum()
{

}

CLCRandNum::~CLCRandNum()
{

}

///////////////////////////////////////////////////////////////
const int CLCRandNum::A = 48271;
const long CLCRandNum::M = 2147483647;
const int CLCRandNum::Q = M / A;
const int CLCRandNum::R = M % A;

////////////////////////////////////////////////////////////////
void CLCRandNum::SetSeed(int flag)
{
	if (flag < 0)
		state = 17;
	else if (flag == 0)
	{
		state = 0;
		while (state == 0)
		{
			srand((unsigned)time(NULL));  
			//unsigned Ϊǿ������ת������� ת���õ���Ҫ������ ԭ�������Ͳ����ı�
			                              
			//srand����ȡunsigned intֵ��������srand( seed );srand�����ĺ���ԭ��
			                             
			//��ͷ�ļ�<stdlib.h>��C++��׼��cstdlib)�С��������ÿ����������ֵ���������
			                            
			//��Ҫ��������䣺srand( time ( O ) )ʹ�����ͨ��ʱ��ֵ�Զ�ȡ������ֵ��
			                            
			//time����(��������иú����Ĳ���Ϊ0)���ص�ǰ������ʱ�䡱�������������
			                          
			//ֵת��Ϊ�޷�������ֵ����Ϊ����������������ӡ�time�����ĺ���ԭ����
			                          
			//<time>(��C++��׼��ctime)�С�srand����ȡһ��unsigned���͵�����������
			                        
			//��Ƕrand����(������)���Ϳ�����ÿ��ִ�г���ʱ������ͬ�������ϵ�С�
            
			state = rand();
			//rand��������O��RAND_MAX֮�������(����<stdlib.h>ͷ�ļ��ж���ķ��ų���)��
			               
			//RAND_MAX��ֵ����ӦΪ32767��Ҳ���������ֽ�(��16λ)���ܱ�ʾ���������ֵ
		}
	}
	else 
	{
//		fprintf(stdout, "\nEnter the initial state: ");        ��Ϊ��release�±��������ȼ���ע��
//		fscanf(stdin, "%ld", &state);
	}

	return;
}

///////////////////////////////////////////////////////////////
/*void CLCRandNum::PrintState(FILE *fp)
{
	fprintf(fp, "\n***init_state = %ld***\n", state);

	return;
}*/
///////////////////////////////////////////////////////////////
double CLCRandNum::Uniform()
{
	double u;
	
	int tmpState = A * ( state % Q ) - R * ( state / Q );
	if ( tmpState >= 0)
		state = tmpState;
	else
		state = tmpState + M;

	u = state / (double)M;

    return u;
}

///////////////////////////////////////////////////////////////
void CLCRandNum::Normal(double *nn, int len_nn)//��̬�ֲ�
{
	double x1, x2, w;
	int t;

	for (t = 0; 2*t + 1 < len_nn; t++){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[2*t] = x1 * w;
		nn[2*t+1] = x2 * w;
	}

	if (len_nn % 2 == 1){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[len_nn-1] = x1 * w;
	}

	return;
}


/////////////////////////////////////////////////////////////
//The following generator employs the Wichman-Hill algorithm
/////////////////////////////////////////////////////////////
/*�����Ĺ�˼�ǣ�������� [0,1] ��ȡ������������������Ǽ��ܣ�
���ܺ͵�С�����ֱ������ [0,1] �е��������
Wichman �� Hill ԭ�����г��� Fortran ����ؼ�����ǣ�
C	IX, IY, IZ SHOULD BE SET TO INTEGER VALUES BETWEEN 1 AND 30000 BEFORE FIRST ENTRY

IX = MOD(171 * IX, 30269)

IY = MOD(172 * IY, 30307)

IZ = MOD(170 * IZ, 30323)

RANDOM = AMOD(double(IX) / 30269.0 + double(IY) / 30307.0 + double(IZ) / 30323.0, 1.0)
��ˣ�IX��IY��IZ �ֱ����ɽ��� 0 �� 30268��0 �� 30306 �Լ� 0 �� 30322 ֮���������
���������һ������������������ʵ��ǰ������ļ�ԭ��
������� [0,1] ��ȡ������������������Ǽ��ܣ����ܺ͵�С�����ֱ������ [0,1] �е��������

���� RAND ����α���������ˣ�����������кܳ���α������������б������ս����ظ���
�� Wichman-Hill �����������������ɱ�֤�ڿ�ʼ�ظ�֮ǰ���ɳ��� 10^13 �����֡�
���ڰ汾�� RAND �ڼ��� Diehard �����еĽ�����������⣬
ԭ���������ֿ�ʼ�ظ�֮ǰ�����������ڶ̵ò������˽��ܡ�*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWHRandNum::CWHRandNum()
{

}

CWHRandNum::~CWHRandNum()
{

}

//////////////////////////////////////////////////////////////////////
void CWHRandNum::SetSeed(int flag)
{
	if (flag < 0)
	{
		X = 13;
		Y = 37;
		Z = 91;
	}
	else if (flag == 0)
	{
		X = 0;
		Y = 0;
		Z = 0;
		while (X == 0 || Y == 0 || Z == 0)
		{
			srand((unsigned)time(NULL));
			X = rand();
			Y = rand();
			Z = rand();
		}
	}
	else 
	{
//		fprintf(stdout, "\nEnter the initial state (X Y Z): ");       ��Ϊ��release�±��������ȼ���ע��
//		fscanf(stdin, "%d %d %d", &X, &Y, &Z);
	}
	
	return;
}

///////////////////////////////////////////////////////////////
/*void CWHRandNum::PrintState(FILE *fp)
{
	fprintf(fp, "\n***init_state (X Y Z) = %d %d %d***\n", X, Y, Z);

	return;
}*/
///////////////////////////////////////////////////////////////
double CWHRandNum::Uniform()
{
	double U;
	
	X =	171 * X % 30269;
	Y = 172 * Y % 30307;
	Z = 170 * Z % 30323;

	U = X / 30269.0 + Y / 30307.0 + Z / 30323.0;//���������ȡС������
	U = U - int(U);

    return U;
}

///////////////////////////////////////////////////////////////
void CWHRandNum::Normal(double *nn, int len_nn)//��̬�ֲ�
{
	double x1, x2, w;
	int t;

	for (t = 0; 2*t + 1 < len_nn; t++){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[2*t] = x1 * w;
		nn[2*t+1] = x2 * w;
	}

	if (len_nn % 2 == 1){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[len_nn-1] = x1 * w;
	}

	return;
}