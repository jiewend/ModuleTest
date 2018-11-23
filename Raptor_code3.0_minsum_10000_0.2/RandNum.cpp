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
			//unsigned 为强制类型转换运算符 转换得到需要的类型 原变量类型并不改变
			                              
			//srand函数取unsigned int值作参数。srand( seed );srand函数的函数原型
			                             
			//在头文件<stdlib.h>新C++标准的cstdlib)中。如果不想每次输入种子值而随机化，
			                            
			//则要用如下语句：srand( time ( O ) )使计算机通过时钟值自动取得种子值。
			                            
			//time函数(上述语句中该函数的参数为0)返回当前“日历时间”的秒数，将这个
			                          
			//值转换为无符号整数值，作为随机数产生器的种子。time函数的函数原型在
			                          
			//<time>(新C++标准的ctime)中。srand函数取一个unsigned类型的整数参数并
			                        
			//内嵌rand函数(即种子)，就可以在每次执行程序时产生不同的随机数系列。
            
			state = rand();
			//rand函数产生O到RAND_MAX之间的整数(这是<stdlib.h>头文件中定义的符号常量)。
			               
			//RAND_MAX的值至少应为32767，也就是两个字节(即16位)所能表示的最大整数值
		}
	}
	else 
	{
//		fprintf(stdout, "\nEnter the initial state: ");        因为在release下报错，所以先加上注释
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
void CLCRandNum::Normal(double *nn, int len_nn)//正态分布
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
/*基本的构思是，如果您在 [0,1] 中取出三个随机数并将它们加总，
则总和的小数部分本身就是 [0,1] 中的随机数。
Wichman 和 Hill 原文中列出的 Fortran 代码关键语句是：
C	IX, IY, IZ SHOULD BE SET TO INTEGER VALUES BETWEEN 1 AND 30000 BEFORE FIRST ENTRY

IX = MOD(171 * IX, 30269)

IY = MOD(172 * IY, 30307)

IZ = MOD(170 * IZ, 30323)

RANDOM = AMOD(double(IX) / 30269.0 + double(IY) / 30307.0 + double(IZ) / 30323.0, 1.0)
因此，IX、IY、IZ 分别生成介于 0 和 30268、0 和 30306 以及 0 和 30322 之间的整数。
它们在最后一个语句中组合起来，以实现前面表述的简单原理：
如果您在 [0,1] 中取出三个随机数并将它们加总，则总和的小数部分本身就是 [0,1] 中的随机数。

由于 RAND 产生伪随机数，因此，如果产生序列很长的伪随机数，则序列本身最终将会重复。
如 Wichman-Hill 过程那样组合随机数可保证在开始重复之前生成超过 10^13 个数字。
早期版本的 RAND 在几项 Diehard 测试中的结果不尽如人意，
原因是在数字开始重复之前所经过的周期短得不能让人接受。*/
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
//		fprintf(stdout, "\nEnter the initial state (X Y Z): ");       因为在release下报错，所以先加上注释
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

	U = X / 30269.0 + Y / 30307.0 + Z / 30323.0;//三个数相加取小数部分
	U = U - int(U);

    return U;
}

///////////////////////////////////////////////////////////////
void CWHRandNum::Normal(double *nn, int len_nn)//正态分布
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