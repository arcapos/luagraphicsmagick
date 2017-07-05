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

static const char *const channels[] = {
	"UndefinedChannel",
	"RedChannel",
	"CyanChannel",
	"GreenChannel",
	"MagentaChannel",
	"BlueChannel",
	"YellowChannel",
	"OpacityChannel",
	"BlackChannel",
	"MatteChannel",
	"AllChannels",
	"GreyChannel",
	NULL
};

static int
gammaImageChannel(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGammaImageChannel(*mw,
	    luaL_checkoption(L, 2, "UndefinedChannel", channels),
	    luaL_checknumber(L, 3)));
	return 1;
}

static int
getConfigureInfo(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetConfigureInfo(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
getException(lua_State *L)
{
	MagickWand **mw;
	ExceptionType severity;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetException(*mw, &severity));
	lua_pushinteger(L, severity);
	return 2;
}

static int
getFilename(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetFilename(*mw));
	return 1;
}

static int
getImage(lua_State *L)
{
	MagickWand **mw, **mw2;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	mw2 = lua_newuserdata(L, sizeof(MagickWand *));
	*mw2 = MagickGetImage(*mw);
	luaL_setmetatable(L, MAGICK_WAND_METATABLE);
	return 1;
}

static int
getImageAttribute(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetImageAttribute(*mw, luaL_checkstring(L, 2)));
	return 1;
}

static int
getImageBackgroundColor(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageBackgroundColor(*mw, *pw));
	return 1;
}

static int
getImageBluePrimary(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	double x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageBluePrimary(*mw, &x, &y));
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 3;
}

static int
getImageBorderColor(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageBorderColor(*mw, *pw));
	return 1;
}

static int
getImageBoundingBox(lua_State *L)
{
	MagickWand **mw;
	unsigned long width, height;
	long x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageBoundingBox(*mw,
	    luaL_checknumber(L, 2), &width, &height, &x, &y));
	lua_pushinteger(L, width);
	lua_pushinteger(L, height);
	lua_pushinteger(L, x);
	lua_pushinteger(L, x);
	return 5;
}

static int
getImageChannelDepth(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageChannelDepth(*mw,
	    luaL_checkoption(L, 2, "UndefinedChannel", channels)));
	return 1;
}

static int
getImageChannelExtrema(lua_State *L)
{
	MagickWand **mw;
	unsigned long minima, maxima;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageChannelExtrema(*mw,
	    luaL_checkoption(L, 2, "UndefinedChannel", channels), &minima,
	    &maxima));
	lua_pushinteger(L, minima);
	lua_pushinteger(L, maxima);
	return 3;
}

static int
getImageChannelMean(lua_State *L)
{
	MagickWand **mw;
	double mean, standard_deviation;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageChannelMean(*mw,
	    luaL_checkoption(L, 2, "UndefinedChannel", channels), &mean,
	    &standard_deviation));
	lua_pushnumber(L, mean);
	lua_pushnumber(L, standard_deviation);
	return 3;
}

static int
getImageColormapColor(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 3, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageColormapColor(*mw,
	    luaL_checkinteger(L, 2), *pw));
	return 1;
}

static int
getImageColors(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageColors(*mw));
	return 1;
}

static const char *const color_spaces[] = {
	"UndefinedColorspace",
	"RGBColorspace",
	"GRAYColorspace",
	"TransparentColorspace",
	"OHTAColorspace",
	"XYZColorspace",
	"YCCColorspace",
	"YIQColorspace",
	"YPbPrColorspace",
	"YUVColorspace",
	"CMYKColorspace",
	"sRGBColorspace",
	"HSLColorspace",
	"HWBColorspace",
	"LABColorspace",
	"CineonLogRGBColorspace",
	"Rec601LumaColorspace",
	"Rec601YCbCrColorspace",
	"Rec709LumaColorspace",
	"Rec709YCbcrColorspace",
	NULL
};

static int
getImageColorspace(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, color_spaces[MagickGetImageColorspace(*mw)]);
	return 1;
}

const char *const compressions[] = {
	"UndefinedCompression",
	"NoCompression",
	"BZipCompression",
	"FaxCompression",
	"Group4Compression",
	"JPEGCompression",
	"LosslessJPEGCompression",
	"LZWCompression",
	"RLECompression",
	"ZipCompression",
	NULL
};

static int
getImageCompression(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, compressions[MagickGetImageCompression(*mw)]);
	return 1;
}

static int
getImageDelay(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageDelay(*mw));
	return 1;
}

static int
getImageDepth(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageDepth(*mw));
	return 1;
}

static int
getImageExtrema(lua_State *L)
{
	MagickWand **mw;
	unsigned long minima, maxima;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageExtrema(*mw, &minima, &maxima));
	lua_pushinteger(L, minima);
	lua_pushinteger(L, maxima);
	return 3;
}

static int
getImageFilename(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetImageFilename(*mw));
	return 1;
}

static int
getImageFormat(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, MagickGetImageFormat(*mw));
	return 1;
}

static int
getImageFuzz(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushnumber(L, MagickGetImageFuzz(*mw));
	return 1;
}

static int
getImageGamma(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushnumber(L, MagickGetImageGamma(*mw));
	return 1;
}

/* XXX factor out enum to string definitions? */
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
getImageGravity(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, gravity[MagickGetImageGravity(*mw)]);
	return 1;
}

static int
getImageGreenPrimary(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	double x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageGreenPrimary(*mw, &x, &y));
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 3;
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
getImageIndex(lua_State *L)
{
	MagickWand **mw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageIndex(*mw));
	return 1;
}

static const char *const interlaces[] = {
	"UndefinedInterlace",
	"NoInterlace",
	"LineInterlace",
	"PlaneInterlace",
	"PartitionInterlace",
	NULL
};

static int
getImageInterlaceScheme(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, interlaces[MagickGetImageInterlaceScheme(*mw)]);
	return 1;
}

static int
getImageIterations(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageIterations(*mw));
	return 1;
}

static int
getImageMatteColor(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageMatteColor(*mw, *pw));
	return 1;
}

#if 0
static const char *const orientations[] = {
	"UndefinedOrientation",
	"TopLeftOrientation",
	"TopRightOrientation",
	"BottomRightOrientation",
	"BottomLeftOrientation",
	"LeftTopOrientation",
	"RightTopOrientation",
	"RightBottomOrientation",
	"LeftBottomOrientation",
	NULL
};

static int
getImageOrientation(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, orientations[MagickGetImageOrientation(*mw)]);
	return 1;
}
#endif

static int
getImagePage(lua_State *L)
{
	MagickWand **mw;
	unsigned long width, height;
	long x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImagePage(*mw, &width, &height, &x, &y));
	lua_pushinteger(L, width);
	lua_pushinteger(L, height);
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	return 5;
}

static int
getImageRedPrimary(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;
	double x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageRedPrimary(*mw, &x, &y));
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 3;
}

static const char* const intents[] = {
	"UndefinedIntent",
	"SalutationIntent",
	"PerceptualIntent",
	"AbsoluteIntent",
	"RelativeIntent",
	NULL
};

static int
getImageRenderingIntent(lua_State *L)
{
	MagickWand **mw;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushstring(L, intents[MagickGetImageRenderingIntent(*mw)]);
	return 1;
}

static int
getImageResolution(lua_State *L)
{
	MagickWand **mw;
	double x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageResolution(*mw, &x, &y));
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 3;
}

static int
getImageScene(lua_State *L)
{
	MagickWand **mw;
	double x, y;
	
	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	lua_pushinteger(L, MagickGetImageScene(*mw));
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
setImageBackgroundColor(lua_State *L)
{
	MagickWand **mw;
	PixelWand **pw;

	mw = luaL_checkudata(L, 1, MAGICK_WAND_METATABLE);
	pw = luaL_checkudata(L, 2, PIXEL_WAND_METATABLE);
	
	lua_pushinteger(L, MagickSetImageBackgroundColor(*mw, *pw));
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
	{ "gammaImageChannel",		gammaImageChannel },
	{ "getConfigureInfo",		getConfigureInfo },
	{ "getException",		getException },
	{ "getFilename",		getFilename },
	{ "getImage",			getImage },
	{ "getImageAttribute",		getImageAttribute },
	{ "getImageBackgroundColor",	getImageBackgroundColor },
	{ "getImageBluePrimary",	getImageBluePrimary },
	{ "getImageBorderColor",	getImageBorderColor },
	{ "getImageBoundingBox",	getImageBoundingBox },
	{ "getImageChannelDepth",	getImageChannelDepth },
	{ "getImageChannelExtrema",	getImageChannelExtrema },
	{ "getImageChannelMean",	getImageChannelMean },
	{ "getImageColormapColor",	getImageColormapColor },
	{ "getImageColors",		getImageColors },
	{ "getImageColorspace",		getImageColorspace },
	{ "getImageCompression",	getImageCompression },
	{ "getImageDelay",		getImageDelay },
	{ "getImageDepth",		getImageDepth },
	{ "getImageExtrema",		getImageExtrema },
	{ "getImageFilename",		getImageFilename },
	{ "getImageFormat",		getImageFormat },
	{ "getImageFuzz",		getImageFuzz },
	{ "getImageGamma",		getImageGamma },
	{ "getImageGravity",		getImageGravity },
	{ "getImageGreenPrimary",	getImageGreenPrimary },
	{ "setImageFormat",		setImageFormat },
	{ "getImageWidth",		getImageWidth },
	{ "getImageHeight",		getImageHeight },
	{ "getImageIndex",		getImageIndex },
	{ "getImageInterlaceScheme",	getImageInterlaceScheme },
	{ "getImageIterations",		getImageIterations },
	{ "getImageMatteColor",		getImageMatteColor },
#if 0
	{ "getImageOrientation",	getImageOrientation },
#endif
	{ "getImagePage",		getImagePage },
	{ "getImageRedPrimary",		getImageRedPrimary },
	{ "getImageRenderingIntent",	getImageRenderingIntent },
	{ "getImageResolution",		getImageResolution },
	{ "getImageScene",		getImageScene },
	{ "readImage",			readImage },
	{ "readImageBlob",		readImageBlob },
	{ "resizeImage",		resizeImage },
	{ "rotateImage",		rotateImage },
	{ "sampleImage",		sampleImage },
	{ "scaleImage",			scaleImage },
	{ "setImageBackgroundColor",	setImageBackgroundColor },
	{ "setSize",			setSize },
	{ "trimImage",			trimImage },
	{ "writeImage",			writeImage },
	{ "writeImageBlob",		writeImageBlob },
	{ "destroy",			destroy },
	{ "__gc",			destroy },
	{ NULL, NULL }
};

