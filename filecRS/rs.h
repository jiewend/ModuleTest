/*
  Operations on Galois Field GF(2^8)
*/
#include <iostream> 
#include <fstream>
#include <cstdint>
#include <vector> 
#include <sys/stat.h> 
#include <string> 
#include <string.h> 
#include <math.h>
#include <algorithm> 
using namespace std;

void encodeFile(string fileName, int PkgSize);
void decodeFile(string fileName, int PkgSize, int Code_K);
#ifndef GF2P8_H

#include <stdio.h>
#include <stdlib.h>
#define PKGSIZE 980

#define GF2P8_ELEMENT 256
#define GF2P8_ORDER 255

typedef unsigned char gf2p8_t;

/*
  initialize internal tables
*/
void gf2p8_init(void);


/*
  addtion
  return x + y
*/
gf2p8_t gf2p8_add(gf2p8_t x, gf2p8_t y);

/*
  subtraction
  return x - y
*/
gf2p8_t gf2p8_sub(gf2p8_t x, gf2p8_t y);



/*
  index
  return y where a^y = x
 */
gf2p8_t gf2p8_ind(gf2p8_t x);

/*
  power
  return a^x
 */
gf2p8_t gf2p8_pow(gf2p8_t x);

/*
  multiplication
  return x * y
*/
gf2p8_t gf2p8_mul(gf2p8_t x, gf2p8_t y);


/*
  reciplocal number
  return y where x * y = 1
 */
gf2p8_t gf2p8_recip(gf2p8_t x);


/*
  dividing
  return x / y
*/
gf2p8_t gf2p8_div(gf2p8_t x, gf2p8_t y);


/*
  negate
  return -x
*/
gf2p8_t gf2p8_neg(gf2p8_t x);

#define GF2P8_H 1
#endif

/*
  Operations on Galois Field GF(prime)
*/

#ifndef GFP_H

#ifndef GFP_PRIME

#define GFP_PRIME 11
#define GFP_PRIMITIVE 2 /* primitive element */

#endif

#define GFP_ELEMENTS GFP_PRIME
#define GFP_ORDER (GFP_PRIME-1)

#if GFP_PRIME < 256

typedef unsigned char gfp_t;

#elif GFP_PRIME < 65536

typedef unsigned short int gfp_t;

#else

typedef unsigned long int gfp_t;

#endif

/*
  initialize internal tables
*/
void gfp_init(void);


/*
  addtion
  return x + y
*/
gfp_t gfp_add(gfp_t x, gfp_t y);

/*
  subtraction
  return x - y
*/
gfp_t gfp_sub(gfp_t x, gfp_t y);

/*
  index
  return y where a^y = x
 */
gfp_t gfp_ind(gfp_t x);

/*
  power
  return a^x
 */
gfp_t gfp_pow(gfp_t x);

/*
  multiplication
  return x * y
*/
gfp_t gfp_mul(gfp_t x, gfp_t y);

/*
  reciplocal number
  return y where x * y = 1
 */
gfp_t gfp_recip(gfp_t x);

/*
  dividing
  return x / y
*/
gfp_t gfp_div(gfp_t x, gfp_t y);

/*
  negate
  return -x
*/
gfp_t gfp_neg(gfp_t x);

#define GFP_H 1
#endif

/*
  Galoris Feild processing routines
*/

#ifndef GF_H
#define GF_H

/* uncomment one of these identfiers */

#define USE_GF2P8 1
//#define USE_GFP 1

#ifdef USE_GF2P8


#define GF_ORDER GF2P8_ORDER

#define gf_t gf2p8_t

#define gf_init gf2p8_init
#define gf_add gf2p8_add
#define gf_sub gf2p8_sub
#define gf_mul gf2p8_mul
#define gf_div gf2p8_div
#define gf_pow gf2p8_pow
#define gf_ind gf2p8_ind
#define gf_neg gf2p8_neg

#elif defined(USE_GFP)

#ifndef GFP_PRIME

/* must define GFP_PRIME and GFP_PRIMITIVE */
#define GFP_PRIME 11
#define GFP_PRIMITIVE 2

#endif


#define gf_t gfp_t

#define GF_ORDER (GFP_PRIME-1)

#define gf_init gfp_init
#define gf_add gfp_add
#define gf_sub gfp_sub
#define gf_mul gfp_mul
#define gf_div gfp_div
#define gf_pow gfp_pow
#define gf_ind gfp_ind
#define gf_neg gfp_neg

#endif

#endif


#ifndef POLY_H


#define POLY_SIZE GF_ORDER /* Maximum number of coefficients */

typedef struct POLY {
  int degree;
  gf_t coeff[POLY_SIZE];
} poly_t;

/*
  copy polynomial
*/
void poly_copy(poly_t *to, poly_t *from);

/*
  normalize polynomial degree
*/
void poly_normalize(poly_t *poly);

/*
  set polynomial degree by coefficients
*/
void poly_setdeg(poly_t *poly);


/*
  return true if polynomial is zero
*/
int poly_iszero(poly_t *poly);

/*
  assign zero to polynomial
*/
void poly_clear(poly_t *poly);

/*
  return true if polynomial is zero
*/
int poly_iszero(poly_t *poly);

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
	     );


/*
  output polynomial coefficients
*/
void poly_print(poly_t *poly);



/* calculate polynomial value */
gf_t poly_subst(poly_t *poly, gf_t x);


/* polynomial addition */
void poly_add(poly_t *p1, poly_t *p2, poly_t *result);


/* polynomial subtraction */
void poly_sub(poly_t *p1, poly_t *p2, poly_t *result);


/* polynomial multiplication */
void poly_mul(poly_t *p1, poly_t *p2, poly_t *result);


/*
  extended Euclidean algorithm

  input x, y, t; t specifies degree of common divider
  output a, b, c where a*x + b*y = c (deg c < t)
*/
void poly_euclid(poly_t *x, poly_t *y, poly_t *a, poly_t *b, poly_t *c, int t);

/*
  negate polynomial
 */
void poly_neg(poly_t *poly);

/*
  Differentiating polynomial
 */
void poly_diff(poly_t *f, poly_t *df);

#define POLY_H 1
#endif


/*
  Reed Solomon code library
*/

#ifndef RS_H


#define RS_ERR (-1)
#define RS_OK 0
#define GF_ELEMENTS 255

/*
  calculate generation polynamial

  n is maximum dimension of generation polynomial.
  return product((x - a^i), i, 0, n)
*/
int make_gp(poly_t *gp, int n);

/*
  initialize RS routine
*/
int rs_init(int code_len, int mesg_len);


/*
  Differentiating polynomial
 */
void poly_diff(poly_t *f, poly_t *df);


/*
  generate RS parity from message
*/
int rs_encode(gf_t code[]);


/*
  decode RS code
*/
int rs_decode(gf_t code[]);

#define RS_H 1
#endif
