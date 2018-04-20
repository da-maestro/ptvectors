/******************************************************************************
* 
*     LuaVectorLib.cpp
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


#include "LuaVectorLib.h"
#include <cstdio>

const char vector2Meta[] = "planevector";
const char vector3Meta[] = "navvector";

enum VectorType
{
 NumberType = 0,
 TVectorType,
 PVectorType
};

void luaPushNewTVector(lua_State *L, const TVector &nv)
{
 TVector *v = (TVector*)lua_newuserdata(L, sizeof(TVector));
 luaL_getmetatable(L, vector3Meta);
 lua_setmetatable(L, -2);
 *v = nv;
}

void luaPushNewPVector(lua_State *L, const PVector &pv)
{
 PVector *v = (PVector*)lua_newuserdata(L, sizeof(PVector));
 luaL_getmetatable(L, vector2Meta);
 lua_setmetatable(L, -2);
 *v = pv;
}

bool luaIsPVector(lua_State *L, int arg)
{
 return luaL_testudata(L, arg, vector2Meta) != NULL;
}

bool luaIsTVector(lua_State *L, int arg)
{
 return luaL_testudata(L, arg, vector3Meta) != NULL;
}

TVector luaToTVector(lua_State *L, int arg)
{
 TVector *nv = (TVector*)luaL_checkudata(L, arg, vector3Meta);
 return (nv != nullptr) ? *nv : 0_x;
}

PVector luaToPVector(lua_State *L, int arg)
{
 PVector *pv = (PVector*)luaL_checkudata(L, arg, vector2Meta);
 return (pv != nullptr) ? *pv : 0_u;
}

TVector luaCheckTVector(lua_State *L, int arg)
{
 TVector *nv = (TVector*)luaL_checkudata(L, arg, vector3Meta);
 luaL_argcheck(L, nv != nullptr, arg, "'TVector' expected");
 return *nv;
}

PVector luaCheckPVector(lua_State *L, int arg)
{
 PVector *pv = (PVector*)luaL_checkudata(L, arg, vector2Meta);
 luaL_argcheck(L, pv != nullptr, arg, "'PVector' expected");
 return *pv;
}

int luaGetArgumentType(lua_State *L, int arg)
{
 int nType;
 lua_tonumberx(L, arg, &nType);
 if (nType) return NumberType;

 TVector *pnv = (TVector*)luaL_testudata(L, arg, vector3Meta);
 PVector *ppv = (PVector*)luaL_testudata(L, arg, vector2Meta);

 if (pnv != nullptr) return TVectorType;
 if (ppv != nullptr) return PVectorType;
 luaL_argerror(L, arg, "Expected 'vector' type");
 return 0;
}

int luaGetArgument(lua_State *L, int arg, lua_Number &number, TVector &nv, PVector &pv)
{
 int nType;
 lua_Number result = lua_tonumberx(L, arg, &nType);
 if (nType)
 {
  number = result;
  return NumberType;
 }

 TVector *pnv = (TVector*)luaL_testudata(L, arg, vector3Meta);
 PVector *ppv = (PVector*)luaL_testudata(L, arg, vector2Meta);

 if (pnv != nullptr)
 {
  nv = *pnv;
  return TVectorType;
 }

 if (ppv != nullptr)
 {
  pv = *ppv;
  return PVectorType;
 }
 luaL_argerror(L, arg, "'Vector' expected");
 return 0;
}

static int newVector(lua_State *L)
{
 // check number of parameters and determine what kind of vector we're building
 int noPar = lua_gettop(L);

 int type;
 lua_Number num;
 TVector nv;
 PVector pv;

 switch (noPar)
 {
  default:
   luaL_error(L, "Invalid size of vector");
  break;

  case 1:
   // For one argument, we check the argument type
   // If number, create unit plane vector to angle in radians from x-axis
   // otherwise, if tvector or pvector, make a copy of the vector
   type = luaGetArgument(L, 1, num, nv, pv);
   if (type == NumberType)
   {
    pv = unitVectorAtAngle(num);
    luaPushNewPVector(L, pv);
   }
   else if (type == TVectorType) luaPushNewTVector(L, nv);
   else luaPushNewPVector(L, pv);
  break;

  case 2:
   pv.u = luaL_checknumber(L, 1);
   pv.v = luaL_checknumber(L, 2);
   luaPushNewPVector(L, pv);
  break;

  case 3:
   nv.xEast = luaL_checknumber(L, 1);
   nv.yNorth = luaL_checknumber(L, 2);
   nv.zUp = luaL_checknumber(L, 3);
   luaPushNewTVector(L, nv);
  break;
 }

 return 1;
}

static int negateTVector(lua_State *L)
{
 luaPushNewTVector(L, -luaCheckTVector(L, 1));
 return 1;
}

static int negatePVector(lua_State *L)
{
 luaPushNewPVector(L, -luaCheckPVector(L, 1));
 return 1;
}

static int addTVectors(lua_State *L)
{
 luaPushNewTVector(L, luaCheckTVector(L, 1) + luaCheckTVector(L, 2));
 return 1;
}

static int addPVectors(lua_State *L)
{
 luaPushNewPVector(L, luaCheckPVector(L, 1) + luaCheckPVector(L, 2));
 return 1;
}

static int subTVectors(lua_State *L)
{
 luaPushNewTVector(L, luaCheckTVector(L, 1) - luaCheckTVector(L, 2));
 return 1;
}

static int subPVectors(lua_State *L)
{
 luaPushNewPVector(L, luaCheckPVector(L, 1) - luaCheckPVector(L, 2));
 return 1;
}

static int scaleTVector(lua_State *L)
{
 luaPushNewTVector(L, luaCheckTVector(L, 1) * luaL_checknumber(L, 2));
 return 1;
}

static int scalePVector(lua_State *L)
{
 luaPushNewPVector(L, luaCheckPVector(L, 1) * luaL_checknumber(L, 2));
 return 1;
}

static int navVectorCrossProduct(lua_State *L)
{
 luaPushNewTVector(L, luaCheckTVector(L, 1) / luaCheckTVector(L, 2));
 return 1;
}

static int navVectorDotProduct(lua_State *L)
{
 lua_pushnumber(L, luaCheckTVector(L, 1) * luaCheckTVector(L, 2));
 return 1;
}

static int planeVectorDotProduct(lua_State *L)
{
 lua_pushnumber(L, luaCheckPVector(L, 1) * luaCheckPVector(L, 2));
 return 1;
}

static int vectorMultiplyOperation(lua_State *L)
{
 int arg1Type = luaGetArgumentType(L, 1);
 int arg2Type = luaGetArgumentType(L, 2);

 if (arg1Type == TVectorType && arg2Type == TVectorType) return navVectorDotProduct(L);
 if (arg1Type == PVectorType && arg2Type == PVectorType) return planeVectorDotProduct(L);
 if (arg1Type == TVectorType && arg2Type == NumberType) return scaleTVector(L);
 if (arg1Type == PVectorType && arg2Type == NumberType) return scalePVector(L);
 if (arg1Type == NumberType)
 {
  lua_rotate(L, 1, 1);
  if (arg2Type == TVectorType) return scaleTVector(L);
  if (arg2Type == PVectorType) return scalePVector(L);
 }

 luaL_argerror(L, 2, "Type mismatch (invalid combination of argument types)");
 return 0;
}

static int vectorLength(lua_State *L)
{
 lua_Number arg1number;
 TVector arg1NV;
 PVector arg1PV;
 int arg1Type = luaGetArgument(L, 1, arg1number, arg1NV, arg1PV);
 luaL_argcheck(L, arg1Type != NumberType, 1, "'Vector' expected");

 if (arg1Type == TVectorType) lua_pushnumber(L, abs(arg1NV));
 else lua_pushnumber(L, abs(arg1PV));

 return 1;
}

static int calcUnitVector(lua_State *L)
{
 lua_Number num;
 TVector nv;
 PVector pv;
 int argType = luaGetArgument(L, 1, num, nv, pv);
 luaL_argcheck(L, argType != NumberType, 1, "'Vector' expected");
 if (argType == TVectorType) luaPushNewTVector(L, unitVector(nv));
 else luaPushNewPVector(L, unitVector(pv));

 return 1;
}

static int vectorRotate(lua_State *L)
{
 lua_Number num;
 TVector nv;
 PVector pv;
 int argType = luaGetArgument(L, 1, num, nv, pv);
 luaL_argcheck(L, argType != NumberType, 1, "'Vector' expected");

 if (argType == TVectorType)
  luaPushNewTVector(L, rotateTVectorAboutAxis(nv, luaCheckTVector(L, 2), luaL_checknumber(L, 3)));
 else luaPushNewPVector(L, rotatePVectorAboutOrigin(pv, luaL_checknumber(L, 2)));

 return 1;
}

static int vectorLERP(lua_State *L)
{
 lua_Number num1, num2;
 TVector nv1, nv2;
 PVector pv1, pv2;
 int arg1Type = luaGetArgument(L, 1, num1, nv1, pv1);
 int arg2Type = luaGetArgument(L, 2, num2, nv2, pv2);
 luaL_argcheck(L, arg1Type != NumberType, 1, "'Vector' expected");
 luaL_argcheck(L, arg2Type != NumberType, 2, "'Vector' expected");
 luaL_argcheck(L, arg1Type == arg2Type, 2, "Vector type mismatch");

 lua_Number lerp = luaL_checknumber(L, 3);
 if (arg1Type == TVectorType) luaPushNewTVector(L, LERP(nv1, nv2, lerp));
 else luaPushNewPVector(L, LERP(pv1, pv2, lerp));

 return 1;
}

static int navVectorSLERP(lua_State *L)
{
 TVector nv1 = luaCheckTVector(L, 1);
 TVector nv2 = luaCheckTVector(L, 2);
 lua_Number slerp = luaL_checknumber(L, 3);
 luaPushNewTVector(L, TVectorSLERP(nv1, nv2, slerp));
 return 1;
}

static int navVectorFindUpAndRight(lua_State *L)
{
 TVector up, right;
 luaCheckTVector(L, 1).calculateUpAndRight(up, right);
 luaPushNewTVector(L, up);
 luaPushNewTVector(L, right);
 return 2;
}

static int planeVectorFindConjugate(lua_State *L)
{
 luaPushNewPVector(L, ~luaCheckPVector(L, 1));
 return 1;
}

static int navVectorXComponent(lua_State *L)
{
 lua_pushnumber(L, luaCheckTVector(L, 1).xEast);
 return 1;
}

static int navVectorYComponent(lua_State *L)
{
 lua_pushnumber(L, luaCheckTVector(L, 1).yNorth);
 return 1;
}

static int navVectorZComponent(lua_State *L)
{
 lua_pushnumber(L, luaCheckTVector(L, 1).zUp);
 return 1;
}

static int planeVectorUComponent(lua_State *L)
{
 lua_pushnumber(L, luaCheckPVector(L, 1).u);
 return 1;
}

static int planeVectorVComponent(lua_State *L)
{
 lua_pushnumber(L, luaCheckPVector(L, 1).v);
 return 1;
}

static int angleBetweenTVectors(lua_State *L)
{
 lua_pushnumber(L, angleBetweenVectors(luaCheckTVector(L, 1), luaCheckTVector(L, 2)));
 return 1;
}

static int angleBetweenPVectors(lua_State *L)
{
 lua_pushnumber(L, angleBetweenVectors(luaCheckPVector(L, 1), luaCheckPVector(L, 2)));
 return 1;
}

static int navVectorToString(lua_State *L)
{
 char str[30];
 TVector nv = luaCheckTVector(L, 1);
 snprintf(str, 30, "(%.3f, %.3f, %.3f)", nv.xEast, nv.yNorth, nv.zUp);
 lua_pushstring(L, str);
 return 1;
}

static int planeVectorToString(lua_State *L)
{
 char str[20];
 PVector pv = luaCheckPVector(L, 1);
 snprintf(str, 20, "(%.3f, %.3f)", pv.u, pv.v);
 lua_pushstring(L, str);
 return 1;
}

static const struct luaL_Reg vectorLibMethods[] =
{
 {"new", newVector},
 {"length", vectorLength},
 {"unit", calcUnitVector},
 {"rotate", vectorRotate},
 {"lerp", vectorLERP},
 {NULL, NULL}
};

static const struct luaL_Reg navVectorMetaTable[] =
{
 {"add", addTVectors},
 {"sub", subTVectors},
 {"mul", vectorMultiplyOperation},
 {"cross", navVectorCrossProduct},
 {"length", vectorLength},
 {"unit", calcUnitVector},
 {"rotate", vectorRotate},
 {"lerp", vectorLERP},
 {"slerp", navVectorSLERP},
 {"upAndRight", navVectorFindUpAndRight},
 {"x", navVectorXComponent},
 {"y", navVectorYComponent},
 {"z", navVectorZComponent},
 {"angle", angleBetweenTVectors},
 {"__tostring", navVectorToString},
 {"__add", addTVectors},
 {"__sub", subTVectors},
 {"__mul", vectorMultiplyOperation},
 {"__mod", navVectorCrossProduct},
 {"__unm", negateTVector},
 {"__len", vectorLength},
 {"__bnot", calcUnitVector},
 {NULL, NULL}
};

static const struct luaL_Reg planeVectorMetaTable[] =
{
 {"add", addPVectors},
 {"sub", subPVectors},
 {"mul", vectorMultiplyOperation},
 {"length", vectorLength},
 {"unit", calcUnitVector},
 {"rotate", vectorRotate},
 {"lerp", vectorLERP},
 {"conj", planeVectorFindConjugate},
 {"u", planeVectorUComponent},
 {"v", planeVectorVComponent},
 {"angle", angleBetweenPVectors},
 {"__tostring", planeVectorToString},
 {"__add", addPVectors},
 {"__sub", subPVectors},
 {"__mul", vectorMultiplyOperation},
 {"__unm", negatePVector},
 {"__len", vectorLength},
 {"__bnot", calcUnitVector},
 {NULL, NULL}
};

void openLuaVectorLibrary(lua_State *L)
{
 luaL_newmetatable(L, vector2Meta);
 lua_pushstring(L, "__index");
 lua_pushvalue(L, -2);
 lua_settable(L, -3);
 luaL_setfuncs(L, planeVectorMetaTable, 0);
 lua_pop(L, 1);

 luaL_newmetatable(L, vector3Meta);
 lua_pushstring(L, "__index");
 lua_pushvalue(L, -2);
 lua_settable(L, -3);
 luaL_setfuncs(L, navVectorMetaTable, 0);
 lua_pop(L, 1);

 luaL_newlib(L, vectorLibMethods);
 lua_setglobal(L, "vector");
 if (luaL_dostring(L, "vector.nzero = vector.new(0, 0, 0)\nvector.pzero = vector.new(0, 0)\n"))
 {
  printf("Error running vector lib code\n");
 }
}
