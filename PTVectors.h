/******************************************************************************
* 
*     PTVectors.h
*     Copywright (C) 2018 Adam Jackson
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef PTVECTORS_H_INCLUDED
#define PTVECTORS_H_INCLUDED

#include <cmath>
#include <utility>

typedef float VectorPrecision;

constexpr VectorPrecision radiansToDegrees(VectorPrecision radians)
{
 return radians/M_PI*180.0;
}

constexpr VectorPrecision degreesToRadians(VectorPrecision degrees)
{
 return degrees/180.0*M_PI;
}

// Define TVector
struct TVector
{
 VectorPrecision xEast;
 VectorPrecision yNorth;
 VectorPrecision zUp;

 TVector& operator+=(const TVector &x)
 {
  xEast += x.xEast;
  yNorth += x.yNorth;
  zUp += x.zUp;
  return *this;
 }

 TVector& operator-=(const TVector &x)
 {
  xEast -= x.xEast;
  yNorth -= x.yNorth;
  zUp -= x.zUp;
  return *this;
 }

 TVector& operator*=(VectorPrecision x)
 {
  xEast *= x;
  yNorth *= x;
  zUp *= x;
  return *this;
 }

 void calculateUpAndRight(TVector &up, TVector &right) const;
};

// Define some number literal types. specifying a vector constant in code has never been easier
// TVector zAxis = 1_z;
// TVector pythagoreanPoint = 3_x + 4_y;
// TVector specificPoint = 3.14159_x + 2.71828_y
// TVector pointInSpace = 5_mNorth + 2.30_mEast + 30_mUp

constexpr TVector operator"" _x (long double x)
{
 return {static_cast<VectorPrecision>(x), 0.0, 0.0};
}

constexpr TVector operator"" _y (long double y)
{
 return {0.0, static_cast<VectorPrecision>(y), 0.0};
}

constexpr TVector operator"" _z (long double z)
{
 return {0.0, 0.0, static_cast<VectorPrecision>(z)};
}

constexpr TVector operator"" _mNorth(long double m)
{
 return {static_cast<VectorPrecision>(m), 0.0, 0.0};
}

constexpr TVector operator"" _mEast(long double m)
{
 return {0.0, static_cast<VectorPrecision>(m), 0.0};
}

constexpr TVector operator"" _mSouth(long double m)
{
 return {-static_cast<VectorPrecision>(m), 0.0, 0.0};
}

constexpr TVector operator"" _mWest(long double m)
{
 return {0.0, -static_cast<VectorPrecision>(m), 0.0};
}

constexpr TVector operator"" _mUp(long double m)
{
 return {0.0, 0.0, static_cast<VectorPrecision>(m)};
}

constexpr TVector operator"" _mDown(long double m)
{
 return {0.0, 0.0, -static_cast<VectorPrecision>(m)};
}

constexpr TVector operator"" _x (long long unsigned int x)
{
 return {static_cast<VectorPrecision>(x), 0.0, 0.0};
}

constexpr TVector operator"" _y (long long unsigned int y)
{
 return {0.0, static_cast<VectorPrecision>(y), 0.0};
}

constexpr TVector operator"" _z (long long unsigned int z)
{
 return {0.0, 0.0, static_cast<VectorPrecision>(z)};
}

constexpr TVector operator"" _mNorth(long long unsigned int m)
{
 return {static_cast<VectorPrecision>(m), 0.0, 0.0};
}

constexpr TVector operator"" _mEast(long long unsigned int m)
{
 return {0.0, static_cast<VectorPrecision>(m), 0.0};
}

constexpr TVector operator"" _mSouth(long long unsigned int m)
{
 return {-static_cast<VectorPrecision>(m), 0.0, 0.0};
}

constexpr TVector operator"" _mWest(long long unsigned int m)
{
 return {0.0, -static_cast<VectorPrecision>(m), 0.0};
}

constexpr TVector operator"" _mUp(long long unsigned int m)
{
 return {0.0, 0.0, static_cast<VectorPrecision>(m)};
}

constexpr TVector operator"" _mDown(long long unsigned int m)
{
 return {0.0, 0.0, -static_cast<VectorPrecision>(m)};
}

// Operators for TVector
constexpr TVector operator+(const TVector &x)
{
 return x;
}

constexpr TVector operator-(const TVector &x)
{
 return {-x.xEast, -x.yNorth, -x.zUp};
}

constexpr TVector operator+(const TVector &lhs, const TVector &rhs)
{
 return {lhs.xEast + rhs.xEast,
         lhs.yNorth + rhs.yNorth,
         lhs.zUp + rhs.zUp};
}

constexpr TVector operator-(const TVector &lhs, const TVector &rhs)
{
 return {lhs.xEast - rhs.xEast,
         lhs.yNorth - rhs.yNorth,
         lhs.zUp - rhs.zUp};
}

// operator/ will compute the cross product
// operator* will compute the dot product or scale the vector

constexpr TVector operator/(const TVector &lhs, const TVector &rhs)
{
 return {lhs.yNorth*rhs.zUp - lhs.zUp*rhs.yNorth,
         lhs.zUp*rhs.xEast - lhs.xEast*rhs.zUp,
         lhs.xEast*rhs.yNorth - lhs.yNorth*rhs.xEast};
}

constexpr VectorPrecision operator*(const TVector &lhs, const TVector &rhs)
{
 return lhs.xEast*rhs.xEast + lhs.yNorth*rhs.yNorth + lhs.zUp*rhs.zUp;
}

constexpr TVector operator*(VectorPrecision lhs, const TVector &rhs)
{
 return {lhs*rhs.xEast, lhs*rhs.yNorth, lhs*rhs.zUp};
}

constexpr TVector operator*(const TVector &lhs, VectorPrecision rhs)
{
 return rhs * lhs;
}

constexpr bool operator==(const TVector &lhs, const TVector &rhs)
{
 return (lhs.xEast == rhs.xEast) && (lhs.yNorth == rhs.yNorth) && (lhs.zUp == rhs.zUp);
}

constexpr bool operator!=(const TVector &lhs, const TVector &rhs)
{
 return (lhs.xEast != rhs.xEast) || (lhs.yNorth != rhs.yNorth) || (lhs.zUp != rhs.zUp);
}

TVector TVectorSLERP(const TVector &unitStart, const TVector &unitFinish, VectorPrecision slerp);

constexpr TVector rotateTVectorAboutAxis(const TVector &v, const TVector &axis, VectorPrecision angleRadians)
{
 return v*cos(angleRadians) + (axis/v)*sin(angleRadians) + axis*(1.0 - cos(angleRadians))*(axis*v);
}

constexpr TVector rotateTVectorAboutPointAxis(const TVector &v,
                                                  const TVector &origin,
                                                  const TVector &axis,
                                                  VectorPrecision angleRadians)
{
 return rotateTVectorAboutAxis(v - origin, axis, angleRadians) + origin;
}

struct PVector
{
 VectorPrecision u;
 VectorPrecision v;

 PVector& operator+=(const PVector &x)
 {
  u += x.u;
  v += x.v;
  return *this;
 }

 PVector& operator-=(const PVector &x)
 {
  u -= x.u;
  v -= x.v;
  return *this;
 }

 PVector& operator*=(VectorPrecision x)
 {
  u *= x;
  v *= x;
  return *this;
 }
};

constexpr PVector unitVectorAtAngle(VectorPrecision theta)
{
 return {VectorPrecision(cos(theta)), VectorPrecision(sin(theta))};
}

// Literal types for plane vector
// PVector uAxis = 1_u;
// PVector floatingHome = 54.3304_u - 138.844_v
// PVector quadrantOne = 45_deg*30;

constexpr PVector operator"" _u (long double x)
{
 return {static_cast<VectorPrecision>(x), 0.0};
}

constexpr PVector operator"" _v (long double x)
{
 return {0.0, static_cast<VectorPrecision>(x)};
}

constexpr PVector operator"" _deg(long double deg)
{
 return unitVectorAtAngle(degreesToRadians(deg));
}

constexpr PVector operator"" _u (long long unsigned int x)
{
 return {static_cast<VectorPrecision>(x), 0.0};
}

constexpr PVector operator"" _v (long long unsigned int x)
{
 return {0.0, static_cast<VectorPrecision>(x)};
}

constexpr PVector operator"" _deg(long long unsigned int deg)
{
 return unitVectorAtAngle(degreesToRadians(deg));
}

// Operators for plane vector
constexpr PVector operator+(const PVector &v)
{
 return v;
}

constexpr PVector operator-(const PVector &v)
{
 return {-v.u, -v.v};
}

// ~ will calculate the conjugate vector (mirrored about the x-axis)
constexpr PVector operator~(const PVector &v)
{
 return {v.u, -v.v};
}

constexpr PVector operator+(const PVector &lhs, const PVector &rhs)
{
 return {lhs.u + rhs.u, lhs.v + rhs.v};
}

constexpr PVector operator-(const PVector &lhs, const PVector &rhs)
{
 return {lhs.u - rhs.u, lhs.v - rhs.v};
}

constexpr VectorPrecision operator*(const PVector &lhs, const PVector &rhs)
{
 return lhs.u*rhs.u + lhs.v*rhs.v;
}

constexpr PVector operator*(const PVector &lhs, VectorPrecision rhs)
{
 return {lhs.u*rhs, lhs.v*rhs};
}

constexpr PVector operator*(VectorPrecision lhs, const PVector &rhs)
{
 return rhs * lhs;
}

constexpr bool operator==(const PVector &lhs, const PVector &rhs)
{
 return (lhs.u == rhs.u) && (lhs.v == rhs.v);
}

constexpr bool operator!=(const PVector &lhs, const PVector &rhs)
{
 return (lhs.u != rhs.u) || (lhs.v != rhs.v);
}

constexpr PVector rotatePVectorAboutOrigin(const PVector &v, VectorPrecision angleRadians)
{
 return {VectorPrecision(v.u*cos(angleRadians) - v.v*sin(angleRadians)),
         VectorPrecision(v.u*sin(angleRadians) + v.v*cos(angleRadians))};
}

constexpr PVector rotatePVectorAboutPoint(const PVector &v, const PVector &point, VectorPrecision angleRadians)
{
 return rotatePVectorAboutOrigin(v - point, angleRadians) + point;
}

constexpr PVector rotatePVectorLeft(const PVector &v)
{
 return {-v.v, v.u};
}

constexpr PVector rotatePVectorRight(const PVector &v)
{
 return {v.v, -v.u};
}

constexpr VectorPrecision planeVectorAngle(const PVector &v)
{
 return atan2(v.v, v.u);
}

// Absolute value functions
constexpr VectorPrecision abs(const PVector &v)
{
 return hypot(v.u, v.v);
}

constexpr VectorPrecision abs(const TVector &v)
{
 return hypot(hypot(v.xEast, v.yNorth), v.zUp);
}

// Template operators that work on both types
template <typename T>
constexpr T unitVector(const T &v)
{
 return (abs(v) == 0.0) ? v : v * (1.0 / abs(v));
}

template <typename T>
constexpr VectorPrecision angleBetweenVectors(const T &a, const T &b)
{
 return acos(unitVector(a)*unitVector(b));
}

template <typename T>
constexpr T LERP(const T &start, const T &finish, VectorPrecision lerp)
{
 return (start*(1.0 - lerp)) + (finish*lerp);
}

#endif // PTVECTORS_H_INCLUDED
