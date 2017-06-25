/*
 * Copyright (c) 2016, 2017 Micro Systems Marc Balmer, CH-5073 Gipf-Oberfrick.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Micro Systems Marc Balmer nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* GraphicsMagick DrawingWand for Lua */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <wand/magick_wand.h>

#include "luagraphicsmagick.h"

static int
annotation(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawAnnotation(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checkstring(L, 4));
	return 0;
}

static double
getdouble(lua_State *L, int index, const char *k)
{
	double val;
	char errmsg[32];

	if (lua_getfield(L, index, k) != LUA_TNUMBER) {
		snprintf(errmsg, sizeof errmsg, "uindex '%s' is missing", k);
		return luaL_argerror(L, index, errmsg);
	}
	val = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return val;
}

static int
affine(lua_State *L)
{
	DrawingWand **dw;
	AffineMatrix am;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	if (!lua_istable(L, 2))
		return luaL_argerror(L, 2, "table expected");
	am.sx = getdouble(L, 2, "sx");
	am.rx = getdouble(L, 2, "rx");
	am.ry = getdouble(L, 2, "ry");
	am.sy = getdouble(L, 2, "sy");
	am.tx = getdouble(L, 2, "tx");
	am.ty = getdouble(L, 2, "ty");
	DrawAffine(*dw, &am);
	return 0;
}

static int
arc(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawArc(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5),
	    luaL_checknumber(L, 6), luaL_checknumber(L, 7));
	return 0;
}

static int
bezier(lua_State *L)
{
	DrawingWand **dw;
	PointInfo *points;
	unsigned long ncoord;
	int n;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	if (!lua_istable(L, 2))
		return luaL_argerror(L, 2, "table expected");

	ncoord = lua_gettop(L) - 1;
	points = malloc(ncoord * sizeof(PointInfo));

	for (n = 2; n < ncoord + 2; n++) {
		points[n - 2].x = getdouble(L, n, "x");
		points[n - 2].y = getdouble(L, n, "y");
	}

	DrawBezier(*dw, ncoord, points);
	free(points);
	return 0;
}

static int
circle(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawCircle(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;	
}

static int
clearException(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushboolean(L, DrawClearException(*dw));
	return 1;
}

static int
getClipPath(lua_State *L)
{
	DrawingWand **dw;
	char *clip_path;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	clip_path = DrawGetClipPath(*dw);
	lua_pushstring(L, clip_path);
	free(clip_path);
	return 1;
}

static int
setClipPath(lua_State *L)
{
	DrawingWand **dw;
	char *clip_path;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetClipPath(*dw, luaL_checkstring(L, 2));
	return 0;
}

static const char *const fill_rules[] = {
	"UndefinedRule",
	"EvenOddRule",
	"NonZeroRule",
	NULL
};

static int
getClipRule(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, fill_rules[DrawGetClipRule(*dw)]);
	return 1;
}

static int
setClipRule(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetClipRule(*dw, luaL_checkoption(L, 2, "UndefinedRule", 
	    fill_rules));
	return 0;
}

static const char *const clip_path_units[] = {
	"UserSpace",
	"UserSpaceOnUse",
	"ObjectBoundingBox",
	NULL
};

static int
getClipUnits(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, clip_path_units[DrawGetClipUnits(*dw)]);
	return 1;
}

static int
getException(lua_State *L)
{
	DrawingWand **dw;
	ExceptionType severity;
	char *text;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	text = DrawGetException(*dw, &severity);
	lua_pushstring(L, text);
	lua_pushinteger(L, severity);
	free(text);
	return 2;
}

static int
setClipUnits(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetClipUnits(*dw, luaL_checkoption(L, 2, "UserSpace",
	    clip_path_units));
	return 0;
}

static int
clone(lua_State *L)
{
	DrawingWand **dw, **dw2;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	dw2 = lua_newuserdata(L, sizeof(DrawingWand *));
	*dw2 = CloneDrawingWand(*dw);
	luaL_setmetatable(L, DRAWING_WAND_METATABLE);
	return 1;
}

static const char *const paint_methods[] = {
	"PointMethod",
	"ReplaceMethod",
	"FloodfillMethod",
	"FillToBorderMethod",
	"ResetMethod",
	"PaintMethod",
	NULL
};

static int
color(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawColor(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checkoption(L, 4, "PointMethod", paint_methods));
	return 0;
}

static int
comment(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawComment(*dw, luaL_checkstring(L, 2));
	return 0;	
}

static int
ellipse(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawEllipse(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5),
	    luaL_checknumber(L, 6), luaL_checknumber(L, 7));
	return 0;
}

static int
destroy(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	if (*dw) {
		DestroyDrawingWand(*dw);
		*dw = NULL;
	}
	return 0;
}

static int
getFillColor(lua_State *L)
{
	DrawingWand **dw;
	PixelWand **pw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	pw = lua_newuserdata(L, sizeof(PixelWand *));
	DrawGetFillColor(*dw, *pw);
	luaL_setmetatable(L, PIXEL_WAND_METATABLE);
	return 1;
}

static int
setFillColor(lua_State *L)
{
	DrawingWand **dw;
	PixelWand **pw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	DrawSetFillColor(*dw, *pw);
	return 0;
}

static int
setFillPatternURL(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFillPatternURL(*dw, luaL_checkstring(L, 2));
	return 0;
}

static int
getFillOpacity(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushnumber(L, DrawGetFillOpacity(*dw));
	return 1;
}

static int
setFillOpacity(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFillOpacity(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
getFillRule(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, fill_rules[DrawGetFillRule(*dw)]);
	return 1;
}

static int
setFillRule(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFillRule(*dw, luaL_checkoption(L, 2, "UndefinedRule", 
	    fill_rules));
	return 0;
}

static int
getFont(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, DrawGetFont(*dw));
	return 1;
}

static int
setFont(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFont(*dw, luaL_checkstring(L, 2));
	return 0;
}

static int
getFontFamily(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, DrawGetFontFamily(*dw));
	return 1;
}

static int
setFontFamily(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFontFamily(*dw, luaL_checkstring(L, 2));
	return 0;
}

static int
getFontSize(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushnumber(L, DrawGetFontSize(*dw));
	return 1;
}

static int
setFontSize(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFontSize(*dw, luaL_checknumber(L, 2));
	return 0;
}

static const char *const stretch_types[] = {
	"NormalStretch",
	"UltraCondensedStretch",
	"ExtraCondensedStretch",
	"CondensedStretch",
	"SemiCondensedStretch",
	"SemiExpandedStretch",
	"ExpandedStretch",
	"ExtraExpandedStretch",
	"UltraExpandedStretch",
	"AnyStretch",
	NULL
};

static int
getFontStretch(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, stretch_types[DrawGetFontStretch(*dw)]);
	return 1;
}
	
static int
setFontStretch(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFontStretch(*dw, luaL_checkoption(L, 2, "NormalStretch", 
	    stretch_types));
	return 0;
}

static const char *const styles[] = {
	"NormalStyle",
	"ItalicStyle",
	"ObliqueStyle",
	"AnyStyle",
	NULL
};

static int
getFontStyle(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, styles[DrawGetFontStyle(*dw)]);
	return 1;
}
	
static int
setFontStyle(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFontStyle(*dw, luaL_checkoption(L, 2, "AnyStyle", styles));
	return 0;
}

static int
getFontWeight(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushinteger(L, DrawGetFontWeight(*dw));
	return 1;
}

static int
setFontWeight(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetFontWeight(*dw, luaL_checkinteger(L, 2));
	return 0;
}

static const char *const gravity[] = {
	"ForgetGravity",
	"NorthWestGravity",
	"NorthGravity",
	"NorthEastGravity",
	"WestGravity",
	"CenterGravity",
	"EastGravity",
	"SouthWestGravity",
	"SouthGravity",
	"SouthEastGravity",
	"StaticGravity",
	NULL
};

static int
getGravity(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	lua_pushstring(L, gravity[DrawGetGravity(*dw)]);
	return 1;
}
	
static int
setGravity(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetGravity(*dw, luaL_checkoption(L, 2, "ForgetGravity", gravity));
	return 0;
}

static int
line(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawLine(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int
matte(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawMatte(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checkoption(L, 4, "PointMethod", paint_methods));
	return 0;
}

static int
pathClose(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathClose(*dw);
	return 0;
}

static int
pathCurveToAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToAbsolute(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	    luaL_checknumber(L, 5), luaL_checknumber(L, 6),
	    luaL_checknumber(L, 7));
	return 0;
}

static int
pathCurveToRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToRelative(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	    luaL_checknumber(L, 5), luaL_checknumber(L, 6),
	    luaL_checknumber(L, 7));
	return 0;
}

static int
pathCurveToQuadraticBezierAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToQuadraticBezierAbsolute(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	    luaL_checknumber(L, 5));
	return 0;
}

static int
pathCurveToQuadraticBezierRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToQuadraticBezierRelative(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	    luaL_checknumber(L, 5));
	return 0;
}

static int
pathCurveToQuadraticBezierSmoothAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToQuadraticBezierSmoothAbsolute(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
pathCurveToQuadraticBezierSmoothRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToQuadraticBezierSmoothRelative(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
pathCurveToSmoothAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToSmoothAbsolute(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int
pathCurveToSmoothRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathCurveToSmoothRelative(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int
pathEllipticArcAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathEllipticArcAbsolute(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checkinteger(L, 5),
	    luaL_checkinteger(L, 6), luaL_checknumber(L, 7),
	    luaL_checknumber(L, 8));
	return 0;
}

static int
pathEllipticArcRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathEllipticArcRelative(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checkinteger(L, 5),
	    luaL_checkinteger(L, 6), luaL_checknumber(L, 7),
	    luaL_checknumber(L, 8));
	return 0;
}

static int
pathFinish(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathFinish(*dw);
	return 0;
}

static int
pathLineToAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToAbsolute(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
pathLineToRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToRelative(*dw,
	    luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
pathLineToHorizontalAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToHorizontalAbsolute(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
pathLineToHorizontalRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToHorizontalRelative(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
pathLineToVerticalAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToVerticalAbsolute(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
pathLineToVerticalRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathLineToVerticalRelative(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
pathMoveToAbsolute(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathMoveToAbsolute(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3));
	return 0;
}

static int
pathMoveToRelative(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathMoveToRelative(*dw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3));
	return 0;
}

static int
pathStart(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPathStart(*dw);
	return 0;
}

static int
point(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPoint(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
polygon(lua_State *L)
{
	DrawingWand **dw;
	PointInfo *points;
	unsigned long ncoord;
	int n;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	if (!lua_istable(L, 2))
		return luaL_argerror(L, 2, "table expected");

	ncoord = lua_gettop(L) - 1;
	points = malloc(ncoord * sizeof(PointInfo));

	for (n = 2; n < ncoord + 2; n++) {
		points[n - 2].x = getdouble(L, n, "x");
		points[n - 2].y = getdouble(L, n, "y");
	}

	DrawPolygon(*dw, ncoord, points);
	free(points);
	return 0;
}

static int
polyline(lua_State *L)
{
	DrawingWand **dw;
	PointInfo *points;
	unsigned long ncoord;
	int n;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	if (!lua_istable(L, 2))
		return luaL_argerror(L, 2, "table expected");

	ncoord = lua_gettop(L) - 1;
	points = malloc(ncoord * sizeof(PointInfo));

	for (n = 2; n < ncoord + 2; n++) {
		points[n - 2].x = getdouble(L, n, "x");
		points[n - 2].y = getdouble(L, n, "y");
	}

	DrawPolyline(*dw, ncoord, points);
	free(points);
	return 0;
}

static int
popClipPath(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPopClipPath(*dw);
	return 0;
}

static int
popDefs(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPopDefs(*dw);
	return 0;
}

static int
popGraphicContext(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPopGraphicContext(*dw);
	return 0;
}

static int
popPattern(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPopPattern(*dw);
	return 0;
}

static int
pushClipPath(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPushClipPath(*dw, luaL_checkstring(L, 2));
	return 0;
}

static int
pushDefs(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPushDefs(*dw);
	return 0;
}

static int
pushGraphicContext(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPushGraphicContext(*dw);
	return 0;
}

static int
pushPattern(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawPushPattern(*dw, luaL_checkstring(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5),
	    luaL_checknumber(L, 6));
	return 0;
}

static int
rectangle(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawRectangle(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int
rotate(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawRotate(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
roundRectangle(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawRoundRectangle(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5),
	    luaL_checknumber(L, 6), luaL_checknumber(L, 7));
	return 0;
}

static int
scale(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawScale(*dw, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int
skewX(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSkewX(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
skewY(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSkewY(*dw, luaL_checknumber(L, 2));
	return 0;
}

static int
setStrokeColor(lua_State *L)
{
	DrawingWand **dw;
	PixelWand **pw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	DrawSetStrokeColor(*dw, *pw);
	return 0;
}

static const char *const decorations[] = {
	"NoDecoration",
	"UnderlineDecoration",
	"OverlineDecoration",
	"LineThroughDecoration",
	NULL
};

static int
setTextDecoration(lua_State *L)
{
	DrawingWand **dw;

	dw = luaL_checkudata(L, 1, DRAWING_WAND_METATABLE);
	DrawSetTextDecoration(*dw,
	    luaL_checkoption(L, 2, "NoDecoration", decorations));
	return 0;
}

struct luaL_Reg drawing_wand_methods[] = {
	{ "annotation",		annotation },
	{ "affine",		affine },
	{ "arc",		arc },
	{ "bezier",		bezier },
	{ "circle",		circle },
	{ "clearException",	clearException },
	{ "getClipPath",	getClipPath },
	{ "setClipPath",	setClipPath },
	{ "getClipRule",	getClipRule },
	{ "setClipRule",	setClipRule },
	{ "getClipUnits",	getClipUnits },
	{ "getException",	getException },
	{ "setClipUnits",	setClipUnits },
	{ "color",		color },
	{ "comment",		comment },
	{ "destroy",		destroy },
	{ "ellipse",		ellipse },
	{ "getFillColor",	getFillColor },
	{ "setFillColor",	setFillColor },
	{ "getFillOpacity",	getFillOpacity },
	{ "setFillOpacity",	setFillOpacity },
	{ "getFillRule",	getFillRule },
	{ "setFillRule",	setFillRule },
	{ "getFont",		getFont },
	{ "setFont",		setFont },
	{ "getFontFamily",	getFontFamily },
	{ "setFontFamily",	setFontFamily },
	{ "getFontSize",	getFontSize },
	{ "setFontSize",	setFontSize },
	{ "getFontStretch",	getFontStretch },
	{ "setFontStretch",	setFontStretch },
	{ "getFontStyle",	getFontStyle },
	{ "setFontStyle",	setFontStyle },
	{ "getFontWeight",	getFontWeight },
	{ "setFontWeight",	setFontWeight },
	{ "getGravity",		getGravity },
	{ "setGravity",		setGravity },
	{ "line",		line },
	{ "matte",		matte },
	{ "pathClose",		pathClose },
	{ "pathCurveToAbsolute",
				pathCurveToAbsolute },
	{ "pathCurveToRelative",
				pathCurveToRelative },
	{ "pathCurveToQuadraticBezierAbsolute",
				pathCurveToQuadraticBezierAbsolute },
	{ "pathCurveToQuadraticBezierRelative",
				pathCurveToQuadraticBezierRelative },
	{ "pathCurveToQuadraticBezierSmoothAbsolute",
				pathCurveToQuadraticBezierSmoothAbsolute },
	{ "pathCurveToQuadraticBezierSmoothRelative",
				pathCurveToQuadraticBezierSmoothRelative },
	{ "pathCurveToSmoothAbsolute",
				pathCurveToSmoothAbsolute },
	{ "pathCurveToSmoothRelative",
				pathCurveToSmoothRelative },
	{ "pathEllipticArcAbsolute",
				pathEllipticArcAbsolute },
	{ "pathEllipticArcRelative",
				pathEllipticArcRelative },
	{ "pathFinish",		pathFinish },
	{ "pathLineToAbsolute",	pathLineToRelative },
	{ "pathLineToHorizontalAbsolute",
				pathLineToHorizontalAbsolute },
	{ "pathLineToHorizontalRelative",
				pathLineToHorizontalRelative },
	{ "pathLineToVerticalAbsolute",
				pathLineToVerticalAbsolute },
	{ "pathLineToVerticalRelative",
				pathLineToVerticalRelative },
	{ "pathMoveToAbsolute",	pathMoveToAbsolute },
	{ "pathMoveToRelative",	pathMoveToRelative },
	{ "pathStart",		pathStart },
	{ "point",		point },
	{ "polygon",		polygon },
	{ "polyline",		polyline },
	{ "popClipPath",	popClipPath },
	{ "popDefs",		popDefs },
	{ "popGraphicContext",	popGraphicContext },
	{ "popPattern",		popPattern },
	{ "pushClipPath",	pushClipPath },
	{ "pushDefs",		pushDefs },
	{ "pushGraphicContext",	pushGraphicContext },
	{ "pushPattern",	pushPattern },
	{ "rectangle",		rectangle },
	{ "rotate",		rotate },
	{ "roundRectangle",	roundRectangle },
	{ "scale",		scale },
	{ "skewX",		skewX },
	{ "skewY",		skewY },
	{ "setStrokeColor",	setStrokeColor },
	{ "setTextDecoration",	setTextDecoration },
	{ "__gc",		destroy },
	{ NULL, NULL }
};

