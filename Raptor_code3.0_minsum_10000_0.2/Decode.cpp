#include"RandNum.h"
#include"RandNum1.h"
//#include"Encode.h"
#include"Decode.h"
#include"stdafx.h"
using namespace std;
//extern CWHRandNum rndGen1;   
// RandNum1   rndGen2;



int CDecode::Decode_Gau(int cc_len,int *cc,int mm,int iter,double *ccc,int *uu,int **HH,double **QQ,double **RR,double sigma)
{                                                                                    //AWGN�ŵ��µ����뺯��
	int i,j,k;                                
	int dd=0;   
	double del;
                    //QQ��RR��Ϊ��ά�������ڴ�ŵ���ʱ�Ĵ��ݲ���Qij��Rij��sigmaΪ������׼��
                      //uuΪ��ʼ��Ԫ���������Աȼ���������ȷ�����õ�
    double *QS;
	QS=new double[cc_len];
	for(j=0;j<cc_len;j++)
		QS[j]=0;
	int suc_num=0;             //���ڷ��سɹ��������Ԫ����
	double *Q;                 //ÿ�ε�����ɺ����Q[i]�����о�,��Q[i]>0,����Ϊ0��������Ϊ1
	Q=new double[cc_len];
	int *ccr;
	ccr=new int[cc_len];                          //����о������
	int nn;                                       //�����������У�������˵ó��Ľ��
    double *ff;                                   
	ff=new double[cc_len];                          
//	for( i=0;i<bm_len;i++)
//	 for(j=0;j<cc_len;j++)
//	 { 
//		 cout<<HH[i][j];
//	 }

    
    del=0.5*sigma*sigma;
	for(j=0;j<cc_len;j++)                           //��Qij���г�ʼ��
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
	for(i=0;i<cc_len;i++)              //��������ɹ�����Ԫ����  
	{   
		if(ff[i]<0)
				ccr[i]=1;
			else
				ccr[i]=0;
	    if(ccr[i]==cc[i])
		suc_num++;		
	}   
	cout<<"����ǰ������ȷΪ��"<<suc_num<<endl;

	for(k=0;k<iter;k++)
	{
       for(i=0;i<mm;i++)
	   {   
		   double t=1;
		   for(j=0;j<cc_len;j++)
		   {   
			   if(HH[i][j]==1)                 //����У���������Ԫ���Ƿ�Ϊ1���ж����Ƿ��У��ڵ��������Ӷ�����Rij
			   {       QS[j]=tanh(0.5*QQ[i][j]);
				       t=t*QS[j];		 //�˴�t��ֵ�Ѿ��ǳ�С����������0
			   }
			 //  cout<<t<<" ";
		   }
		   
		   for(j=0;j<cc_len;j++)
		   {
			   if(HH[i][j]==1)
			   {                 
				   RR[i][j]=log((1+t/QS[j])/(1-t/QS[j])) ;            //������õ�RR[i][j]ȫ��Ϊ0����˵���������Ч

			   }
			   else
				   RR[i][j]=0;
			      
		   }
	   }
   //     cout<<"���RΪ"<<endl;
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
			Q[j]=ff[j]+q;                     //����Q[i]����ÿ�ε������������Ԫ��Ӳ�о���
			for(i=0;i<mm;i++)             //���㴫�ݲ���Qij��
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
      

		for(j=0;j<cc_len;j++)               //ÿ�ε���������о�
		{
			if(Q[j]<0)
				ccr[j]=1;
			else
				ccr[j]=0;
		}
		
        
		dd++;                              //��������˶��ٴ�
	
        nn=0;


		for(i=0;i<mm;i++)              //����������У������ת�ý������
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
		for(i=0;i<cc_len;i++)              //��������ɹ�����Ԫ����  
	{   
	    if(cc[i]==ccr[i])
		suc_num++;			
	}   
	cout<<"��"<<dd<<"��"<<"�ɹ���������Ϊ:"<<suc_num<<endl;
		
		if(nn==0)                     //�ж�C*��H��ת�ã��Ƿ�Ϊ0,Ϊ0��ֹͣ����
			break;
	//	cout<<endl;
     //  for(j=670;j<710;j++)
	//	   cout<<ccr[j]<<" ";
	}
    cout<<"nn="<<nn<<endl;
	cout<<"��������Ϊ"<<dd<<endl;
    suc_num=0;
	for(i=0;i<cc_len;i++)              //��������ɹ�����Ԫ����  
	{   
	    if(cc[i]==ccr[i])
		suc_num++;
			
	}   

	
	
	
	return suc_num;      //��������ɹ�����
	
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
      
        
		for(j=0;j<cc_len;j++)               //ÿ�ε���������о�
		{
			if(Q[j]>0)
				ccr[j]=0;
			else
				ccr[j]=1;
			
		}
		

		dd++;                              //��������˶��ٴ�
	
        nn=0;


		for(i=0;i<mm;i++)              //����������У������ת�ý������
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
	  for(i=0;i<(cc_len-mm);i++)              //��������ɹ�����Ԫ����  
	{   
	    if(uu[i]==ccr[mm+i])
		suc_num++;		
	}   

	cout<<"��"<<dd<<"��"<<"�ɹ���������Ϊ:"<<suc_num<<endl;
		
		if(nn==0)                     //�ж�C*��H��ת�ã��Ƿ�Ϊ0,Ϊ0��ֹͣ����
			break;
		 
	  
	}
    cout<<"nn="<<nn<<endl;
	cout<<"��������Ϊ"<<dd<<endl;
    suc_num=0;
	for(i=0;i<(cc_len-mm);i++)              //��������ɹ�����Ԫ����  
	{   
	    if(uu[i]==ccr[mm+i])
		suc_num++;
	}                    
    cout<<"�ɹ���������Ϊ"<<suc_num<<endl;
	
	return suc_num;      //��������ɹ�����
	
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
	
	for(j=0;j<lt_len;j++)                           //��Qij���г�ʼ��
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
   
    for(j=0;j<ld_len;j++)               //ÿ�ε���������о�
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
	for(i=0;i<M;i++)              //����������У������ת�ý������
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
//	cout<<"��"<<dd<<"��"<<"�ɹ���������Ϊ:"<<suc_num<<endl;
	if(nn==0)   
	  break;

  }
 




    cout<<"��������Ϊ"<<dd<<endl;
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
	
	for(j=0;j<lt_len;j++)                           //��Qij���г�ʼ��
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
   
    for(j=0;j<ld_len;j++)               //ÿ�ε���������о�
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
	for(i=0;i<M;i++)              //����������У������ת�ý������
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
//	cout<<"��"<<dd<<"��"<<"�ɹ���������Ϊ:"<<suc_num<<endl;
	if(nn==0)   
	  break;

  }
 




    cout<<"��������Ϊ"<<dd<<endl;
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
	ccr=new int[ld_len];                                                      //�����������У�������˵ó��Ľ��
    double *ff;                                   
	ff=new double[lt_len];                          
    del=0.5*sigma;
//	del=0.25;
//	del=0.516;
//	cout<<"delta is "<<del<<endl;
	double *fc;
	fc=new double[ld_len];	
	for(j=0;j<lt_len;j++)                           //��Qij���г�ʼ��
	{   
		ff[j]=tanh(0.5*c_rap1[j]/del);               
//		ff[j]=c_rap1[j]/del;                       //minsum����
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
 
//-----**********************LT����BP����**********************************

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
//-----------------------------LT����minsum����-------
		
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
	 

//----------------------------------�����Ȼ����---------------------------------

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
//----------------------------------------------------------min-sum����------------------	

		

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
	
		
	
 
//-----------------------------------------�о�------------------------

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
		
//		cout<<"��"<<k+1<<"�ε���������ȷ��Ϊ"<<suc_num+M<<endl;
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
	



		
				

     
				

				




	


	













	



		
				

     
				

				




	


	



























       