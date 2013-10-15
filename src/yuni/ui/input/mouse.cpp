#include "mouse.h"

namespace Yuni
{
namespace Input
{

	void IMouse::doMove(int x, int y)
	{
		pLastPos(pPos);
		pPos(x, y);
		onMove(x, y);
	}

	void IMouse::doDown(Button btn)
	{
		pButtonClicked[btn] = true;
		pDragPos(pPos);
		onButtonDown(btn, pPos.x, pPos.y);
	}

	void IMouse::doUp(Button btn)
	{
		pButtonClicked[btn] = false;
		onButtonUp(btn, pPos.x, pPos.y);
	}

	void IMouse::doDblClick(Button btn)
	{
		onDblClick(btn, pPos.x, pPos.y);
	}

	void IMouse::doScroll(float delta)
	{
		onScroll(delta);
	}

	void IMouse::doHover(int x, int y)
	{
		onHover(x, y);
	}


} // namespace Input
} // namespace Yuni
