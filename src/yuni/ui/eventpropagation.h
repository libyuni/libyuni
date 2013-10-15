#ifndef __YUNI_UI_EVENTPROPAGATION_H__
# define __YUNI_UI_EVENTPROPAGATION_H__

namespace Yuni
{
namespace UI
{

	//! Propagation types for keyboard events
	enum EventPropagation
	{
		//! Continue propagation
		epContinue = 0,

		//! Continue propagation until you have notified all controls in the view
		epFinishView = 1,

		//! Stop propagation
		epStop = 2

	}; // enum EventPropagation


} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_EVENTPROPAGATION_H__
