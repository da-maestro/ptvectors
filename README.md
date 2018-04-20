# ptvectors
====================================================

PTVectors inc. LuaVectorLib
Copywright (C) 2018 Adam Jackson

====================================================

TODO: create documentation for LuaVectorLib



## Overview and Installation

PTVectors is a spacial vector mathematics library for C++11. It is written to be almost entirely constexpr, meaning that PTVectors supports compile time manipulation of vectors and deep optimisation. PTVectors also supports inline vector constants using the new C++11 operator"" feature.

PTVectors is small enough to include directly into any C++11 project and has been tested with GCC 5.4.0. PTVectors also includes LuaVectorLib; a library for Lua 5.3.0 to manipulate PTVectors within Lua code. Like PTVectors, LuaVectorLib is small enough to include directly into your project. PTVectors can be used with or without LuaVectorLib.

PTVectors supports two kinds of vectors:
 * TVector: A 3 dimensional vector.
 * PVector: A 2 dimensional vector.

PTVectors is basically just a header file with a very small C++ that contains two functions. So, as mentioned above, it is meant at this point to be included with the rest of the source for your project and compiled in directly. This means there is no installation nessesary, and your project has no extra dependencies.




## Inline Constants

Inline vector constants enable one to specify constant vectors in source in an intuitive way, using operator"" which is new to C++11. 

    "_x", "_mNorth" - A TVector on the positive X-axis
    "_mSouth"       - A TVector on the negative X-axis
    "_y", "_mEast"  - A TVector on the positive Y-axis
    "_mWest"        - A TVector on the negative Y-axis
    "_z", "_mUp"    - A TVector on the positive Z-axis
    "_mDown"        - A TVector on the negative Z-axis
    "_u"            - A PVector on the positive U-axis
    "_v"            - A PVector on the positive V-axis
    "_deg"          - A unit PVector rotated on the plane.
                   0 degrees points along the +ve u axis
                   90 degrees points along the +ve v axis
                   There is no shorthand way to specify the angle in radians

eg.

    PVector uAxis = 1_u;
    PVector floatingHome = 54.3304_u - 138.844_v
    PVector quadrantOne = 45_deg*30;
    TVector zAxis = 1_z;
    TVector pythagoreanPoint = 3_x + 4_y;
    TVector specificPoint = 3.14159_x - 2.71828_y
    TVector pointInSpace = -5_mNorth + 2.30_mEast + 30_mUp
    



## Operations and Functions

NOTE:
s represents a scalar value
P represents a PVector ONLY
T represents a TVector ONLY
V represents any vector
Unless noted otherwise, all operations are constexpr


### Unary operators

 +V             returns V unchanged

 -V             returns V in the opposite direction

 ~P             returns the conjugate of P
 
 abs(V)         returns the length of the vector
 
 unitVector(V)  returns a unit vector in the same direction as V


### Binary operators

 V + V                          returns the sum

 V - V                          returns the difference

 T / T                          returns the cross product

 V * V                          returns the dot product

 s * V                          returns the vector scaled
 V * s
 
 V == V                         returns true if the two vectors are exactly equal
 
 V != V                         returns true if the two vectors are not exactly equal
 
 angleBetweenVectors(V, V)      returns the angle between the vectors in radians
 

### Ternary operators

 LERP(V, V, s)  returns the linear interpolation between the vectors by s


### TVector Rotations

rotateTVectorAboutAxis(Tx, Ta, s)

    rotates Tx about the axis Ta by s radians
    follows the right hand rule
 
rotateTVectorAboutPointAxis(Tx, To, Ta, s)

    rotates Tx about an axis originating from To, as above

TVectorSLERP(Ts, Tf, s)

    expects Ts and Tf to be UNIT VECTORS
    returns a unit vector that lies on the unit sphere between Ts and Tf, by s
    NOT constexpr

T.calculateUpandRight(Tu, Tr)

    takes T, and calculates two vectors that point 'up' and 'right'
    up is assumed to follow the positive z-axis
    returns a zero vector if T is a zero vector
    NOT constexpr


### PVector Rotations

rotatePVectorAboutOrigin(P, s)

    rotates P about the origin anti-clockwise by s radians
 
rotatePVectorAboutPoint(Px, Pp, s)

    rotates Px about Pp anti-clockwise by s radians
 
rotatePVectorLeft(P)

    rotates P 90 degrees anti-clockwise
 
rotatePVectorRight(P)

    rotates P 90 degrees clockwise
 
unitVectorAtAngle(s)

    creates a new unit PVector that subtends an angle s from the u-axis in radians

