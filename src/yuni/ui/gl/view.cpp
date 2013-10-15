#include <yuni/core/math.h>
#include "view.h"
#include "shadermanager.h"
#include "texture.h"

namespace Yuni
{
namespace UI
{


	View::View(uint x, uint y, uint w, uint h, uint8 z, bool visible) :
		pID(UUID::fGenerate),
		pX(x),
		pY(y),
		pWidth(w),
		pHeight(h > 0 ? h : 1), // Forbid null height to avoid divide by 0
		pZ(z),
		pVisible(visible),
		pTextShaders(nullptr),
		pPictureShaders(nullptr)
	{}


	View::~View()
	{}


	bool View::initShaders()
	{
		// Shaders are created here temporarily, because creation in the constructor requires
		// a GL context that the first created view does not have yet
		if (!pTextShaders and !pPictureShaders)
		{
			auto& shaderManager = Gfx3D::ShaderManager::Instance();
			pTextShaders = shaderManager.get("data/shaders/minimal.vert", "data/shaders/text.frag");
			// Bind attributes before linking
			pTextShaders->bindAttribute("attrVertex", Gfx3D::Vertex<>::vaPosition);
			pTextShaders->bindAttribute("attrTexCoord", Gfx3D::Vertex<>::vaTextureCoord);
			if (!pTextShaders->load())
			{
				std::cerr << pTextShaders->errorMessage();
				return false;
			}
			pTextShaders->activate();
			pTextShaders->deactivate();

			pPictureShaders = shaderManager.get("data/shaders/minimal.vert", "data/shaders/simpletexture.frag");
			// Bind attributes before linking
			pPictureShaders->bindAttribute("attrVertex", Gfx3D::Vertex<>::vaPosition);
			pPictureShaders->bindAttribute("attrTexCoord", Gfx3D::Vertex<>::vaTextureCoord);
			if (!pPictureShaders->load())
			{
				std::cerr << pPictureShaders->errorMessage();
				return false;
			}
			pPictureShaders->activate();
			pPictureShaders->bindUniform("Texture0", Gfx3D::Vertex<>::vaTexture0);
			pPictureShaders->deactivate();
		}
		return true;
	}


	void View::draw() const
	{
		if (!pVisible)
			return;


		// Clear depth buffer
		::glClear(GL_DEPTH_BUFFER_BIT);

		/*
		if (!(!pCamera))
		{
			// Reset The Current Viewport
			::glViewport(pX, pY, pWidth, pHeight);

			// Select the Projection Matrix
			::glMatrixMode(GL_PROJECTION);
			// Reset the Projection Matrix
			::glLoadIdentity();

			// Calculate the Aspect Ratio of the window
			::gluPerspective(pCamera->fov(), (GLfloat)pWidth / (GLfloat)pHeight, 0.01f, 10000000.0f);

			// Reset the current model-view matrix
			::glMatrixMode(GL_MODELVIEW);
			::glLoadIdentity();

			// Apply the camera rotation
			const Yuni::Vector3D<>& camRot = pCamera->rotation();
			::glRotatef(360.0f - camRot.x, 1.0f, 0.0f, 0.0f);
			::glRotatef(360.0f - camRot.y, 0.0f, 1.0f, 0.0f);
			::glRotatef(360.0f - camRot.z, 0.0f, 0.0f, 1.0f);


			Draw the skybox
			if (!(!pScene))
			{
				const Yuni::Point3D<>& camPos = pCamera->position();
				const Yuni::Vector3D<>& camView = pCamera->direction();
				const Yuni::Vector3D<>& camUp = pCamera->up();
				// Look at
				::gluLookAt(camPos.x, camPos.y, camPos.z,
					camView.x + camPos.x, camView.y + camPos.y, camView.z + camPos.z,
					camUp.z, camUp.y, camUp.z);

				::glTranslatef(camPos.x, camPos.y, camPos.z);

				pScene->drawSky();

				// Apply the camera translation
				::glTranslatef(-camPos.x, -camPos.y, -camPos.z);

				pScene->draw();
			}
		}
		*/

		draw2D();
	}


	void View::draw2D() const
	{

		// Save current states
		::glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
		// Disable depth test to avoid having the overlay hidden
		::glDisable(GL_DEPTH_TEST);

		// Set view matrices for 2D overlay display
		::glMatrixMode(GL_PROJECTION);
		::glPushMatrix();
		::glLoadIdentity();
		// Window coordinates : [0,0] at top-left, [width,height] at bottom-right
		::gluOrtho2D(0.0f, pWidth, pHeight, 0.0f);

		::glMatrixMode(GL_MODELVIEW);
		::glPushMatrix();
		::glLoadIdentity();

		// Alpha blending
		::glEnable(GL_BLEND);
		::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw the overlays
		PictureOverlay::Vector::const_iterator endP = pPictures.end();
		for (PictureOverlay::Vector::const_iterator it = pPictures.begin(); endP != it; ++it)
			drawOverlay(**it);

		TextOverlay::Vector::const_iterator endT = pTexts.end();
		for (TextOverlay::Vector::const_iterator it = pTexts.begin(); endT != it; ++it)
			drawOverlay(**it);

		// Create a drawing surface for control rendering that has the size of the root control
		/*
		if (!(!pControl) and pControl->visible())
		{
			if (!pUISurface)
			{
				pUISurface = new DrawingSurface(pControl->width(), pControl->height());
				pUISurface->begin();
				pControl->draw(pUISurface, true);
				pUISurface->commit();
			}
			else if (pControl->modified())
			{
				pUISurface->begin();
				pControl->draw(pUISurface, true);
				pUISurface->commit();
			}

			if (pPictureShaders and pPictureShaders->valid())
			{
				pPictureShaders->activate();
				drawPicture(pUISurface->texture(), pControl->x(), pControl->y(),
					pControl->width(), pControl->height());
				pPictureShaders->deactivate();
			}
		}
		*/

		// Reset matrices
		::glMatrixMode(GL_PROJECTION);
		::glPopMatrix();
		::glMatrixMode(GL_MODELVIEW);
		::glPopMatrix();

		// Restore enable settings
		::glPopAttrib();
	}


	void View::drawOverlay(TextOverlay& text) const
	{
		// Update the data if necessary
		text.update();

		// Draw the texture
		text.draw(pTextShaders);
	}


	void View::drawOverlay(const PictureOverlay& picture) const
	{
		picture.draw(pPictureShaders);
	}


	void View::drawPicture(const Gfx3D::Texture::Ptr& texture, int x, int y, unsigned int width, unsigned int height) const
	{
		if (!texture)
			return;

		// Bind the texture
		::glBindTexture(GL_TEXTURE_2D, texture->id());

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
		// Set vertices
		const float vertices[] =
			{
				(float)x, (float)(y + height),
				(float)x, (float)y,
				(float)(x + width), (float)y,
				(float)x, (float)(y + height),
				(float)(x + width), (float)y,
				(float)(x + width), (float)(y + height)
			};
		::glEnableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glVertexAttribPointer(Gfx3D::Vertex<>::vaPosition, 2, GL_FLOAT, 0, 0, vertices);
		// Draw
		::glDrawArrays(GL_TRIANGLES, 0, 6);
		// Clean up
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaPosition);
		::glDisableVertexAttribArray(Gfx3D::Vertex<>::vaTextureCoord);

		// Unbind the texture
		::glBindTexture(GL_TEXTURE_2D, 0);
	}


	IControl* View::getControlAt(int x, int y)
	{
		if (!pControl)
			return nullptr;
		return pControl->getControlAt(x, y);
	}


	EventPropagation View::doMouseMove(int x, int y)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doMouseMove(x, y);
		if (epContinue != propagate)
			return propagate;
		// If the event was not managed by UI in this view, try 3D picking
		EventPropagation prop2 = epContinue; // TODO : Event handling on 3D object picking
		return Math::Max(propagate, prop2);
	}


	EventPropagation View::doMouseDown(Input::IMouse::Button btn, int x, int y)
	{
		// If there are UI controls in this view
		if (!(!pControl))
		{
			// Dispatch the event
			EventPropagation propagate = pControl->doMouseDown(btn, x, y);
			if (epContinue != propagate)
				return propagate;
		}
		// If the event was not managed by UI in this view, try 3D picking
		// if (!(!pScene))
		// {
		// 	const IDrawable::Ptr& object = pScene->pick(x, y);
		// 	// TODO
		// 	return !object ? epContinue : epStop;
		// }
		return epContinue;
	}


	EventPropagation View::doMouseUp(Input::IMouse::Button btn, int x, int y)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doMouseUp(btn, x, y);
		if (epContinue != propagate)
			return propagate;
		// If the event was not managed by UI in this view, try 3D picking
		EventPropagation prop2 = epContinue; // TODO : Event handling on 3D object picking
		return Math::Max(propagate, prop2);
	}


	EventPropagation View::doMouseDblClick(Input::IMouse::Button btn, int x, int y)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doMouseDblClick(btn, x, y);
		if (epContinue != propagate)
			return propagate;
		// If the event was not managed by UI in this view, try 3D picking
		EventPropagation prop2 = epContinue; // TODO : Event handling on 3D object picking
		return Math::Max(propagate, prop2);
	}


	EventPropagation View::doMouseScroll(float delta, int x, int y)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doMouseScroll(delta, x, y);
		if (epContinue != propagate)
			return propagate;
		// If the event was not managed by UI in this view, try 3D picking
		EventPropagation prop2 = epContinue; // TODO : Event handling on 3D object picking
		return Math::Max(propagate, prop2);
	}


	EventPropagation View::doMouseHover(int x, int y)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doMouseHover(x, y);
		if (epContinue != propagate)
			return propagate;
		// If the event was not managed by UI in this view, try 3D picking
		EventPropagation prop2 = epContinue; // TODO : Event handling on 3D object picking
		return Math::Max(propagate, prop2);
	}


	EventPropagation View::doKeyDown(Input::Key key)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doKeyDown(key);
		return propagate;
	}


	EventPropagation View::doKeyUp(Input::Key key)
	{
		EventPropagation propagate = epContinue;
		// If there are UI controls in this view
		if (!(!pControl))
			// Dispatch the event
			propagate = pControl->doKeyUp(key);
		return propagate;
	}



} // namespace UI
} // namespace Yuni
