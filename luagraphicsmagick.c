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

/* GraphicsMagick for Lua */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <wand/magick_wand.h>

#include "luagraphicsmagick.h"

static int
newDrawingWand(lua_State *L)
{
	DrawingWand **dw;

	dw = lua_newuserdata(L, sizeof(DrawingWand *));
	*dw = NewDrawingWand();
	luaL_setmetatable(L, DRAWING_WAND_METATABLE);
	return 1;
}

static int
newMagickWand(lua_State *L)
{
	MagickWand **mw;

	mw = lua_newuserdata(L, sizeof(MagickWand *));
	*mw = NewMagickWand();
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
newPixelWand(lua_State *L)
{
	PixelWand **pw;

	pw = lua_newuserdata(L, sizeof(PixelWand *));
	*pw = NewPixelWand();
	luaL_setmetatable(L, PIXEL_WAND_METATABLE);
	return 1;
}

static const char *const resources[] = {
	"UndefinedResource",
	"FileResource",
	"MemoryResource",
	"MapResource",
	"DiskResource",
	NULL
};

static int
setResourceLimit(lua_State *L)
{
	lua_pushinteger(L, MagickSetResourceLimit(
	    luaL_checkoption(L, 1, "UndefinedResource", resources),
	    luaL_checkinteger(L, 2)));
	return 1;
}

int
luaopen_graphicsmagick(lua_State *L)
{
	struct luaL_Reg luagraphicsmagick[] = {
		{ "newDrawingWand",	newDrawingWand },
		{ "newMagickWand",	newMagickWand },
		{ "newPixelWand",	newPixelWand },
		{ "setResourceLimit",	setResourceLimit },
		{ NULL, NULL }
	};
	luaL_newlib(L, luagraphicsmagick);

	if (luaL_newmetatable(L, DRAWING_WAND_METATABLE)) {
		luaL_setfuncs(L, drawing_wand_methods, 0);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		lua_pushliteral(L, "__metatable");
		lua_pushliteral(L, "must not access this metatable");
		lua_settable(L, -3);
	}
	lua_pop(L, 1);

	if (luaL_newmetatable(L, MAGICK_WAND_METATABLE)) {
		luaL_setfuncs(L, magick_wand_methods, 0);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		lua_pushliteral(L, "__metatable");
		lua_pushliteral(L, "must not access this metatable");
		lua_settable(L, -3);
	}
	lua_pop(L, 1);

	if (luaL_newmetatable(L, PIXEL_WAND_METATABLE)) {
		luaL_setfuncs(L, pixel_wand_methods, 0);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		lua_pushliteral(L, "__metatable");
		lua_pushliteral(L, "must not access this metatable");
		lua_settable(L, -3);
	}
	lua_pop(L, 1);

	lua_pushliteral(L, "_COPYRIGHT");
	lua_pushliteral(L, "Copyright (C) 2016, 2017 by "
	    "micro systems marc balmer");
	lua_settable(L, -3);
	lua_pushliteral(L, "_DESCRIPTION");
	lua_pushliteral(L, "GraphicsMagick for Lua");
	lua_settable(L, -3);
	lua_pushliteral(L, "_VERSION");
	lua_pushliteral(L, "graphicsmagick 1.1.0");
	lua_settable(L, -3);
	return 1;
}
