#pragma once

#include <cmath>
#include <limits>

namespace sv {
///-----------------------------------------------------------------------------
/// This typedef is used throughout the math classes. It allows the programmer
/// to easily change the precision of math and physics calculations.
///-----------------------------------------------------------------------------
typedef float scalar;

///-----------------------------------------------------------------------------
/// Maximum value of a scalar.
///-----------------------------------------------------------------------------
const float SCALAR_MAX = std::numeric_limits<scalar>::max();

///-----------------------------------------------------------------------------
/// Precision of the power operator.
///-----------------------------------------------------------------------------
#define SCALAR_POW powf

///-----------------------------------------------------------------------------
/// Precision of the abs operator.
///-----------------------------------------------------------------------------
#define SCALAR_ABS fabs

///-----------------------------------------------------------------------------
/// Precision of the square root operator.
///-----------------------------------------------------------------------------
#define SCALAR_SQRT sqrtf

///-----------------------------------------------------------------------------
/// How 'close' two floats need to be in order to be considered equal.
///-----------------------------------------------------------------------------
const float FLOAT_EPSILON = 10.0e-6f;

///-----------------------------------------------------------------------------
/// Utility function for comparing two floats, allowing for an error of
/// FLOAT_EPSILON.
///-----------------------------------------------------------------------------
bool floatsAreEqual(float f1, float f2) {
    return (fabs(f1 - f2) <= FLOAT_EPSILON);
}
}
