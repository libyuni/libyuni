#include "control.h"

namespace Yuni
{
namespace UI
{


	IControl* IControl::getControlAt(float x, float y)
	{
		// Invisible controls don't answer to events
		if (!pVisible)
			return nullptr;
		// If the point is outside the control, return null
		if (x < pPosition.x || y < pPosition.y ||
			x > pPosition.x + pSize.x ||
			y > pPosition.y + pSize.y)
			return nullptr;

		// If no child contains the point, return this control
		IControl* result = this;

		// Try to find a child that contains the point
		float relativeX = x - pPosition.x;
		float relativeY = y - pPosition.y;
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

	void IControl::getControlStackAt(float x, float y, std::vector<IControl*>& stack)
	{
		// Invisible controls don't answer to events
		if (!pVisible)
			return;
		// If the point is outside the control, return null
		if (x < pPosition.x || y < pPosition.y ||
			x > pPosition.x + pSize.x ||
			y > pPosition.y + pSize.y)
			return;

		// Push this control in the stack
		stack.push_back(this);

		// Try to find a child that contains the point
		float relativeX = x - pPosition.x;
		float relativeY = y - pPosition.y;
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
		invalidate();
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


	EventPropagation IControl::doMouseMove(float x, float y, Set& enteredControls)
	{
		if (!pVisible)
			return epContinue;

		// Check first which controls have been left
		if (not enteredControls.empty())
		{
			Vector toRemoveControls;
			for (auto control : enteredControls)
			{
				if (!control->contains(x, y))
				{
					// Control has been left
					toRemoveControls.push_back(control);
					control->doMouseLeave(x, y);
				}
			}
			for (auto toRemove : toRemoveControls)
				enteredControls.erase(toRemove);
		}

		// Get the stack of controls under the (x,y) point
		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();

			// Mouse enter check
			if (enteredControls.end() == enteredControls.find(child))
			{
				enteredControls.insert(child);
				child->onMouseEnter(child, x, y);
			}
			// Allow the control to react to the event before the callback
			child->mouseMove(x, y);
			// Event callback
			EventPropagation prop = eventFold(child->onMouseMove, child, x, y);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		// Allow the control to react to the event before the callback
		mouseMove(x, y);
		// Event callback on the root control
		EventPropagation prop = eventFold(onMouseMove, this, x, y);
		return Math::Max(prop, finalProp);
	}

	EventPropagation IControl::doMouseDown(Input::IMouse::Button btn, float x, float y)
	{
		if (!pVisible)
			return epContinue;

		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			child->mouseDown(btn, x, y);
			EventPropagation prop = eventFold(child->onMouseDown, child, btn, x, y);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		mouseDown(btn, x, y);
		EventPropagation prop = eventFold(onMouseDown, this, btn, x, y);
		return Math::Max(prop, finalProp);
	}

	EventPropagation IControl::doMouseUp(Input::IMouse::Button btn, float x, float y)
	{
		if (!pVisible)
			return epContinue;

		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			child->mouseUp(btn, x, y);
			EventPropagation prop = eventFold(child->onMouseUp, child, btn, x, y);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		mouseUp(btn, x, y);
		EventPropagation prop = eventFold(onMouseUp, this, btn, x, y);
		return Math::Max(prop, finalProp);
	}

	EventPropagation IControl::doMouseDblClick(Input::IMouse::Button btn, float x, float y)
	{
		if (!pVisible)
			return epContinue;

		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			child->mouseDblClick(btn, x, y);
			EventPropagation prop = eventFold(child->onMouseDblClick, child, btn, x, y);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		mouseDblClick(btn, x, y);
		EventPropagation prop = eventFold(onMouseDblClick, this, btn, x, y);
		return Math::Max(prop, finalProp);
	}

	EventPropagation IControl::doMouseScroll(float delta, float x, float y)
	{
		if (!pVisible)
			return epContinue;

		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			child->mouseScroll(delta, x, y);
			EventPropagation prop = eventFold(child->onMouseScroll, child, delta);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		mouseScroll(delta, x, y);
		EventPropagation prop = eventFold(onMouseScroll, this, delta);
		return Math::Max(prop, finalProp);
	}


	EventPropagation IControl::doMouseEnter(float x, float y)
	{
		EventPropagation prop = eventFold(onMouseEnter, this, x, y);
		if (epStop == prop)
			return epStop;
		return eventFold(onMouseEnter, this, x, y);
	}

	EventPropagation IControl::doMouseLeave(float x, float y)
	{
		EventPropagation prop = eventFold(onMouseLeave, this, x, y);
		if (epStop == prop)
			return epStop;
		return eventFold(onMouseLeave, this, x, y);
	}


	EventPropagation IControl::doMouseHover(float x, float y)
	{
		if (!pVisible)
			return epContinue;

		std::vector<IControl*> stack;
		getControlStackAt(x, y, stack);
		EventPropagation finalProp = epContinue;
		while (!stack.empty())
		{
			IControl* child = stack.back();
			stack.pop_back();
			EventPropagation prop = eventFold(child->onMouseHover, child, x, y);
			if (epStop == prop)
				return epStop;
			if (prop > finalProp)
				finalProp = prop;
		}
		EventPropagation prop = eventFold(onMouseHover, this, x, y);
		return Math::Max(prop, finalProp);
	}


	EventPropagation IControl::doKeyDown(Input::Key key)
	{
		// TODO : the managing control should be the top-most one
		return eventFold(onKeyDown, this, key);
	}


	EventPropagation IControl::doKeyUp(Input::Key key)
	{
		// TODO : the managing control should be the top-most one
		return eventFold(onKeyUp, this, key);
	}


} // namespace UI
} // namespace Yuni
