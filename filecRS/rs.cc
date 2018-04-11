#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "rs.h"

using namespace std;
#define RS_CODE 31
#define RS_MESG 8
#define RS_PARI (RS_CODE - RS_MESG)

#define ERR_RATE 1000 /* error rate is 1/ERR_RATE */
#define LOOP_COUNT 10000

static int rs_init_flag = 0;
static int rs_code_len;
static int rs_mesg_len;
static int rs_gp_n;
static int rs_t;
static poly_t rs_gen_p; // generating polynomial

#define RS_ERR (-1)
#define RS_OK 0
#define GF_ELEMENTS 255

void encodeFile(string fileName, int PkgSize){
// 读入一个文件，对文件进行RS编码，将编码追加到文件末尾,另存为
	fstream file(fileName, ios::out|ios::in);
	file.seekg(0, ios::end);
	int filesize = file.tellg();
	file.seekg(0, ios::beg) ;

	cout<<"file size"<<filesize<<endl;
	int CODE_K = ceil(filesize / float(PkgSize));
	int CODE_N = pow(2, ceil(log10(CODE_K) / log10(2))) - 1;
	vector<string> fileMatrix;
	string temp(PkgSize, ' ');
	int byteLeft = filesize;

// read file loop	
	for (unsigned i = 0; i < CODE_K; i++) { 
		int readSize = min(PkgSize, filesize - (int)file.tellg());
		bzero((void*)temp.c_str(), temp.size());
		file.read((char*)temp.c_str(), readSize);
		fileMatrix.push_back(temp);
	}
	
	
	for (unsigned i = 0; i < CODE_N - CODE_K; i++)  
		fileMatrix.push_back(temp);

	cout<<"code init"<<endl;
	rs_init(CODE_N, CODE_K);
	cout<<"CODE_K : "<<CODE_K<<"  CODE_N : "<<CODE_N<<endl;
	uint8_t codeOriginal[CODE_N], codeCorrect[CODE_N];
	bzero(codeOriginal, sizeof(codeOriginal));
	bzero(codeCorrect, sizeof(codeCorrect));
// encode loop
	for (unsigned i = 0; i < PkgSize; i++) { 
		// cout<<"__________________ encode i : "<<i<<"______________"<<endl;
		for (unsigned j = 0; j < CODE_K; j++) { 
			codeOriginal[j] = (uint8_t)fileMatrix[j].c_str()[i];
		} 

		rs_encode(codeOriginal);

		for (unsigned j = CODE_K; j < CODE_N; j++) { 

			memcpy((char*)fileMatrix[j].c_str() + i, codeOriginal + j, 1);
			// cout<<hex<<(int)codeOriginal[j]<<"  "<<(int)(unsigned char)(fileMatrix[j].c_str()[i])<<dec<<endl;
		} 
	} 

	for (unsigned i = 0; i < CODE_N; i++) { 
		cout<<fileMatrix[i].size()<<endl;
	} 

	string rsFileName = fileName.append(".rs");
	cout<<".rs file name : "<<rsFileName<<endl;
	ofstream rsFile(rsFileName, ios::binary);
	for (unsigned i = 0; i < CODE_N; i++) { 
		rsFile.write(fileMatrix[i].c_str(), PkgSize);
	} 
	rsFile.close();
	ifstream file1(rsFileName, ios::in|ios::out);
	file1.seekg(0, ios::end) ;
	int filesize1 = file1.tellg();
	cout<<"file size"<<filesize1<<endl;
}

void decodeFile(string fileName, int PkgSize, int Code_K){
// 读入一个文件，对文件进行RS解码，解码后将文件末尾的RS编码去除，另存为

	fstream file(fileName, ios::out|ios::in);
	file.seekg(0, ios::end);
	int filesize = file.tellg();
	file.seekg(0, ios::beg) ;

	cout<<"file size"<<filesize<<endl;
	int CODE_K = Code_K;
	int CODE_N = pow(2, ceil(log10(CODE_K) / log10(2))) - 1;

	rs_init(CODE_N, CODE_K);
	vector<string> fileMatrix;
	string temp(PkgSize, ' ');
	int byteLeft = filesize;

	for (unsigned i = 0; i < CODE_N; i++) { 
		int readSize = min(PkgSize, filesize - (int)file.tellg());
		file.read((char*)temp.c_str(), readSize);
		fileMatrix.push_back(temp);
	}

	int m[PKGSIZE] = {1};
	uint8_t codeOriginal[CODE_N], codeCorrect[CODE_N];
	bzero(codeOriginal, sizeof(codeOriginal));
	bzero(codeCorrect, sizeof(codeCorrect));

	for (unsigned i = 0; i < PkgSize; i++) { 
		// cout<<"================== decode i : "<<i<<"=============="<<endl;
		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy(codeCorrect + j, (char*)fileMatrix[j].c_str() + i, 1);
			// cout<<hex<<(int)codeCorrect[j]<<dec<<endl;
		} 
		
		// cout<<endl;
		m[i] = rs_decode(codeCorrect);

		for (unsigned j = 0; j < CODE_N; j++) { 
			memcpy((char*)fileMatrix[j].c_str() + i, codeCorrect + j, 1);
		} 

	} 

	string foutName = fileName + ".repair";
	cout<<"decode file : "<<foutName<<endl;
	ofstream fout(foutName, ios::binary);
	
	// string content(100, 'a');
	// fout.write(content.c_str(), 100);
	for (unsigned i = 0; i < CODE_K; i++)  
		fout.write(fileMatrix[i].c_str(), PkgSize);
	fout.close();

	cout<<"hello world!!"<<endl;
}



/*
  calculate generation polynamial

  n is maximum dimension of generation polynomial.
  return product((x - a^i), i, 0, n)
*/
int make_gp(poly_t *gp, int n)
{
  int i, j;

  if (n >= POLY_SIZE) return -1; // too big

  poly_clear(gp);
  gp->coeff[0] = 1; // initialize

  for (i = 0; i < n; i++) {
    gf_t a = gf_pow(i);
    gf_t b = 0;

    for (j = 0; j <= i; j++) {
      gf_t c = gp->coeff[j]; /* save value */

      gp->coeff[j] = gf_sub(b, gf_mul(c, a)); /* a_i = a_i * a^i + a_i-1 */
      b = c; /* carry next degree of x */
    }
    gp->coeff[j] = b; /* increase most significant degree */
  }
  gp->degree = n;

  return 0;
}

/*
  make generating polynomial G_n(x)
*/
static int make_genp(poly_t *gp, int n)
{
  int i;
  poly_t xa, tmp;
  gf_t an;

  if (n >= POLY_SIZE) return -1; /* too big */

  poly_clear(gp);
  gp->coeff[0] = 1; /* G_0(x) = 1 */

  /* initialize polynomial (x - a^n) */
  poly_clear(&xa);
  xa.degree = 1;
  xa.coeff[1] = 1;

  for (i = 0; i  < n; i++) {
    an = gf_pow(i); /* a^i */
    xa.coeff[0] = gf_neg(an); /* make (x - a^i) */

    poly_mul(gp, &xa, &tmp); /* G_i+i(x) = G_i(x) * (x - a^i) */
    poly_copy(gp, &tmp);
  }

  return 0;
}

int rs_init(int code_len, int mesg_len)
{
  gf_init();

  if (code_len <= 0) return RS_ERR;
  if (code_len > GF_ELEMENTS) return RS_ERR;

  rs_code_len = code_len;

  if (mesg_len >= code_len) return RS_ERR;
  if (mesg_len <= 0) return RS_ERR;

  rs_mesg_len = mesg_len;

  rs_gp_n = code_len - mesg_len;
  rs_t = rs_gp_n / 2;

  if (rs_t <= 0) return RS_ERR;

  make_genp(&rs_gen_p, rs_gp_n);

  /*
  printf("rs_gen_p: %d\n", rs_gp_n);
  poly_print(&rs_gen_p);
  */

  if (rs_init_flag == 0) {
    gf_init();
    rs_init_flag = 1;
  }

  return RS_OK;
}

/*
  generate RS parity from message
*/
int rs_encode(gf_t code[])
{
  int i;
  poly_t dividend, quotinent, remainder;

  /* clear low coefficient of dividend */
  for (i = 0; i < rs_gp_n; i++) {
    dividend.coeff[i] = 0;
  }

  /* copy message to dividend */
  for (i = 0; i < rs_mesg_len; i++) {
    dividend.coeff[(rs_code_len - 1) - i] = code[i];
  }

  dividend.degree = rs_code_len - 1;
  poly_normalize(&dividend);


  /* divide by generating polynomial */
  poly_div(&dividend, &rs_gen_p, &quotinent, &remainder);

  /* copy negated remainder */
  for (i = 0; i < rs_gp_n; i++) {
    code[rs_mesg_len + i] = gf_neg(remainder.coeff[(rs_gp_n - 1) - i]);
  }

  return RS_OK;
}

/*
  decode RS code

  input: received RS code
  output: corrected code
  return value: zero or positive, correction successful, value is number of corrected symbols
  negative value: unsuccessful of error correction
*/
int rs_decode(gf_t code[])
{
  int i;
  poly_t code_p, syn_p;
  poly_t quotient, remainder;

 /* make polynomial from received code */
  for (i = 0; i < rs_code_len; i++) {
    code_p.coeff[(rs_code_len - 1) - i] = code[i];
  }
  code_p.degree = rs_code_len - 1;
  poly_normalize(&code_p);

  /* calculate syndrome */

  /*
  printf("code_p\n");
  poly_print(&code_p);

  printf("Syndrome\n");
  */

  /* calculate syndrome and make polinomial S(x) */

  for (i = 0; i < rs_gp_n; i++) { // rs_gp_n is number of parity
    gf_t s = poly_subst(&code_p, gf_pow(i));
    syn_p.coeff[(rs_gp_n - 1) - i] = s;
    /*printf("C(a^%d) = %d, a^%d\n", i, s, gf_ind(s));*/
  }
  syn_p.degree = rs_gp_n - 1;
  poly_normalize(&syn_p);

  /* check, is syndrome all zero ? */

  if (poly_iszero(&syn_p)) {
#if 1
    return 0; /* no error */
#else
    /* parity check */
    poly_div(&code_p, &rs_gen_p, &quotient, &remainder);
    if (poly_iszero(&remainder)) return 0; /* no error */
 
    return -3; /* something wrong */
#endif
  }

  poly_t x_2t; /* polinomial X^(2*t) */

  poly_clear(&x_2t);
  x_2t.degree = rs_gp_n;
  x_2t.coeff[rs_gp_n] = 1;

  poly_t sigma, omega;
  poly_t phi;

  /* calculate sigma(x), omega(x) */
  poly_euclid(&x_2t, &syn_p, &sigma, &omega, &phi, rs_gp_n / 2);

  /* error correction */

  /* calculate -sigma(x) */
  poly_neg(&sigma);

  poly_t dsigma;

  /* differentiating sigma */
  poly_diff(&sigma, &dsigma);

  /* find error position */
  int errs = 0;

  for (i = 0; (i < rs_code_len) && (errs < rs_t); i++) {
    gf_t an = gf_pow(i);

    if (poly_subst(&sigma, an) == 0) { /* found error position */
      gf_t c, e, n;

      errs++;

      /* calculate error value */
      e = gf_div(poly_subst(&omega, an), poly_subst(&dsigma, an));

      /* correct message */
      c = code[(rs_code_len - 1) - i];
      n = gf_sub(c, e);
      code[(rs_code_len - 1) - i] = n;

      /* correct code polinomial for parity check */
      code_p.coeff[i] = n;
    }
  }

  if (errs == 0) { /* there is no error, something wrong */
    return -1;
  }

  /* check correctness of RS decoded data */
  code_p.degree = rs_code_len - 1;
  poly_normalize(&code_p);
  poly_div(&code_p, &rs_gen_p, &quotient, &remainder);
  if (!poly_iszero(&remainder)) {
    return -2; /* error correction unsuccessful */
  }

  return errs;
}
////////////////////////////////////
/*
  polynomial operation library
*/


/*
  copy polynomial
*/
void poly_copy(poly_t *to, poly_t *from)
{
  int i;

  for (i = 0; i <= from->degree; i++)
    to->coeff[i] = from->coeff[i];

  to->degree = from->degree;
}

/*
  normalize polynomial degree
*/
void poly_normalize(poly_t *poly)
{
  int i;

  i = poly->degree;
  while ((i > 0) && (poly->coeff[i] == 0)) {
    --i;
  }
  poly->degree = i;
}

/*
  set polynomial degree by coefficients
*/
void poly_setdeg(poly_t *poly)
{
  int i;

  //i = poly->degree;
  i = POLY_SIZE - 1;
  while ((i > 0) && (poly->coeff[i] == 0)) {
    --i;
  }
  poly->degree = i;
}

/*
  assign zero to polynomial
*/
void poly_clear(poly_t *poly)
{
  int i;

  for (i = 0; i < POLY_SIZE; i++)
    poly->coeff[i] = 0;

  poly->degree = 0;
}

/*
  return true if polynomial is zero
*/
int poly_iszero(poly_t *poly)
{
  int i;

  i = poly->degree;
  while ((i > 0) && (poly->coeff[i] == 0)) {
    --i;
  }

  return (i == 0) && (poly->coeff[i] == 0);
}

/*
  polynomial division

  return
    quotient = dividend / divisor
    remainder = dividend % dvisor
*/
int poly_div(
	     poly_t *dividend,
	     poly_t *divisor,
	     poly_t *quotient,
	     poly_t *remainder
	     )
{
  int i, j;
  poly_t dd; /* work for divide */

  if (poly_iszero(divisor)) return -1; // dvisor == 0

  poly_copy(&dd, dividend);

  /* degree of quotient */
  int qi = dividend->degree - divisor->degree;
  quotient->degree = qi;

  /* reciplocal number of most significant coefficient of divisor */
  gf_t d = divisor->coeff[divisor->degree];

  for (i = dd.degree; i >= divisor->degree; --i) {
    gf_t q = gf_div(dd.coeff[i], d); /* quotient = dd.coeff / divisor.coeff */
    quotient->coeff[qi--] = q;

    /* subtract by divisor * q */
    for (j = 0; j <= divisor->degree; j++) {
      dd.coeff[i - divisor->degree + j] =
	gf_sub(dd.coeff[i - divisor->degree + j], gf_mul(divisor->coeff[j], q));
    }
  }

  poly_copy(remainder, &dd); // dd is remainder
  poly_normalize(remainder); // normalize remainder and quotinent
  poly_normalize(quotient);

  return 0;
}

/*
  output polynomial coefficients
*/
void poly_print(poly_t *poly)
{
  int i;
  gf_t c;

  printf("degree: %d\n", poly->degree);
  for (i = 0; i <= poly->degree; i++) {
    c = poly->coeff[i];
    if ((c != 0) || (i == 0)) printf("coeff[%d]: %d, a^%d\n", i, c, gf_ind(c));
  }
}


/* calculate polynomial value */
gf_t poly_subst(poly_t *poly, gf_t x)
{
  int i;
  gf_t v = 0;

  for (i = poly->degree; i >= 0; --i) {
    v = gf_add(gf_mul(v, x), poly->coeff[i]);
  }

  return v;
}

/*
  polynomial addition/subtraction
*/
static void poly_addsub(poly_t *p1, poly_t *p2, poly_t *result,
		     gf_t (*gf_addsub)(gf_t x, gf_t y))
{
  int i;

  poly_clear(result);

  i = 0;
  while ((i <= p1->degree) || (i <= p2->degree)) {
    result->coeff[i] = (*gf_addsub)((i <= p1->degree) ? p1->coeff[i] : 0,
			       (i <= p2->degree) ? p2->coeff[i] : 0);
    //printf("poly_add: coeff[%d] = %d\n", i, result->coeff[i]);
    i++;
  }

  result->degree = i - 1;
  poly_normalize(result);
  //printf("poly_add: degree: %d\n", result->degree);
}

/* polynomial addtion */
void poly_add(poly_t *p1, poly_t *p2, poly_t *result)
{
  poly_addsub(p1, p2, result, gf_add);
}

/* polynomial subtraction */
void poly_sub(poly_t *p1, poly_t *p2, poly_t *result)
{
  poly_addsub(p1, p2, result, gf_sub);
}

/* polynomial multiplication */
void poly_mul(poly_t *p1, poly_t *p2, poly_t *result)
{
  int i, j;

  poly_clear(result);

  for (i = 0; i <= p1->degree; i++) {
    gf_t m = p1->coeff[i];
    for (j = 0; j <= p2->degree; j++) {
      gf_t t;
      t = gf_mul(m, p2->coeff[j]);
      result->coeff[i + j] = gf_add(result->coeff[i + j], t);
    }
  }
  result->degree = p1->degree + p2->degree;
  poly_normalize(result);
}

/*
  extended Euclidean algorithm

  input x, y, t; t specifies degree of remainder
  output a, b, c where a*x + b*y = c (deg c < t)
*/
void poly_euclid(poly_t *x, poly_t *y, poly_t *a, poly_t *b, poly_t *c, int t)
{
  poly_t a0, a1, a2;
  poly_t b0, b1, b2;
  poly_t r0, r1, r2;
  poly_t q1;
  poly_t tmp;

  //printf("t = %d\n", t);

  poly_copy(&r0, x);
  poly_copy(&r1, y);

  poly_clear(&a0); a0.coeff[0] = 1;
  poly_clear(&a1);
  poly_clear(&b0);
  poly_clear(&b1); b1.coeff[0] = 1;

  while (r1.degree >= t) { /* exit if deg r1 < t */

    poly_div(&r0, &r1, &q1, &r2); /* q1 = r0 / r1 and r2 = r0 % r1 */


    /*
    printf("quotient\n");
    poly_print(&q1);

    printf("remainder\n");
    poly_print(&r2);
    */

    poly_mul(&q1, &a1, &tmp); // tmp = aq * a1
    poly_sub(&a0, &tmp, &a2); // a2 = a0 - tmp

    poly_mul(&q1, &b1, &tmp); // tmp = q1 * b1
    poly_sub(&b0, &tmp, &b2); // b2 = b0 - tmp

    poly_copy(&r0, &r1); // r0 = r1
    poly_copy(&r1, &r2); // r1 = r2

    poly_copy(&a0, &a1); // a0 = a1
    poly_copy(&a1, &a2); // a1 = a2

    poly_copy(&b0, &b1); // b0 = b1
    poly_copy(&b1, &b2); // b1 = b2
  }

  poly_copy(c, &r1); // c = r0
  poly_copy(a, &b1); // a = a0
  poly_copy(b, &a1); // b = b0
}

/*
  negate polynomial
 */
void poly_neg(poly_t *poly)
{
  int i;

  for (i = 0; i <= poly->degree; i++) {
    poly->coeff[i] = gf_neg(poly->coeff[i]); /* coeff = - coeff */
  }
}

/*
  Differentiating polynomial
 */
void poly_diff(poly_t *f, poly_t *df)
{
  int i;

  poly_clear(df);

  if (f->degree <= 0) return;

  for (i = 1; i <= f->degree; i++) {
    gf_t c = 0;
    gf_t a = f->coeff[i];
    int j = i;

    while (j-- > 0) { /* add i times (can not use gf_mul) */
      c = gf_add(c, a);
    }

    df->coeff[i - 1] = c; /* coefficient of x^(i-1) */
  }
  df->degree = f->degree - 1;
  poly_normalize(df);
}

//////////////////////////////////////////
/*
  Operations on Galois Field GF(prime)
*/


static gfp_t gfp_index[GFP_ELEMENTS]; /* table for ind */
static gfp_t gfp_power[GFP_ELEMENTS]; /* table for pow */
static gfp_t gfp_reciprocal[GFP_ELEMENTS]; /* table for reciprocal number */

/*
  initialize internal tables
*/
void gfp_init(void)
{
  static int init_flag = 0;
  int i, j;
  gfp_t a = GFP_PRIMITIVE; /* primitive element */
  int n = 1; /* a^0 */

  if (init_flag) return;

  /* make index and power tables */
  for (i = 0; i < GFP_ELEMENTS - 1; i++) {
    gfp_index[n] = i; /* n = a^i */
    gfp_power[i] = n; /* a^i = n */

    n = gfp_mul(n, a); /* a^(i+1) = a^n * a, increment index of a^n */
  }
  gfp_index[0] = GFP_PRIME - 1; // 0 has no index, assign maximum element
  gfp_power[GFP_PRIME - 1] = 1; // a^(GF_PRIME - 1) = 1

  /* make reciplocal number table */
  for (i = 1; i < GFP_PRIME; i++) {
    for (j = 1; j < GFP_PRIME; j++) {
      if (gfp_mul(i, j) == 1) {
	gfp_reciprocal[i] = j;
	break;
      }
    }
  }
  gfp_reciprocal[0] = 0; // 0 has no reciprocal number;

  init_flag = 1;
}

/*
  addtion
  return x + y
*/
gfp_t gfp_add(gfp_t x, gfp_t y)
{
  return (x + y) % GFP_PRIME;
}

/*
  subtraction
  return x - y
*/
gfp_t gfp_sub(gfp_t x, gfp_t y)
{
  return (x + gfp_neg(y)) % GFP_PRIME;
}


/*
  index
  return y where a^y = x
 */
gfp_t gfp_ind(gfp_t x)
{
  return gfp_index[x];
}

/*
  power
  return a^x
 */
gfp_t gfp_pow(gfp_t x)
{
  return gfp_power[x];
}

/*
  multiplication
  return x * y
*/
gfp_t gfp_mul(gfp_t x, gfp_t y)
{
  return (x * y) % GFP_PRIME;
}

/*
  reciplocal number
  return y where x * y = 1
 */
gfp_t gfp_recip(gfp_t x)
{
  if (x <= 1) return x; // return 0 if x == 0

  return gfp_reciprocal[x];
}

/*
  dividing
  return x / y
*/
gfp_t gfp_div(gfp_t x, gfp_t y)
{
  return (x * gfp_recip(y)) % GFP_PRIME;
}

/*
  negate
  return -x
*/
gfp_t gfp_neg(gfp_t x)
{
  if (x == 0) return 0;

  return GFP_PRIME - x;
}
/////////////////////////////////////////
/*
  Operations on Galois Field GF(2^8)
*/


static gf2p8_t gf2p8_index[GF2P8_ELEMENT];
static gf2p8_t gf2p8_power[GF2P8_ELEMENT];

/*
  initialize internal tables
*/
void gf2p8_init(void)
{
  int i;
  const int b = 0x1d; /* x^8 = x^4 + x^3 + x^2 + 1*/
  int n = 1; /* a^0 */

  /* make index and power tables */
  for (i = 0; i < GF2P8_ELEMENT - 1; i++) {
    gf2p8_index[n] = i; /* n = a^i */
    gf2p8_power[i] = n; /* a^i = n */

    n <<= 1; /* a^(i+1) = a^n * a, increment index of a^n */
    if (n > 255) {
      /* calculate bit vector notation */
      n = (n & 0xff) ^ b; /* a^n = (a^n - a^8) + (a^4 + a^3 + a^2 + 1) */
    }
  }
  gf2p8_index[0] = 255; // 0 has no index
  gf2p8_power[255] = 1; // a^255 = 1
}

/*
  addtion
  return x + y
*/
gf2p8_t gf2p8_add(gf2p8_t x, gf2p8_t y)
{
  return x ^ y;
}

/*
  subtraction
  return x - y
*/
gf2p8_t gf2p8_sub(gf2p8_t x, gf2p8_t y)
{
  return gf2p8_add(x, y); /* same as addtion */
}


/*
  index
  return y where a^y = x
 */
gf2p8_t gf2p8_ind(gf2p8_t x)
{
  return gf2p8_index[x];
}

/*
  power
  return a^x
 */
gf2p8_t gf2p8_pow(gf2p8_t x)
{
  return gf2p8_power[x];
}

/*
  multiplication
  return x * y
*/
gf2p8_t gf2p8_mul(gf2p8_t x, gf2p8_t y)
{
  int ind;

  if (x == 0 || y == 0) return 0;

  ind = gf2p8_ind(x) + gf2p8_ind(y);
  ind %= GF2P8_ORDER;

  return gf2p8_pow(ind);
}

/*
  reciplocal number
  return y where x * y = 1
 */
gf2p8_t gf2p8_recip(gf2p8_t x)
{
  if (x <= 1) return x; // return 0 if x == 0

  return gf2p8_pow(GF2P8_ORDER - gf2p8_ind(x));
}

/*
  dividing
  return x / y
*/
gf2p8_t gf2p8_div(gf2p8_t x, gf2p8_t y)
{
  return gf2p8_mul(x, gf2p8_recip(y));
}

/*
  negate
  return -x
*/
gf2p8_t gf2p8_neg(gf2p8_t x)
{
  return x; // x == -x on GF(2^n)
}
///////////////////////////////////////////
/*
  Galoris Feild routines
*/


#ifdef USE_GF2P8
#endif

#ifdef USE_GFP
#endif
