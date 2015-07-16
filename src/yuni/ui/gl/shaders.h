/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#ifndef __YUNI_UI_GL_SHADERS_H__
# define __YUNI_UI_GL_SHADERS_H__

// This file contains necessary GLSL shaders for internal use

namespace Yuni
{
namespace Gfx3D
{



	/////////////// VERTEX SHADERS


	// Minimal vertex shader : only transform the vertex coordinates
	extern const char* const vsTransform;

	// Very simple vertex shader : transform coordinates and propagate texture coordinates
	extern const char* const vsTexCoord;

	// For 2D post shaders, texture coordinates are calculated by transforming vertex position
	// from [-1,1] to [0,1]
	extern const char* const vs2D;

	// Pass the color as attribute
	extern const char* const vsColorAttr;

	// Sample a texture using a rectangle, do not resize the image, fill empty parts with a color
	extern const char* const vsImageRect;

	// Phong shading
	extern const char* const vsPhong;

	extern const char* const vsCubeMap;



	/////////////// FRAGMENT SHADERS


	// Use a single color given as uniform
	extern const char* const fsColorUniform;

	// Use a single color given as attribute
	extern const char* const fsColorAttr;

	// Use directly the texture value, no lighting
	extern const char* const fsSimpleTexture;

	// Sample a texture using a rectangle, do not resize the image, fill empty parts with a color
	extern const char* const fsImageRect;

	// Freetype with normal render mode generates alpha-only bitmaps, stored as GL_R textures
	// This shader displays them with the proper color.
	extern const char* const fsText;

	// Color picking
	extern const char* const fsPicking;

	// Skybox : cube map sampling
	extern const char* const fsSkybox;

	// Phong shading
	extern const char* const fsPhong;


	//// POST FRAGMENT SHADERS

	extern const char* const fsYuv2Rgb;



	/////////////// GEOMETRY SHADERS


	// Generate empty borders for image rectangles
	extern const char* const gsImageRect;



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_UI_GL_SHADERS_H__
