class RandNum1  
{
public:
    RandNum1();
	virtual ~RandNum1();
    void RandomInitialise(int ij,int kl);
    double RandomUniform(int ij, int kl);
	double RandomGaussian(double mean,double stddev, int ij, int kl);
    int RandomInt(int lower,int upper);
    double Randomdouble(double lower,double upper);

private:
    
};
