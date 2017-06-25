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

/* GraphicsMagick MagickWand for Lua */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <wand/magick_wand.h>

#include "luagraphicsmagick.h"

static int
clone(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = CloneMagickWand(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
adaptiveThresholdImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickAdaptiveThresholdImage(*mw,
	    luaL_checkinteger(L, 2), luaL_checkinteger(L, 3),
	    luaL_checkinteger(L, 2)));
	return 1;
}

static int
addImage(lua_State *L)
{
	MagickWand **mw, **add_wand;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	add_wand = luaL_checkudata(L, 2, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickAddImage(*mw, *add_wand));
	return 1;
}

static const char *const noises[] = {
	"UniformNoise",
	"GaussianNoise",
	"MultiplicativeGaussianNoise",
	"ImpulseNoise",
	"LaplacianNoise",
	"PoissonNoise",
	NULL
};

static int
addNoiseImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickAddNoiseImage(*mw,
	    luaL_checkoption(L, 2, "UniformNoise", noises)));
	return 1;
}

static int
affineTransformImage(lua_State *L)
{
	MagickWand **mw;
	DrawingWand **dw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	dw = luaL_checkudata(L, 2, DRAWING_WAND_METATABLE);
	lua_pushinteger(L, MagickAffineTransformImage(*mw, *dw));
	return 1;
}

static int
annotateImage(lua_State *L)
{
	DrawingWand **dw;
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	dw = luaL_checkudata(L, 2, DRAWING_WAND_METATABLE);

	lua_pushinteger(L, MagickAnnotateImage(*mw, *dw, luaL_checknumber(L, 3),
	    luaL_checknumber(L, 4), luaL_checknumber(L, 5),
	    luaL_checkstring(L, 6)));
	return 1;
}

static int
animateImages(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickAnimateImages(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
appendImages(lua_State *L)
{
	MagickWand **mw, **mw2;
	unsigned int stack;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	stack = luaL_checkinteger(L, 2);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickAppendImages(*mw, stack);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);

	return 1;
}

static int
averageImages(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickAverageImages(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
blackThresholdImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickThresholdImage(*mw, luaL_checknumber(L, 2)));
	return 1;
}

static int
blurImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickBlurImage(*mw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3)));
	return 1;
}

static int
borderImage(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);

	lua_pushinteger(L, MagickBorderImage(*mw, *pw, luaL_checkinteger(L, 3),
	    luaL_checkinteger(L, 4)));
	return 1;
}

static int
cdlImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushboolean(L, MagickCdlImage(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
charcoalImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickCharcoalImage(*mw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3)));
	return 1;
}

static int
chopImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickChopImage(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3), luaL_checkinteger(L, 4),
	    luaL_checkinteger(L, 5)));
	return 1;
}

static int
clipImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickClipImage(*mw));
	return 1;
}

static int
clipPathImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickClipPathImage(*mw, luaL_checkstring(L, 2),
	    luaL_checkinteger(L, 3)));
	return 1;
}

static int
coalesceImages(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickCoalesceImages(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
colorFloodfillImage(lua_State *L)
{
	MagickWand **mw;
	PixelWand **fill, **border;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	fill = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	border = luaL_checkudata(L, 4, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickColorFloodfillImage(*mw, *fill,
	    luaL_checknumber(L, 3), *border,  luaL_checkinteger(L, 5),
	    luaL_checkinteger(L, 6)));
	return 1;
}

static int
colorizeImage(lua_State *L)
{
	MagickWand **wand;
	PixelWand **colorize, **opacity;
	
	wand = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	colorize = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	opacity = luaL_checkudata(L, 3, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickColorizeImage(*wand, *colorize, *opacity));
	return 1;
}

static int
commentImage(lua_State *L)
{
	MagickWand **wand;
	
	wand = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickCommentImage(*wand, luaL_checkstring(L, 2)));
	return 1;
}

static int
contrastImage(lua_State *L)
{
	MagickWand **wand;
	
	wand = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickContrastImage(*wand, luaL_checkinteger(L, 2)));
	return 1;
}

static int
cropImage(lua_State *L)
{
	MagickWand **wand;
	
	wand = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickCropImage(*wand, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3), luaL_checkinteger(L, 4),
	    luaL_checkinteger(L, 5)));
	return 1;
}

static int
cycleColormapImage(lua_State *L)
{
	MagickWand **wand;
	
	wand = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickCycleColormapImage(*wand,
	    luaL_checkinteger(L, 2)));
	return 1;
}

static int
deconstructImages(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickDeconstructImages(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
describeImage(lua_State *L)
{
	MagickWand **mw;
	char *text;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	text = MagickDescribeImage(*mw);
	lua_pushstring(L, text);
	free(text);
	return 1;
}

static int
despeckleImage(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickDespeckleImage(*mw));
	return 1;
}

static int
displayImage(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickDisplayImage(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
displayImages(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickDisplayImages(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
drawImage(lua_State *L)
{
	DrawingWand **dw;
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	dw = luaL_checkudata(L, 2, DRAWING_WAND_METATABLE);

	lua_pushinteger(L, MagickDrawImage(*mw, *dw));
	return 1;
}

static int
edgeImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickEdgeImage(*mw, luaL_checknumber(L, 2)));
	return 1;
}

static int
embossImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickEmbossImage(*mw, luaL_checknumber(L, 2),
	    luaL_checknumber(L, 3)));
	return 1;
}

static int
enhanceImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickEnhanceImage(*mw));
	return 1;
}

static int
equalizeImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickEqualizeImage(*mw));
	return 1;
}

static int
extentImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickExtentImage(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3), luaL_checkinteger(L, 4),
	    luaL_checkinteger(L, 5)));
	return 1;
}

static int
flattenImages(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickFlattenImages(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
flipImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickFlipImage(*mw));
	return 1;
}

static int
flopImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickFlopImage(*mw));
	return 1;
}

static int
frameImage(lua_State *L)
{
	MagickWand **mw;
	PixelWand **matte_color;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	matte_color = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	
	lua_pushinteger(L, MagickFrameImage(*mw, *matte_color,
	    luaL_checkinteger(L, 3), luaL_checkinteger(L, 4),
	    luaL_checkinteger(L, 5), luaL_checkinteger(L, 6)));
	return 1;
}

static int
fxImage(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickFxImage(*mw, luaL_checkstring(L, 2));
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
gammaImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGammaImage(*mw, luaL_checknumber(L, 2)));
	return 1;
}

static int
setImageFormat(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickSetImageFormat(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
getImageWidth(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageWidth(*mw));
	return 1;
}

static int
getImageHeight(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageHeight(*mw));
	return 1;
}

static int
readImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickReadImage(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
readImageBlob(lua_State *L)
{
	MagickWand **mw;
	size_t len;
	const char *blob;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	blob = luaL_checklstring(L, 2, &len);

	lua_pushinteger(L, MagickReadImageBlob(*mw, blob, len));
	return 1;
}

static const char *const filters[] = {
	"UndefinedFilter",
	"PointFilter",
	"BoxFilter",
	"TriangleFilter",
	"HermiteFilter",
	"HanningFilter",
	"HammingFilter",
	"BlackmanFilter",
	"GaussianFilter",
	"QuadraticFilter",
	"CubicFilter",
	"CatromFilter",
	"MitchellFilter",
	"LanczosFilter",
	"BesselFilter",
	"SincFilter",
	NULL
};

static int
resizeImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);

	lua_pushinteger(L, MagickResizeImage(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3),
	    luaL_checkoption(L, 4, "UndefinedFilter", filters),
	    luaL_checknumber(L, 5)));
	return 1;
}

static int
rotateImage(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickRotateImage(*mw, *pw, luaL_checknumber(L, 3)));
	return 1;
}

static int
sampleImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickSampleImage(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3)));
	return 1;
}

static int
scaleImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickScaleImage(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3)));
	return 1;
}

static int
setSize(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickSetSize(*mw, luaL_checkinteger(L, 2),
	    luaL_checkinteger(L, 3)));
	return 1;
}

static int
trimImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickTrimImage(*mw, luaL_checknumber(L, 2)));
	return 1;
}

static int
writeImage(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickWriteImage(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
writeImageBlob(lua_State *L)
{
	MagickWand **mw;
	size_t len;
	const char *blob;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	blob = MagickWriteImageBlob(*mw, &len);
	lua_pushlstring(L, blob, len);
	return 1;
}

static int
destroy(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	if (*mw) {
		DestroyMagickWand(*mw);
		*mw = NULL;
	}
	return 0;
}

struct luaL_Reg magick_wand_methods[] = {
	{ "clone",			clone },
	{ "adaptiveThresholdImage",	adaptiveThresholdImage },
	{ "addImage",			addImage },
	{ "addNoiseImage",		addNoiseImage },
	{ "affineTransformImage",	affineTransformImage },
	{ "annotateImage",		annotateImage },
	{ "animateImages",		animateImages },
	{ "appendImages",		appendImages },
	{ "averageImages",		averageImages },
	{ "blackThresholdImage",	blackThresholdImage },
	{ "blurImage",			blurImage },
	{ "borderImage",		borderImage },
	{ "cdlImage",			cdlImage },
	{ "charcoalImage",		charcoalImage },
	{ "chopImage",			chopImage },
	{ "clipImage",			clipImage },
	{ "clipPathImage",		clipPathImage },
	{ "coalesceImages",		coalesceImages },
	{ "colorFloodfillImage",	colorFloodfillImage },
	{ "colorizeImage",		colorizeImage },
	{ "commentImage",		commentImage },
	{ "contrastImage",		contrastImage },
	{ "cropImage",			cropImage },
	{ "cycleColormapImage",		cycleColormapImage },
	{ "deconstructImages",		deconstructImages },
	{ "describeImage",		describeImage },
	{ "despeckleImage",		despeckleImage },
	{ "displayImage",		displayImage },
	{ "displayImages",		displayImages },
	{ "drawImage",			drawImage },
	{ "edgeImage",			edgeImage },
	{ "embossImage",		embossImage },
	{ "enhanceImage",		enhanceImage },
	{ "equalizeImage",		equalizeImage },
	{ "extentImage",		extentImage },
	{ "flattenImages",		flattenImages },
	{ "flipImage",			flipImage },
	{ "flopImage",			flopImage },
	{ "frameImage",			frameImage },
	{ "fxImage",			fxImage },
	{ "gammaImage",			gammaImage },
	{ "setImageFormat",		setImageFormat },
	{ "getImageWidth",		getImageWidth },
	{ "getImageHeight",		getImageHeight },
	{ "readImage",			readImage },
	{ "readImageBlob",		readImageBlob },
	{ "resizeImage",		resizeImage },
	{ "rotateImage",		rotateImage },
	{ "sampleImage",		sampleImage },
	{ "scaleImage",			scaleImage },
	{ "setSize",			setSize },
	{ "trimImage",			trimImage },
	{ "writeImage",			writeImage },
	{ "writeImageBlob",		writeImageBlob },
	{ "destroy",			destroy },
	{ "__gc",			destroy },
	{ NULL, NULL }
};
