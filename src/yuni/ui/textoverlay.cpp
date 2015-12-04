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
# include "textoverlay.h"


namespace Yuni
{
namespace UI
{


	void TextOverlay::update()
	{
		if (pModified || pPrevText != pText)
		{
			if (pModified)
				pFont->reset();
			pModified = false;
			pPrevText = pText;
			if (!pTexture)
				pTexture = Gfx3D::Texture::New(1, 1, 1, Gfx3D::Texture::UInt8, nullptr, false);
			if (pText.empty())
				pTexture->clear();
			else
			{
				uint width = 0u;
				uint height = 0u;
				int descent = 0;
				// We are only interested in the ascent here
				pFont->measure(pText, width, height, pAscent, descent, true, pTabWidth);
				// Draw
				pFont->draw(pText, pTexture, pAntiAliased, true, pTabWidth);
			}
		}
	}


	void TextOverlay::draw(const Gfx3D::ShaderProgram::Ptr& shader) const
	{
		if (!shader || !shader->valid() || !pTexture)
			return;
		assert(pMaterial && "Uninitialized material !");

		// Set shaders
		shader->activate();
		// Put the alpha (the actual text) on texture 0
		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, pTexture->id());
		shader->bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);
		// Put the optional material surface on texture 1
		pMaterial->activate(shader, 1);
		//::glActiveTexture(GL_TEXTURE1);
		//::glBindTexture(GL_TEXTURE_2D, 8);
		// Temporary !!!
		shader->bindUniform("BackColor", Color::RGBA<float>(0.2863f, 0.5451f, 0.651f));
		shader->bindUniform("HasBGColor", 0.0f);

		// Set texture coordinates
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		const float texCoord[] =
			{
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaTextureCoord, 2, GL_FLOAT, 0, 0, texCoord);

		float yPos = pY;
		// If necessary, offset by the Ascent to reach the text baseline
		if (pDrawOnBaseline)
		{
			yPos -= (float)pAscent;
		}
		// Ensure dimensions are pixel-aligned
		float xPos = (float)(int)pX;
		yPos = (float)(int)yPos;
		// Set vertex positions
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		const float vertices[] =
			{
				xPos, yPos,
				xPos, yPos + height(),
				xPos + width(), yPos,
				xPos + width(), yPos,
				xPos, yPos + height(),
				xPos + width(), yPos + height()
			};
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaPosition, 2, GL_FLOAT, 0, 0, vertices);

		// Draw
		::glDrawArrays(GL_TRIANGLES, 0, 6);

		// Clean-up
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glActiveTexture(GL_TEXTURE1);
		::glBindTexture(GL_TEXTURE_2D, 0);
		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, 0);
		shader->deactivate();
	}


	void TextOverlay::draw(const Gfx3D::ShaderProgram::Ptr& shader, const Color::RGB<float>& backColor) const
	{
		draw(shader, Color::RGBA<float>(backColor.red, backColor.green, backColor.red, 1.0f));
	}


	void TextOverlay::draw(const Gfx3D::ShaderProgram::Ptr& shader, const Color::RGBA<float>& backColor) const
	{
		if (!shader || !shader->valid() || !pTexture)
			return;
		assert(pMaterial && "Uninitialized material !");

		// Set shaders
		shader->activate();
		// Put the alpha (the actual text) on texture 0
		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, pTexture->id());
		shader->bindUniform("Texture0", Yuni::Gfx3D::Vertex<>::vaTexture0);
		// Put the optional material surface on texture 1
		pMaterial->activate(shader, 1);
		shader->bindUniform("BackColor", backColor);
		shader->bindUniform("HasBGColor", 1.0f);

		// Set texture coordinates
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		const float texCoord[] =
			{
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaTextureCoord, 2, GL_FLOAT, 0, 0, texCoord);

		// Ensure dimensions are pixel-aligned
		float xPos = (float)(int)pX;
		float yPos = (float)(int)pY;
		// Set vertex positions
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		const float vertices[] =
			{
				xPos, yPos,
				xPos, yPos + height(),
				xPos + width(), yPos,
				xPos + width(), yPos,
				xPos, yPos + height(),
				xPos + width(), yPos + height()
			};
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaPosition, 2, GL_FLOAT, 0, 0, vertices);

		// Draw
		::glDrawArrays(GL_TRIANGLES, 0, 6);

		// Clean-up
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glActiveTexture(GL_TEXTURE1);
		::glBindTexture(GL_TEXTURE_2D, 0);
		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, 0);
		shader->deactivate();
	}


	PictureOverlay::Ptr TextOverlay::toPicture() const
	{
		return new PictureOverlay(Gfx3D::Texture::Copy(pTexture), (int)pX, (int)pY);
	}



} // namespace UI
} // namespace Yuni
