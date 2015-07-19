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
#include "pictureoverlay.h"

namespace Yuni
{
namespace UI
{


	void PictureOverlay::draw(const Gfx3D::ShaderProgram::Ptr& shader) const
	{
		if (!visible() || !shader || !shader->valid() || !pTexture)
			return;

		// Set shaders
		shader->activate();
		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, pTexture->id());
		shader->bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);

		const float texWidth = (float)pTexture->width();
		const float texHeight = (float)pTexture->height();
		const float overlayWidth = (float)pWidth;
		const float overlayHeight = (float)pHeight;
		// pDisplay == dmNone
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		if (pDisplay == dmOffset)
		{
			offsetX = (float)pOffsetX;
			offsetY = (float)pOffsetY;
		}
		else if (pDisplay == dmCenter)
		{
			offsetX = (texWidth - overlayWidth) / 2.0f;
			offsetY = (texHeight - overlayHeight) / 2.0f;
		}
		float minTexX = 0.0f;
		float maxTexX = 1.0f;
		float minTexY = 0.0f;
		float maxTexY = 1.0f;
		float xStart = (float)pX;
		float yStart = (float)pY;
		float xEnd = xStart + overlayWidth;
		float yEnd = yStart + overlayHeight;
		switch (pDisplay)
		{
			case dmStretch:
				// Nothing to do, coordinates are perfect
				break;
			case dmNone:
			case dmOffset:
			case dmCenter:
				if (offsetX > 0.0f)
				{
					// Fix texture coordinates
					minTexX = offsetX / texWidth;
					if (minTexX < 0.0f)
						minTexX = 0.0f;
				}
				if (texWidth > overlayWidth + offsetX)
				{
					// Fix texture coordinates
					maxTexX = (overlayWidth + offsetX) / texWidth;
					if (maxTexX > 1.0f)
						maxTexX = 1.0f;
				}
				if (offsetY > 0.0f)
				{
					minTexY = offsetY / texHeight;
					if (minTexY < 0.0f)
						minTexY = 0.0f;
				}
				if (texHeight > overlayHeight + offsetY)
				{
					maxTexY = (overlayHeight + offsetY) / texHeight;
					if (maxTexY > 1.0f)
						maxTexY = 1.0f;
				}
				break;
			case dmFit:
				// TODO
				break;
			case dmFill:
				// TODO
				break;
			default:
				assert(false && "Invalid enum value for PictureOverlay::Display !");
				break;
		}

		shader->bindUniform("Bounds", xStart, yStart, texWidth + xStart, texHeight + yStart);
		shader->bindUniform("FillColor", pFillColor);

		// Set texture coordinates
		const float texCoord[] =
			{
				minTexX, minTexY,
				minTexX, maxTexY,
				maxTexX, minTexY,
				maxTexX, minTexY,
				minTexX, maxTexY,
				maxTexX, maxTexY
			};
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaTextureCoord, 2, GL_FLOAT, 0, 0, texCoord);
		// Set vertex positions
		const float vertices[] =
			{
				xStart, yStart,
				xStart, yEnd,
				xEnd, yStart,
				xEnd, yStart,
				xStart, yEnd,
				xEnd, yEnd
			};
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaPosition, 2, GL_FLOAT, 0, 0, vertices);
		// Draw
		::glDrawArrays(GL_TRIANGLES, 0, 6);

		// Clean-up
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glBindTexture(GL_TEXTURE_2D, 0);
		shader->deactivate();
	}



} // namespace UI
} // namespace Yuni
