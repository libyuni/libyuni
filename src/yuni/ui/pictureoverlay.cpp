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

		// Set texture coordinates
		const float texCoord[] =
			{
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f
			};
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaTextureCoord, 2, GL_FLOAT, 0, 0, texCoord);
		// Set vertex positions
		const float vertices[] =
			{
				(float)pX, (float)(pY + height()),
				(float)pX, (float)pY,
				(float)(pX + width()), (float)pY,
				(float)pX, (float)(pY + height()),
				(float)(pX + width()), (float)pY,
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
