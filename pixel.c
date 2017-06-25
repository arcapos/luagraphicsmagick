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

/* GraphicsMagick PixelWand for Lua */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <wand/magick_wand.h>

#include "luagraphicsmagick.h"

static int
clone(lua_State *L)
{
	PixelWand **pw, **pw2;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	pw2 = lua_newuserdata(L, sizeof(PixelWand *));
	*pw2 = ClonePixelWand(*pw);
	luaL_setmetatable(L, PIXEL_WAND_METATABLE);
	return 1;
}

static int
destroy(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	if (*pw) {
		DestroyPixelWand(*pw);
		*pw = NULL;
	}
	return 0;
}

static int
getBlack(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetBlack(*pw));
	return 1;
}

static int
getBlackQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetBlackQuantum(*pw));
	return 1;
}

static int
getBlue(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetBlue(*pw));
	return 1;
}

static int
getBlueQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetBlueQuantum(*pw));
	return 1;
}

static int
getColorAsString(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushstring(L, PixelGetColorAsString(*pw));
	return 1;
}

static int
getColorCount(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetColorCount(*pw));
	return 1;
}

static int
getCyan(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetCyan(*pw));
	return 1;
}

static int
getCyanQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetCyanQuantum(*pw));
	return 1;
}

static int
getGreen(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetGreen(*pw));
	return 1;
}

static int
getGreenQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetGreenQuantum(*pw));
	return 1;
}

static int
getMagenta(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetMagenta(*pw));
	return 1;
}

static int
getMagentaQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetMagentaQuantum(*pw));
	return 1;
}

static int
getOpacity(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetOpacity(*pw));
	return 1;
}

static int
getOpacityQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetOpacityQuantum(*pw));
	return 1;
}

static int
getRed(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetRed(*pw));
	return 1;
}

static int
getRedQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetRedQuantum(*pw));
	return 1;
}

static int
getYellow(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushnumber(L, PixelGetYellow(*pw));
	return 1;
}

static int
getYellowQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelGetYellowQuantum(*pw));
	return 1;
}

static int
setBlack(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetBlack(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setBlackQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetBlackQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setBlue(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetBlue(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setBlueQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetBlueQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}


static int
setColor(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, PixelSetColor(*pw, luaL_checkstring(L, 2)));
	return 1;
}

static int
setColorCount(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetColorCount(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setCyan(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetCyan(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setCyanQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetCyanQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setGreen(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetGreen(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setGreenQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetGreenQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setMagenta(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetMagenta(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setMagentaQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetMagentaQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setOpacity(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetOpacity(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setOpacityQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetOpacityQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setRed(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetRed(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setRedQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetRedQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

static int
setYellow(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetYellow(*pw, luaL_checknumber(L, 2));
	return 0;
}

static int
setYellowQuantum(lua_State *L)
{
	PixelWand **pw;

	pw = luaL_checkudata(L, 1, PIXEL_WAND_METATABLE);
	PixelSetYellowQuantum(*pw, luaL_checkinteger(L, 2));
	return 0;
}

struct luaL_Reg pixel_wand_methods[] = {
	{ "clone",		clone },
	{ "destroy",		destroy },
	{ "getBlack",		getBlack },
	{ "getBlackQuantum",	getBlackQuantum },
	{ "getBlue",		getBlue },
	{ "getBlueQuantum",	getBlueQuantum },
	{ "getColorAsString",	getColorAsString },
	{ "getColorCount",	getColorCount },
	{ "getCyan",		getCyan },
	{ "getCyanQuantum",	getCyanQuantum }, 
	{ "getGreen",		getGreen },
	{ "getGreenQuantum",	getGreenQuantum },
	{ "getMagenta",		getMagenta },
	{ "getMagentaQuantum",	getMagentaQuantum },
	{ "getOpacity",		getOpacity },
	{ "getOpacityQuantum",	getOpacityQuantum },
	{ "getRed",		getRed },
	{ "getRedQuantum",	getRedQuantum },
	{ "getYellow",		getYellow },
	{ "getYellowQuantum",	getYellowQuantum },
	{ "setBlack",		setBlack },
	{ "setBlackQuantum",	setBlackQuantum },
	{ "setBlue",		setBlue },
	{ "setBlueQuantum",	setBlueQuantum },
	{ "setColor",		setColor },
	{ "setColorCount",	setColorCount },
	{ "setCyan",		setCyan },
	{ "setCyanQuantum",	setCyanQuantum },
	{ "setGreen",		setGreen },
	{ "setGreenQuantum",	setGreenQuantum },
	{ "setMagenta",		setMagenta },
	{ "setMagentaQuantum",	setMagentaQuantum },
	{ "setOpacity",		setOpacity },
	{ "setOpacityQuantum",	setOpacityQuantum },
	{ "setRed",		setRed },
	{ "setRedQuantum",	setRedQuantum },
	{ "setYellow",		setYellow },
	{ "setYellowQuantum",	setYellowQuantum },
	{ "__gc",		destroy },
	{ NULL, NULL }
};

