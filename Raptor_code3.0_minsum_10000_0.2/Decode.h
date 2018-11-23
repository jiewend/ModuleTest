
class CDecode
{
public:
//	CDecode();
//	virtual ~CDecode();
//	int Decode(int cc_len,int ss_len,int*CS,struct Node**list1,struct Node**list2);
    int Decode_Gau(int cc_len,int *cc,int mm,int iter,double *ccc,int *uu,int **HH,double **QQ,double **RR,double sigma);
	int Decode_Bsc(int cc_len,int mm,int iter,int *cc,int *uu,int **HH,double **QQ,double **RR,double qq);
	int Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int **H1,int **H2,double sigma,double *Q,double *Q1,double *QS,double *QS1,double **QQ,double **RR,double**QQ1,double **RR1 );
    int Raptor_BSC(int* c_ld,int M,int ld_len,int lt_len,int iter,int *c_rap2,int **H1,int **H2,double *Q,double *Q1,double *QS,double *QS1,double **QQ,double **RR,double**QQ1,double **RR1,double qq );
    int Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int** H1,double sigma,
		            int** Row,int* Row_weight,int** Row_pos,int** Col,int* Col_weight,int** Col_pos,
					int** LT_Row,int* LT_Row_weight,int** LT_Row_pos,int** LT_Col,int* LT_Col_weight,int** LT_Col_pos,
					double** QQ,double** RR,double** QQ1,double** RR1);

};
