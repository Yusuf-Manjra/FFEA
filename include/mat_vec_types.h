#ifndef MAT_VEC_TYPES_H_INCLUDED
#define MAT_VEC_TYPES_H_INCLUDED

#include <limits>
#include <cmath> 

/*
 * Defines what is meant by a scalar (essentially sets the precision of
 * the code between float or double).
 */
#ifdef USE_DOUBLE
typedef double scalar;
typedef double geoscalar;
#else
typedef float scalar;
typedef double geoscalar;
#endif 
//typedef long double scalar;

////////  Constants and scalar functions ///////
namespace ffea_const {
   const scalar threeErr = 3.0*std::numeric_limits<scalar>::epsilon();
   const scalar threeGeoErr = 3.0*std::numeric_limits<geoscalar>::epsilon();
   const scalar mOne = -1.0;
   const scalar zero = 0.0;
   const scalar half = 0.5;
   const scalar one = 1.0;
   const scalar two = 2.0;
   const scalar eight = 8.0;
   const geoscalar ten = 10.0;
   const scalar twentyfour = 24.0;
   const scalar oneOverThree = 0.33333333333333333;
   const scalar oneOverSix = 0.166666666666666667;
   const scalar oneOverEight = 0.12500000000000000; 
   const scalar sphereFactor = 4.0 * 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148 / 3.0;
}

/**
 * A simple 3 dimensional vector (x, y, z)
 */
typedef struct {
    scalar x, y, z;
} vector3;

/** arr3 will hopefully substitute vector3 one day */ 
typedef scalar arr3[3]; 
typedef geoscalar grr3[3]; 

/**
 * Defines a 12 vector (just for ease of use, clarity and compiler type checking)
 */
typedef scalar vector12[12];

/**
 * Defines a 12x12 matrix
 */
typedef scalar matrix12[12][12];

/**
 * Defines a 3x3 matrix
 */
typedef scalar matrix3[3][3];

/**
 * Defines a 4x4 matrix
 */
typedef scalar matrix4[4][4];

/**
 * A useful type for holding the upper triangular part of symmetric 4x4 matrices
 */
typedef struct {
    scalar u00, u01, u02, u03,
    u11, u12, u13,
    u22, u23,
    u33;
} upper_triangular_matrix4;
#endif
