#ifndef __YUNI_UI_THEME_HXX__
# define __YUNI_UI_THEME_HXX__

namespace Yuni
{
namespace UI
{

	inline Theme::Ptr Theme::Current()
	{
		if (!sCurrent)
			sCurrent = sDefault;
		return sCurrent;
	}


	inline Theme::Ptr Theme::Default()
	{
		return sDefault;
	}



} // namespace UI
} // namespace Yuni

#endif // __YUNI_UI_THEME_HXX__
