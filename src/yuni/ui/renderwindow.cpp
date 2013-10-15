
#include "renderwindow.h"


namespace Yuni
{
namespace UI
{


	RenderWindow::RenderWindow(const AnyString& title, uint width, uint height, uint bitDepth, bool fullScreen):
		pTitle(title),
		pLeft(0),
		pTop(0),
		pWidth(width),
		pHeight(height),
		pResWidth(width),
		pResHeight(height),
		pBitDepth(bitDepth),
		pFB(width, height),
		pPostEffects(0),
		pFullScreen(fullScreen),
		pMultiSampling(msNone),
		pState(fullScreen ? wsMaximized : wsNormal),
		pRefreshFunc(0)
	{
		pActiveView = new View(0, 0, width, height, 127, true);
		pViewList.push_back(pActiveView);
	}


	RenderWindow::~RenderWindow()
	{
		pRefreshFunc.unbind();
		delete pMouse;
	}


	void RenderWindow::kill()
	{
		pRefreshFunc.unbind();
		pActiveView = nullptr;
		pViewList.clear();
		pDefaultFont = nullptr;
	}


	void RenderWindow::clearView(const UUID& viewID)
	{
		clearView(view(viewID));
	}


	void RenderWindow::clearView(const View::Ptr& view)
	{
		if (!view)
			return;
		clearRect(view->x(), view->y(), view->width(), view->height());
	}


	View::Ptr RenderWindow::createView(int x, int y, uint w, uint h, uint8 z)
	{
		View::Ptr newView = new View(x, y, w, h, z);
		newView->initShaders();
		attachView(newView);
		return newView;
	}


	bool RenderWindow::destroyView(const UUID& id)
	{
		// Always keep at least one view
		if (pViewList.size() < 2)
			return false;

		View::Ptr lowestView = nullptr;
		bool found = false;
		View::List::iterator end = pViewList.end();
		for (View::List::iterator it = pViewList.begin(); end != it; ++it)
		{
			if ((*it)->id() == id)
			{
				pViewList.erase(it);
				found = true;
			}
			else if (!lowestView or lowestView->z() > (*it)->z())
				lowestView = *it;
		}
		// If we are deleting the active view, the lowest view becomes the new active view
		if (found and pActiveView->id() == id)
			pActiveView = lowestView;
		return found;
	}


	const View::Ptr& RenderWindow::activeView() const
	{
		return pActiveView;
	}


	View::Ptr RenderWindow::view(const UUID& id) const
	{
		View::List::const_iterator end = pViewList.end();
		for (View::List::const_iterator it = pViewList.begin(); end != it; ++it)
		{
			if ((*it)->id() == id)
				return *it;
		}
		return nullptr;
	}


	bool RenderWindow::activeView(const UUID& id)
	{
		View::List::iterator end = pViewList.end();
		for (View::List::iterator it = pViewList.begin(); end != it; ++it)
			if ((*it)->id() == id)
			{
				pActiveView = *it;
				return true;
			}
		return false;
	}


	bool RenderWindow::activeView(const View::Ptr& view)
	{
		View::List::iterator end = pViewList.end();
		for (View::List::iterator it = pViewList.begin(); end != it; ++it)
		{
			if (view->id() == (*it)->id())
			{
				pActiveView = *it;
				return true;
			}
		}
		return false;
	}


	void RenderWindow::attachView(const View::Ptr& view)
	{
		uint8 z = view->z();
		View::List::iterator end = pViewList.end();
		for (View::List::iterator it = pViewList.begin(); end != it; ++it)
		{
			if ((*it)->z() > z)
			{
				pViewList.insert(it, 1, view);
				return;
			}
		}
		pViewList.push_back(view);
	}


	void RenderWindow::drawView(const UUID& id) const
	{
		if (wsMinimized == pState)
			return;
		View::List::const_iterator end = pViewList.end();
		for (View::List::const_iterator it = pViewList.begin(); end != it; ++it)
		{
			if (id == (*it)->id())
			{
				(*it)->draw();
				break;
			}
		}
	}


	void RenderWindow::drawView(const View::Ptr& view) const
	{
		if (wsMinimized == pState)
			return;
		view->draw();
	}


	void RenderWindow::drawAllViews() const
	{
		if (wsMinimized == pState)
			return;
		// Activate the frame buffer
		pFB.activate();
		// Draw the views
		for (const View::Ptr& view : pViewList)
			view->draw();
		static bool init = false;
		if (!init)
		{
			// pPostEffects.push_back(new Gfx3D::PostEffect("Glow",
			//   	"data/shaders/post/2d.vert", "data/shaders/post/glow.frag",
			//   	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Horizontal Gaussian Blur",
			// 	"data/shaders/post/2d.vert", "data/shaders/post/gaussianblurhorizontal.frag",
			// 	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Vertical Gaussian Blur",
			// 	"data/shaders/post/2d.vert", "data/shaders/post/gaussianblurvertical.frag",
			// 	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Radial Blur",
			// 	"data/shaders/post/rot90left.vert", "data/shaders/post/radialblur.frag",
			// 	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Mirror and Invert colors",
			//  	"data/shaders/post/fliphorizontal.vert", "data/shaders/post/invertcolors.frag",
			//  	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Grayscale",
			//  	"data/shaders/post/2d.vert", "data/shaders/post/crosshatch.frag",
			//  	pWidth, pHeight));
			// pPostEffects.push_back(new Gfx3D::PostEffect("Wave",
			//  	"data/shaders/post/2d.vert", "data/shaders/post/wave.frag",
			//  	pWidth, pHeight));
			init = true;
		}
		// Draw post-processing effects by playing `ping-pong` between 2 colour buffers
		uint i;
		for (i = 0; i + 1 < pPostEffects.size(); ++i)
		{
			auto texture = pFB.texture();
			// Ping-pong !
			pFB.swap();
			// Activate Post shader
			pPostEffects[i]->enableShader();
			drawFullWindowQuad(texture);
		}
		// The last post filter is applied directly, so deactivate our frame buffer
		pFB.deactivate();
		// And draw one last time to the default frame buffer
		if (pPostEffects.size() > 0)
		{
			pPostEffects[i]->enableShader();
			drawFullWindowQuad(pFB.texture());
			pPostEffects[i]->disableShader();
		}
		else
			drawFullWindowQuad(pFB.texture());
	}


	void RenderWindow::multiSampling(MultiSamplingType samplingType)
	{
		if (samplingType != pMultiSampling)
		{
			switch (samplingType)
			{
				case msNone:
					pFB.resize(pWidth, pHeight);
					break;
				case ms2x:
					pFB.resize(pWidth * 2, pHeight * 2);
					break;
				case ms4x:
					pFB.resize(pWidth * 4, pHeight * 4);
					break;
			}
			pMultiSampling = samplingType;
		}
	}


	void RenderWindow::doMouseMove(int x, int y)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseMove(x, y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doMove(x, y);
	}

	void RenderWindow::doMouseDown(Input::IMouse::Button btn)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseDown(btn, pMouse->pos().x, pMouse->pos().y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doDown(btn);
	}

	void RenderWindow::doMouseUp(Input::IMouse::Button btn)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseUp(btn, pMouse->pos().x, pMouse->pos().y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doUp(btn);
	}

	void RenderWindow::doMouseDblClick(Input::IMouse::Button btn)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseDblClick(btn, pMouse->pos().x, pMouse->pos().y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doDblClick(btn);
	}

	void RenderWindow::doMouseScroll(float delta)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseScroll(delta, pMouse->pos().x, pMouse->pos().y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doScroll(delta);
	}

	void RenderWindow::doMouseHover(int x, int y)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doMouseHover(x, y);
			if (epFinishView <= propagate)
				break;
		}
		pMouse->doHover(x, y);
	}


	void RenderWindow::doKeyDown(Input::Key key)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doKeyDown(key);
			if (epFinishView <= propagate)
				break;
		}
		pKeyboard.doDown(key);
	}


	void RenderWindow::doKeyUp(Input::Key key)
	{
		EventPropagation propagate = epContinue;
		for (const View::Ptr& view : pViewList)
		{
			propagate = view->doKeyUp(key);
			if (epFinishView <= propagate)
				break;
		}
		pKeyboard.doUp(key);
	}



} // namespace UI
} // namespace Yuni


