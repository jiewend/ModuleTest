
class CDecode
{
public:

    int Raptor_AWGN(int* c_ld,int M,int ld_len,int lt_len,int iter,double *c_rap1,int** H1,double sigma,
		            int** Row,int* Row_weight,int** Row_pos,int** Col,int* Col_weight,int** Col_pos,
					int** LT_Row,int* LT_Row_weight,int** LT_Row_pos,int** LT_Col,int* LT_Col_weight,int** LT_Col_pos,
					double** QQ,double** RR,double** QQ1,double** RR1);

};
