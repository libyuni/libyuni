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
				pFont->draw(pText, pTexture, pAntiAliased, true, pTabWidth);
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

		// Set vertex positions
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		const float vertices[] =
			{
				pX, pY,
				pX, pY + height(),
				pX + width(), pY,
				pX + width(), pY,
				pX, pY + height(),
				pX + width(), pY + height()
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

		// Set vertex positions
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		const float vertices[] =
			{
				pX, pY,
				pX, pY + height(),
				pX + width(), pY,
				pX + width(), pY,
				pX, pY + height(),
				pX + width(), pY + height()
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



} // namespace UI
} // namespace Yuni
