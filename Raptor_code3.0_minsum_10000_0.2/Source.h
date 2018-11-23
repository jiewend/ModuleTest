
class CSource 
{
public:
	CSource();
	virtual ~CSource();

	double m_num_tot_blk;
	double m_num_tot_sym;
	int m_num_err_blk;
	int m_num_err_sym;
	int m_temp_err;

	double m_ser;
	double m_fer;
	
	void GetSymStr(int *uu, int q_ary, int len);
	void GetBitStr(int *uu, int len);
	void GetSparseStr(int *uu, double p, int len);
	void ClrCnt();
	void CntErr(int *uu, int *uu_hat, int len, int flag);
	//void PrintResult(FILE *fp);
};