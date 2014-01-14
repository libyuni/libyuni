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

		int offsetX = 0;
		int offsetY = 0;
		if (pDisplay == podOffset)
		{
			offsetX = pOffsetX;
			offsetY = pOffsetY;
		}
		float minX = 0.0f;
		float maxX = 1.0f;
		float minY = 0.0f;
		float maxY = 1.0f;
		switch (pDisplay)
		{
			case podStretch:
				// Nothing to do, coordinates are perfect
				break;
			case podNone:
				if (pTexture->width() > pWidth)
					maxX = (float)pWidth / (float)pTexture->width();
				else if (pTexture->width() < pWidth)
				{
					// TODO : Fix vertices
				}
				if (pTexture->height() > pHeight)
					maxY = (float)pHeight / (float)pTexture->height();
				else if (pTexture->height() < pHeight)
				{
					// TODO : Fix vertices
				}
				break;
			case podOffset:
			case podCenter:
			case podFit:
			case podFill:
				// TODO
				break;
			default:
				assert(false && "Invalid enum value for PictureOverlay::Display !");
				break;
		}

		// Set texture coordinates
		const float texCoord[] =
			{
				minX, minY,
				minX, maxY,
				maxX, minY,
				maxX, minY,
				minX, maxY,
				maxX, maxY
			};
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaTextureCoord, 2, GL_FLOAT, 0, 0, texCoord);
		// Set vertex positions
		const float vertices[] =
			{
				(float)pX, (float)pY,
				(float)pX, (float)(pY + height()),
				(float)(pX + width()), (float)pY,
				(float)(pX + width()), (float)pY,
				(float)pX, (float)(pY + height()),
				(float)(pX + width()), (float)(pY + height())
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
