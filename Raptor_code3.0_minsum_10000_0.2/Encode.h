

class CEncode
{
public:
   void	Encode_ldpc(int **a,int m,int n ,int *kk,int cc_len,int *uu,int *cc);
   void Encode_LT (int M,int *c_ld,int *c_rap,int ld_len,int lt_len,int **HH, int **HH_LDPC, int** Row, int* Row_weight, int** Row_pos, int** Col, int* Col_weight, int** Col_pos);
};