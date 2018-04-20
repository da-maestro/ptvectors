/******************************************************************************
* 
*     PTVectors.cpp
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


#include "PTVectors.h"

void TVector::calculateUpAndRight(TVector &up, TVector &right) const
{
 if (xEast == 0.0 && yNorth == 0.0)
 {
  if (zUp == 0.0)
  {
   up = 0_y;
   right = 0_x;
  }
  else if (zUp > 0.0)
  {
   up = 1_y;
   right = 1_x;
  }
  else
  {
   up = -1_y;
   right = -1_x;
  }
 }
 else
 {
  right = unitVector(*this/1_z);
  up = unitVector(right/(*this));
 }
}

TVector TVectorSLERP(const TVector &unitStart, const TVector &unitFinish, VectorPrecision slerp)
{
 if (unitStart == unitFinish) return unitStart;

 VectorPrecision t1 = unitStart*unitFinish;
 VectorPrecision recipSinOmega = 1.0 / sqrt(1.0 - t1*t1);
 VectorPrecision omega = acos(t1);
 t1 = sin(omega*(1.0 - slerp))*recipSinOmega;
 VectorPrecision t2 = sin(omega*slerp)*recipSinOmega;
 return t1*unitStart + t2*unitFinish;
}
