#include"RandNum.h"
#include"RandNum1.h"
//#include"Encode.h"
#include"Decode.h"
#include"stdafx.h"
using namespace std;
//extern CWHRandNum rndGen1;   
// RandNum1   rndGen2;



int CDecode::Decode_Gau(int cc_len,int *cc,int mm,int iter,double *ccc,int *uu,int **HH,double **QQ,double **RR,double sigma)
{                                                                                    //AWGN信道下的译码函数
	int i,j,k;                                
	int dd=0;   
	double del;
                    //QQ和RR都为二维矩阵，用于存放迭代时的传递参数Qij和Rij，sigma为噪声标准差
                      //uu为初始码元，用于最后对比计算译码正确个数用的
    double *QS;
	QS=new double[cc_len];
	for(j=0;j<cc_len;j++)
		QS[j]=0;
	int suc_num=0;             //用于返回成功译码的码元个数
	double *Q;                 //每次迭代完成后计算Q[i]进行判决,若Q[i]>0,则判为0，否则判为1
	Q=new double[cc_len];
	int *ccr;
	ccr=new int[cc_len];                          //存放判决后的码
	int nn;                                       //用译出的码与校验矩阵相乘得出的结果
    double *ff;                                   
	ff=new double[cc_len];                          
//	for( i=0;i<bm_len;i++)
//	 for(j=0;j<cc_len;j++)
//	 { 
//		 cout<<HH[i][j];
//	 }

    
    del=0.5*sigma*sigma;
	for(j=0;j<cc_len;j++)                           //对Qij进行初始化
	{   
		ff[j]=ccc[j]/del;   
		for(i=0;i<mm;i++)
		{
			if(HH[i][j]==1)
				QQ[i][j]=ff[j];
			else
				QQ[i][j]=0;
		}
	//	cout<<ff[j]<<" ";
	}
    suc_num=0;
	for(i=0;i<cc_len;i++)              //计算译码成功的码元数量  
	{   
		if(ff[i]<0)
				ccr[i]=1;
			else
				ccr[i]=0;
	    if(ccr[i]==cc[i])
		suc_num++;		
	}   
	cout<<"迭代前译码正确为："<<suc_num<<endl;

	for(k=0;k<iter;k++)
	{
       for(i=0;i<mm;i++)
	   {   
		   double t=1;
		   for(j=0;j<cc_len;j++)
		   {   
			   if(HH[i][j]==1)                 //根据校验矩阵里面元素是否为1来判断码是否和校验节点相连，从而计算Rij
			   {       QS[j]=tanh(0.5*QQ[i][j]);
				       t=t*QS[j];		 //此处t的值已经非常小，基本等于0
			   }
			 //  cout<<t<<" ";
		   }
		   
		   for(j=0;j<cc_len;j++)
		   {
			   if(HH[i][j]==1)
			   {                 
				   RR[i][j]=log((1+t/QS[j])/(1-t/QS[j])) ;            //计算求得的RR[i][j]全都为0，因此迭代基本无效

			   }
			   else
				   RR[i][j]=0;
			      
		   }
	   }
   //     cout<<"输出R为"<<endl;
//	  for(i=0;i<bm_len;i++)
	//	   for(j=0;j<cc_len;j++)
	//	   cout<<RR[i][j]<<" ";

        for(j=0;j<cc_len;j++)
		{   
			double q=0;
			for(int t=0;t<mm;t++)
				{
					if(HH[t][j]==1)
						  q=q+RR[t][j];
				}
			Q[j]=ff[j]+q;                     //计算Q[i]用于每次迭代结束后对码元做硬判决；
			for(i=0;i<mm;i++)             //计算传递参数Qij；
			{
				if(HH[i][j]==1)
				{
				
					  if(HH[i][j]==1)
				      QQ[i][j]=Q[j]-RR[i][j];
				}
				else
					QQ[i][j]=0;

			}
		}
      

		for(j=0;j<cc_len;j++)               //每次迭代后进行判决
		{
			if(Q[j]<0)
				ccr[j]=1;
			else
				ccr[j]=0;
		}
		
        
		dd++;                              //计算迭代了多少次
	
        nn=0;


		for(i=0;i<mm;i++)              //译码序列与校验矩阵的转置进行相乘
		{   
			
			int w=0;
			for(j=0;j<cc_len;j++)
			{
				if(ccr[j]==1&&HH[i][j]==1)
				w++;
				w=w%2;
	
			}
			
			if(w!=0)
				nn++;
		}
		suc_num=0;
		for(i=0;i<cc_len;i++)              //计算译码成功的码元数量  
	{   
	    if(cc[i]==ccr[i])
		suc_num++;			
	}   
	cout<<"第"<<dd<<"次"<<"成功译码数量为:"<<suc_num<<endl;
		
		if(nn==0)                     //判断C*（H的转置）是否为0,为0就停止迭代
			break;
	//	cout<<endl;
     //  for(j=670;j<710;j++)
	//	   cout<<ccr[j]<<" ";
	}
    cout<<"nn="<<nn<<endl;
	cout<<"迭代次数为"<<dd<<endl;
    suc_num=0;
	for(i=0;i<cc_len;i++)              //计算译码成功的码元数量  
	{   
	    if(cc[i]==ccr[i])
		suc_num++;
			
	}   

	
	
	
	return suc_num;      //返回译码成功数量
	
	delete []Q;
	delete []ccr;
	delete []ff;
}









int CDecode::Decode_Bsc(int cc_len,int mm,int iter,int *cc,int *uu,int **HH,double **QQ,double **RR,double qq)
{
	int i,j,k;                              
	int dd=0;
	double *QS;
    QS=new double[cc_len];
	for(j=0;j<cc_len;j++)
		QS[j]=0;                 
                      

	int suc_num=0;            
	double *Q;                 
	Q=new double[cc_len];
	int *ccr;
	ccr=new int[cc_len];                          
	int nn;                                       
    double *ff;                                   
	ff=new double[cc_len];                          
	

	for(j=0;j<cc_len;j++)                          
	{   
		if(cc[j]==1)
			ff[j]=log(qq)-log(1-qq);
		else
			ff[j]=log(1-qq)-log(qq);
		for(i=0;i<mm;i++)
		{
			if(HH[i][j]==1)
				QQ[i][j]=ff[j];
			else
				QQ[i][j]=0;
		}
	//	cout<<ff[j]<<" ";
	}

	for(k=0;k<iter;k++)
	{
       for(i=0;i<mm;i++)
	   {   
		   double t=1;
		   for(j=0;j<cc_len;j++)
		   {   
			   if(HH[i][j]==1)                
				{ 
			      
					 QS[j]=tanh(0.5*QQ[i][j]);
				       t=t*QS[j];	
					   
			   }
		   }
		   for(j=0;j<cc_len;j++)
			   if(HH[i][j]==1)
			   {   
				   
				   RR[i][j]=log((1+t/QS[j])/(1-t/QS[j])) ;
			
			   }
			   else
				   RR[i][j]=0;
			      
	   }


        for(j=0;j<cc_len;j++)
		{   
			double q=0;
			for(int t=0;t<mm;t++)
				{
					if(HH[t][j]==1)
						  q=q+RR[t][j];
				}
			Q[j]=ff[j]+q;                     
			for(i=0;i<mm;i++)             
			{
				if(HH[i][j]==1)
				{
				
					  if(HH[i][j]==1)
				      QQ[i][j]=Q[j]-RR[i][j];
				//	  cout<<QQ[i][j]<<" ";
				}
				else
					QQ[i][j]=0;

			}
		}
      
        
		for(j=0;j<cc_len;j++)               //每次迭代后进行判决
		{
			if(Q[j]>0)
				ccr[j]=0;
			else
				ccr[j]=1;
			
		}
		

		dd++;                              //计算迭代了多少次
	
        nn=0;


		for(i=0;i<mm;i++)              //译码序列与校验矩阵的转置进行相乘
		{   
			
			int w=0;
			for(j=0;j<cc_len;j++)
			{
				if(ccr[j]==1&&HH[i][j]==1)
				w++;
				w=w%2;
	
			}
			
			if(w!=0)
				nn++;
		}
	  for(i=0;i<(cc_len-mm);i++)              //计算译码成功的码元数量  
	{   
	    if(uu[i]==ccr[mm+i])
		suc_num++;		
	}   

	cout<<"第"<<dd<<"次"<<"成功译码数量为:"<<suc_num<<endl;
		
		if(nn==0)                     //判断C*（H的转置）是否为0,为0就停止迭代
			break;
		 
	  
	}
    cout<<"nn="<<nn<<endl;
	cout<<"迭代次数为"<<dd<<endl;
    suc_num=0;
	for(i=0;i<(cc_len-mm);i++)              //计算译码成功的码元数量  
	{   
	    if(uu[i]==ccr[mm+i])
		suc_num++;
	}                    
    cout<<"成功译码数量为"<<suc_num<<endl;
	
	return suc_num;      //返回译码成功数量
	
	delete []Q;
	delete []ccr;
	delete []ff;
}


/* 
int CDecode::Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int **H1,int **H2,double sigma,double *Q,double *Q1,double *QS,double *QS1,double **QQ,double **RR,double**QQ1,double **RR1 )
{
	int i,j,k,nn;
	int suc_num;
	int dd=0;
//	double *QS,*QS1;
//	QS=new double[cc1_len];
//	QS1=new double[cc_len];
    double del;
	del=0.5*sigma*sigma;
	int *ccr;
	ccr=new int[ld_len];
//	double *Q,*Q1;
//	Q=new double[cc1_len];
//	Q1=new double[cc_len];
	double *ff;                                   
	ff=new double[lt_len];  
	double *fc;
	fc=new double[ld_len];
	
	for(j=0;j<lt_len;j++)                           //对Qij进行初始化
	{   
		ff[j]=tanh(0.5*c_rap1[j]/del);  
//		cout<<ff[j]<<" ";
	}
	for(i=0;i<ld_len;i++)
	{
		Q[i]=0;
		Q1[i]=0;
	}
    for(i=0;i<lt_len;i++) 
	  for(j=0;j<ld_len;j++)
//	     RR[i][j]=c_rap1[i]/del;
	     RR[i][j]=0;
	for(i=0;i<M;i++) 
	  for(j=0;j<ld_len;j++)
	     RR1[i][j]=0;
	for(i=0;i<ld_len;i++)
		fc[i]=0;
    
	
 for(k=0;k<iter;k++)
 {   
		for(j=0;j<ld_len;j++)
		{   
			for(i=0;i<lt_len;i++)
			{
             if(H2[i][j]!=0)
			 {  
				double q=0;
			    for(int t=0;t<lt_len;t++)
				{
					if((H2[t][j]==1)&&(t!=i))
						  q=q+RR[t][j];
				}                   
			
				  QQ[i][j]=q+Q1[j];
				//	  cout<<QQ[i][j]<<" ";
			}
			else
				QQ[i][j]=0;
			}
		}
	
//    for(i=0;i<80;i++)
//		for(j=0;j<80;j++)
//			cout<<QQ[i][j]<<" ";
//		cout<<endl;


	for(i=0;i<lt_len;i++)
	{   
	  // double t=1;
	   for(j=0;j<ld_len;j++)
	   {   
		if(H2[i][j]==1)                
		{ 
		  QS[j]=tanh(0.5*QQ[i][j]);
	//	  t=t*QS[j];		   
		}
	   }
	   for(j=0;j<ld_len;j++)
	   {
		if(H2[i][j]==1)     
//		   RR[i][j]=log((1+ff[i]*t/QS[j])/(1-ff[i]*t/QS[j])) ;
		{
			double t=1;
			for(int g=0;g<ld_len;g++)
			{
				if((H2[i][g]==1)&&g!=j)
					t=t*QS[g];
			}
			RR[i][j]=log((1+ff[i]*t)/(1-ff[i]*t)) ;
		}
		else
		   RR[i][j]=0;		      
	   }
	}	
   

   for(j=0;j<ld_len;j++)
   {
	   Q[j]=0;
	   for(i=0;i<lt_len;i++)
	   {
		   if(H2[i][j]!=0)
			   Q[j]=Q[j]+RR[i][j];
	   }
   }
		

   for(j=0;j<ld_len;j++)
   {   
	 for(i=0;i<M;i++)
	 {
       if(H1[i][j]!=0)
		{  
		  double q=0;		
		  for(int t=0;t<M;t++)	    
		  {
			if(H1[t][j]==1&&(t!=i))
			  q=q+RR1[t][j];
		  }                   
		  QQ1[i][j]=q+Q[j];
		}
	   else
		QQ1[i][j]=0;
	 }
   }

//for(i=0;i<80;i++)
//		for(j=0;j<80;j++)
//			cout<<QQ1[i][j]<<" ";
//		cout<<endl;



   for(i=0;i<M;i++)
	{   
	  // double t=1;
	   for(j=0;j<ld_len;j++)
	   {   
		if(H1[i][j]==1)                
		{ 
		  QS1[j]=tanh(0.5*QQ1[i][j]);
	//	  t=t*QS[j];		   
		}
	   }
	   for(j=0;j<ld_len;j++)
	   {
		if(H1[i][j]==1)     
//		   RR[i][j]=log((1+ff[i]*t/QS[j])/(1-ff[i]*t/QS[j])) ;
		{
			double t=1;
			for(int d=0;d<ld_len;d++)
			{
				if((H1[i][d]==1)&&d!=j)
					t=t*QS1[d];
			}
			RR1[i][j]=log((1+t)/(1-t)) ;
		}
		else
		   RR1[i][j]=0;		      
	   }
	}	
       
    for(j=0;j<ld_len;j++)
    {
	   Q1[j]=0;
	   for(i=0;i<M;i++)
	   {
		   if(H1[i][j]!=0)
			Q1[j]=Q1[j]+RR1[i][j];
	   }
    }
   
    for(j=0;j<ld_len;j++)               //每次迭代后进行判决
	{
		fc[j]=fc[j]+(Q[j]+Q1[j]);
		if(fc[j]>0)
			ccr[j]=0;
		else
			ccr[j]=1;
	}
	
	dd++;	                             
	suc_num=0;
    nn=0;
	for(i=0;i<M;i++)              //译码序列与校验矩阵的转置进行相乘
		{   	
			int w=0;
			for(j=0;j<ld_len;j++)
			{
				if(ccr[j]==1&&H1[i][j]==1)
				w++;
				w=w%2;
			}
			if(w!=0)
				nn++;
		}
	 for(i=0;i<ld_len;i++)
	 {
		 if(ccr[i]==c_ld[i])
			 suc_num++;
	 }
//	cout<<"第"<<dd<<"次"<<"成功译码数量为:"<<suc_num<<endl;
	if(nn==0)   
	  break;

  }
 




    cout<<"迭代次数为"<<dd<<endl;
//	if(nn==0)
//		suc_num=ld_len;
//	else
//		suc_num=0;

	delete []fc;
	delete []ff;
	delete []ccr;
	return suc_num;
 }
        
*/    

 int CDecode::Raptor_BSC(int* c_ld,int M,int ld_len,int lt_len,int iter,int *c_rap2,int **H1,int **H2,double *Q,double *Q1,double *QS,double *QS1,double **QQ,double **RR,double**QQ1,double **RR1,double qq )
{
	int i,j,k,nn;
	int suc_num;
	int dd=0;
//	double *QS,*QS1;
//	QS=new double[cc1_len];
//	QS1=new double[cc_len];
 //   double del;
//	del=0.5*sigma*sigma;
	int *ccr;
	ccr=new int[ld_len];
//	double *Q,*Q1;
//	Q=new double[cc1_len];
//	Q1=new double[cc_len];
	double *ff;                                   
	ff=new double[lt_len];  
	double *fc;
	fc=new double[ld_len];
	
	for(j=0;j<lt_len;j++)                           //对Qij进行初始化
	{   
		if(c_rap2[j]>0)
			ff[j]=tanh(log(qq)-log(1-qq));
		else
			ff[j]=tanh(log(1-qq)-log(qq));
	}
	for(i=0;i<ld_len;i++)
	{
		Q[i]=0;
		Q1[i]=0;
	}
    for(i=0;i<lt_len;i++) 
	  for(j=0;j<ld_len;j++)
	     RR[i][j]=0;
	for(i=0;i<M;i++) 
	  for(j=0;j<ld_len;j++)
	     RR1[i][j]=0;
	for(i=0;i<ld_len;i++)
		fc[i]=0;
    
	
 for(k=0;k<iter;k++)
 {   
		for(j=0;j<ld_len;j++)
		{   
			for(i=0;i<lt_len;i++)
			{
             if(H2[i][j]!=0)
			 {  
				double q=0;
			    for(int t=0;t<lt_len;t++)
				{
					if((H2[t][j]==1)&&(t!=i))
						  q=q+RR[t][j];
				}                   
			
				  QQ[i][j]=q+Q1[j];
				//	  cout<<QQ[i][j]<<" ";
			}
			else
				QQ[i][j]=0;
			}
		}
	
//    for(i=0;i<80;i++)
//		for(j=0;j<80;j++)
//			cout<<QQ[i][j]<<" ";
//		cout<<endl;


	for(i=0;i<lt_len;i++)
	{   
	  // double t=1;
	   for(j=0;j<ld_len;j++)
	   {   
		if(H2[i][j]==1)                
		{ 
		  QS[j]=tanh(0.5*QQ[i][j]);
	//	  t=t*QS[j];		   
		}
	   }
	   for(j=0;j<ld_len;j++)
	   {
		if(H2[i][j]==1)     
//		   RR[i][j]=log((1+ff[i]*t/QS[j])/(1-ff[i]*t/QS[j])) ;
		{
			double t=1;
			for(int g=0;g<ld_len;g++)
			{
				if((H2[i][g]==1)&&g!=j)
					t=t*QS[g];
			}
			RR[i][j]=log((1+ff[i]*t)/(1-ff[i]*t)) ;
		}
		else
		   RR[i][j]=0;		      
	   }
	}	
   

   for(j=0;j<ld_len;j++)
   {
	   Q[j]=0;
	   for(i=0;i<lt_len;i++)
	   {
		   if(H2[i][j]!=0)
			   Q[j]=Q[j]+RR[i][j];
	   }
   }
		

   for(j=0;j<ld_len;j++)
   {   
	 for(i=0;i<M;i++)
	 {
       if(H1[i][j]!=0)
		{  
		  double q=0;		
		  for(int t=0;t<M;t++)	    
		  {
			if(H1[t][j]==1&&(t!=i))
			  q=q+RR1[t][j];
		  }                   
		  QQ1[i][j]=q+Q[j];
		}
	   else
		QQ1[i][j]=0;
	 }
   }

//for(i=0;i<80;i++)
//		for(j=0;j<80;j++)
//			cout<<QQ1[i][j]<<" ";
//		cout<<endl;



   for(i=0;i<M;i++)
	{   
	  // double t=1;
	   for(j=0;j<ld_len;j++)
	   {   
		if(H1[i][j]==1)                
		{ 
		  QS1[j]=tanh(0.5*QQ1[i][j]);
	//	  t=t*QS[j];		   
		}
	   }
	   for(j=0;j<ld_len;j++)
	   {
		if(H1[i][j]==1)     
//		   RR[i][j]=log((1+ff[i]*t/QS[j])/(1-ff[i]*t/QS[j])) ;
		{
			double t=1;
			for(int d=0;d<ld_len;d++)
			{
				if((H1[i][d]==1)&&d!=j)
					t=t*QS1[d];
			}
			RR1[i][j]=log((1+t)/(1-t)) ;
		}
		else
		   RR1[i][j]=0;		      
	   }
	}	
       
    for(j=0;j<ld_len;j++)
    {
	   Q1[j]=0;
	   for(i=0;i<M;i++)
	   {
		   if(H1[i][j]!=0)
			Q1[j]=Q1[j]+RR1[i][j];
	   }
    }
   
    for(j=0;j<ld_len;j++)               //每次迭代后进行判决
	{
		fc[j]=fc[j]+(Q[j]+Q1[j]);
		if(fc[j]>0)
			ccr[j]=0;
		else
			ccr[j]=1;
	}
	
	dd++;	                             
	suc_num=0;
    nn=0;
	for(i=0;i<M;i++)              //译码序列与校验矩阵的转置进行相乘
		{   	
			int w=0;
			for(j=0;j<ld_len;j++)
			{
				if(ccr[j]==1&&H1[i][j]==1)
				w++;
				w=w%2;
			}
			if(w!=0)
				nn++;
		}
	 for(i=0;i<ld_len;i++)
	 {
		 if(ccr[i]==c_ld[i])
			 suc_num++;
	 }
//	cout<<"第"<<dd<<"次"<<"成功译码数量为:"<<suc_num<<endl;
	if(nn==0)   
	  break;

  }
 




    cout<<"迭代次数为"<<dd<<endl;
//	if(nn==0)
//		suc_num=ld_len;
//	else
//		suc_num=0;

	delete []fc;
	delete []ff;
	delete []ccr;
	return suc_num;
 }
        
    

		
        

        

int CDecode::Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int** H1,double sigma,
		            int** Row,int* Row_weight,int** Row_pos,int** Col,int* Col_weight,int** Col_pos,
					int** LT_Row,int* LT_Row_weight,int** LT_Row_pos,int** LT_Col,int* LT_Col_weight,int** LT_Col_pos,
					double** QQ,double** RR,double** QQ1,double** RR1)
{
	int i,j,k,m;                                
	int dd=0;   
	double del,t;                   
	int suc_num=0;           
	int *ccr;
	ccr=new int[ld_len];                                                      //用译出的码与校验矩阵相乘得出的结果
    double *ff;                                   
	ff=new double[lt_len];                          
    del=0.5*sigma;
//	del=0.25;
//	del=0.516;
//	cout<<"delta is "<<del<<endl;
	double *fc;
	fc=new double[ld_len];	
	for(j=0;j<lt_len;j++)                           //对Qij进行初始化
	{   
		ff[j]=tanh(0.5*c_rap1[j]/del);               
//		ff[j]=c_rap1[j]/del;                       //minsum译码
	}
	double* Q=new double[ld_len];
	double* Q1=new double[ld_len];

	double* LT_Qtemp=new double[1200];
	double* LT_Rtemp=new double[1200];
	double* LT_Rtemp1=new double[1200];
	double* Qtemp=new double[1200];
	double* Rtemp=new double[1200];
	double* Rtemp1=new double[1200];

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
			RR[i][j]=0.0;
		}
	}

	for(i=0;i<ld_len;i++)
	{
		for(j=0;j<Col_weight[i];j++)
		{
			QQ[i][j]=0.0;
		}
	}
	for(i=0;i<ld_len;i++)
		Q1[i]=0;
//------------------------------------------------

	for(k=0;k<iter;k++)
	{   
		for(i=0;i<ld_len;i++)
			Q[i]=0;
 
//-----**********************LT部分BP译码**********************************

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
		
//-------------------------------------

		for(i=0;i<lt_len;i++)
		{
			for(j=0;j<LT_Row_weight[i];j++)
			{
				LT_Rtemp[j]=tanh(0.5*QQ1[LT_Row[i][j]][LT_Row_pos[i][j]]);
			}
			for(j=0;j<LT_Row_weight[i];j++)
			{  
			   t=1.0;
			   for(m=0;m<LT_Row_weight[i];m++)
			   {
			    if(m!=j)
				  t=t*LT_Rtemp[m];
			   }
			   RR1[i][j]=log((1+ff[i]*t)/(1-ff[i]*t));
			}
			for(j=0;j<LT_Row_weight[i];j++)
				Q[LT_Row[i][j]]+=RR1[i][j];
		}
//******************************************************************************	
        /*
		for(k1=0;k1<1;k1++)
		{
//-----------------------------LT部分minsum译码-------
		
		   for(i=0;i<ld_len;i++)
		    {   
			    for(j=0;j<LT_Col_weight[i];j++)
			   {
				LT_Qtemp[j]=RR1[LT_Col[i][j]][LT_Col_pos[i][j]];
			   }
			   t=0;
			   for(j=0;j<LT_Col_weight[i];j++)
				 t+=LT_Qtemp[j];
			   if(k1==0)
			   {
			     for(j=0;j<LT_Col_weight[i];j++)
			     {
				   QQ1[i][j]=(t-LT_Qtemp[j]+Q1[i]);
			     }
			   }
			   else
			   {
			     for(j=0;j<LT_Col_weight[i];j++)
			     {
				   QQ1[i][j]=(t-LT_Qtemp[j]);
			     }			   
			   }
		    }  
           //-----------------------------------------------------
		   for(i=0;i<lt_len;i++)
		   {
			   for(j=0;j<LT_Row_weight[i];j++)
			   {
				LT_Rtemp[j]=QQ1[LT_Row[i][j]][LT_Row_pos[i][j]];
				LT_Rtemp1[j]=fabs(LT_Rtemp[j]);
			   }
			   if(LT_Row_weight[i]==1)
			   {
					 RR1[i][0]=0.714*ff[i];
			   }
			   else
			   {
			     for(j=0;j<LT_Row_weight[i];j++)
			    { 
				  if(j==0)
				  {
					min=LT_Rtemp1[1];
					if(LT_Rtemp[1]<0)
						t=-1;
					else
						t=1;
			        for(m=2;m<LT_Row_weight[i];m++)
					 {
						if(LT_Rtemp1[m]<min)
							min=LT_Rtemp1[m];
						if(LT_Rtemp[m]<0)
							t=t*(-1);
					 }
				
				  }
				  else
				  {
					min=LT_Rtemp1[0];
					if(LT_Rtemp[0]<0)
						t=-1;
					else
						t=1;
					 for(m=1;m<LT_Row_weight[i];m++)
					{
						if(m!=j)
						{
						  if(LT_Rtemp1[m]<min)
							  min=LT_Rtemp1[m];
						  if(LT_Rtemp[m]<0)
							  t=t*(-1);
						}
					}
					
				   }
				   if(ff[i]<0)
					  t=t*(-1);
				   if(min>fabs(ff[i]))
					 min=fabs(ff[i]);
				   RR1[i][j]=0.714*min*t;	 
		          }
			    }
			for(j=0;j<LT_Row_weight[i];j++)
				    Q[LT_Row[i][j]]+=RR1[i][j];  
		  }
		}
		
//----------------------------------------------------
	//	    for(i=0;i<lt_len;i++)
	//			for(j=0;j<LT_Row_weight[i];j++)
	//			    Q[LT_Row[i][j]]+=RR1[i][j];
		   
//------------------------------------------------------		
*/
		for(i=0;i<ld_len;i++)
			Q1[i]=0;
//--------------
	 
	 
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
	 

//----------------------------------最大似然译码---------------------------------

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
	 

/*
//----------------------------------------------------------min-sum译码------------------	

		

		 for(i=0;i<M;i++)
		 {
			for(j=0;j<Row_weight[i];j++)
			{
				Rtemp[j]=QQ[Row[i][j]][Row_pos[i][j]];
				Rtemp1[j]=fabs(Rtemp[j]);
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
			for(j=0;j<Row_weight[i];j++)
		           Q1[Row[i][j]]+=RR[i][j];
		 }
	 }
//---------------------------------------
	//	  for(i=0;i<M;i++)
	//		for(j=0;j<Row_weight[i];j++)
	//	           Q1[Row[i][j]]+=RR[i][j];
//----------------------------------------------
*/		
	
		
	
 
//-----------------------------------------判决------------------------

		for(i=0;i<ld_len;i++)
		{
			fc[i]=Q[i]+Q1[i];
//			fc[i]=Q[i];
			if(fc[i]>0)
				ccr[i]=0;
			else
				ccr[i]=1;
		}

		suc_num=0;
		for(i=0;i<(ld_len-M);i++)
		{
			if(ccr[i]==c_ld[i])
				suc_num++;
		}
		
//		cout<<"第"<<k+1<<"次迭代译码正确数为"<<suc_num+M<<endl;
 	if(suc_num==(ld_len-M))
			break;
	}


	delete []ccr;
	delete []fc;
	delete []ff;
	delete []Q;
	delete []Q1;
	delete []Qtemp;
	delete []Rtemp;
	delete []Rtemp1;
	delete []LT_Qtemp;
	delete []LT_Rtemp;
	delete []LT_Rtemp1;

	return suc_num;
	}
	



		
				

     
				

				




	


	













	



		
				

     
				

				




	


	



























       