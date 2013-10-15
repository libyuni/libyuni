#include "control.h"

namespace Yuni
{
namespace UI
{


	IControl* IControl::getControlAt(int x, int y)
	{
		// Invisible controls don't answer to events
		if (!pVisible)
			return nullptr;
		// If the point is outside the control, return null
		if (x < pPosition.x || y < pPosition.y ||
			x > pPosition.x + (int)pSize.x ||
			y > pPosition.y + (int)pSize.y)
			return nullptr;

		// If no child contains the point, return this control
		IControl* result = this;

		// Try to find a child that contains the point
		int relativeX = x - pPosition.x;
		int relativeY = y - pPosition.y;
		Vector::iterator end = pChildren.end();
		for (Vector::iterator it = pChildren.begin(); it != end; ++it)
		{
			// Recursive call
			IControl* child = (*it)->getControlAt(relativeX, relativeY);
			if (child)
			{
				result = child;
				break;
			}
		}
		return result;
	}

	void IControl::getControlStackAt(int x, int y, std::vector<IControl*>& stack)
	{
		// Invisible controls don't answer to events
		if (!pVisible)
			return;
		// If the point is outside the control, return null
		if (x < pPosition.x || y < pPosition.y ||
			x > pPosition.x + (int)pSize.x ||
			y > pPosition.y + (int)pSize.y)
			return;

		// Push this control in the stack
		stack.push_back(this);

		// Try to find a child that contains the point
		int relativeX = x - pPosition.x;
		int relativeY = y - pPosition.y;
		Vector::iterator end = pChildren.end();
		for (Vector::iterator it = pChildren.begin(); it != end; ++it)
		{
			// Recursive call
			(*it)->getControlStackAt(relativeX, relativeY, stack);
		}
	}

	void IControl::show(bool visible)
	{
		if (visible == pVisible)
			return;
		pVisible = visible;
		// Propagate visibility to children
		for (auto& child : pChildren)
			child->show(visible);
	}


	bool IControl::modified() const
	{
		if (pModified)
			return true;

		// Ask children for modifications
		for (const auto& child : pChildren)
			if (child->modified())
				return true;

		return false;
	}


	EventPropagation IControl::doMouseMove(int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseMove(child, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}

	EventPropagation IControl::doMouseDown(Input::IMouse::Button btn, int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseDown(child, btn, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}

	EventPropagation IControl::doMouseUp(Input::IMouse::Button btn, int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseUp(child, btn, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}

	EventPropagation IControl::doMouseDblClick(Input::IMouse::Button btn, int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseDblClick(child, btn, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}

	EventPropagation IControl::doMouseScroll(float delta, int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseScroll(child, delta);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}


	EventPropagation IControl::doMouseHover(int x, int y)
	{
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = child->onMouseHover(child, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
			prop = onMouseMove(this, x, y);
			if (epStop == prop)
				return epStop;
			else if (prop > finalProp)
				finalProp = prop;
		}
		return finalProp;
	}


	EventPropagation IControl::doKeyDown(Input::Key key)
	{
		// TODO : the managing control should be the top-most one
		return onKeyDown(this, key);
	}


	EventPropagation IControl::doKeyUp(Input::Key key)
	{
		// TODO : the managing control should be the top-most one
		return onKeyUp(this, key);
	}


} // namespace UI
} // namespace Yuni
