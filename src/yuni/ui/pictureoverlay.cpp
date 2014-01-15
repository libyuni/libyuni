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
		// pDisplay == podNone
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		if (pDisplay == podOffset)
		{
			offsetX = (float)pOffsetX;
			offsetY = (float)pOffsetY;
		}
		else if (pDisplay == podCenter)
		{
			offsetX = (texWidth - overlayWidth) / 2.0f;
			offsetY = (texHeight - overlayHeight) / 2.0f;
		}
		// Check if the whole overlay is out of bounds of the texture
		if (offsetX > texWidth || offsetY > texHeight ||
			offsetX + overlayWidth < 0.0f || offsetY + overlayHeight < 0.0f)
			return;
		float minTexX = 0.0f;
		float maxTexX = 1.0f;
		float minTexY = 0.0f;
		float maxTexY = 1.0f;
		float xStart = pX;
		float yStart = pY;
		float xEnd = pX + overlayWidth;
		float yEnd = pY + overlayHeight;
		switch (pDisplay)
		{
			case podStretch:
				// Nothing to do, coordinates are perfect
				break;
			case podNone:
			case podOffset:
			case podCenter:
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
					maxTexX = pWidth + offsetX / texWidth;
					if (maxTexX > 1.0f)
						maxTexX = 1.0f;
				}
				else
				{
					// Fix vertices
				}
				if (offsetY > 0.0f)
				{
					minTexY = offsetY / texHeight;
					if (minTexY < 0.0f)
						minTexY = 0.0f;
				}
				if (texHeight > overlayHeight + offsetY)
				{
					maxTexY = pHeight + offsetY / texHeight;
					if (maxTexY > 1.0f)
						maxTexY = 1.0f;
				}
				else
				{
					// Fix vertices
				}
				break;
			case podFit:
				// TODO
				break;
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
