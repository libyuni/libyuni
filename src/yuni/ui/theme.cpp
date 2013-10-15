# include "theme.h"

namespace Yuni
{
namespace UI
{

	Theme::Ptr Theme::sDefault = new Theme();

	Theme::Ptr Theme::sCurrent = nullptr;

	Theme::Theme():
		font(new FTFont("data/fonts/OpenSans-Light.ttf", 16)),
		textColor(0.6274f, 0.7882f, 0.8509f), // (160,201,217)
 		windowColor(0.2274f, 0.3804f, 0.5412f, 0.8f), // (58, 97, 138) 80% alpha
		buttonColor(0.2863f, 0.5451f, 0.651f), // (73, 139, 166)
		buttonColorDisabled(0.2863f, 0.5451f, 0.651f), // (103, 120, 139)
		menuColor(0.2863f, 0.5451f, 0.651f), // (73, 139, 166)
		menuColorDisabled(0.2863f, 0.5451f, 0.651f), // (103, 120, 139)
		borderColor(0.1882f, 0.3490f, 0.451f), // (48, 89, 115)
		borderColorDisabled(0.1882f, 0.3490f, 0.451f), // (102, 100, 126)
		margin(10),
		borderWidth(3),
		menuHeight(20),
		checkBoxSize(15)
	{}


	Theme::Theme(const Theme& other):
		font(other.font),
		textColor(other.textColor),
 		windowColor(other.windowColor),
		buttonColor(other.buttonColor),
		buttonColorDisabled(other.buttonColorDisabled),
		menuColor(other.menuColor),
		menuColorDisabled(other.menuColorDisabled),
		borderColor(other.borderColor),
		borderColorDisabled(other.borderColorDisabled),
		margin(other.margin),
		borderWidth(other.borderWidth),
		menuHeight(other.menuHeight),
		checkBoxSize(other.checkBoxSize)
	{}


	void Theme::SwitchTo(const Theme::Ptr& newTheme)
	{
		if (!(!newTheme))
			sCurrent = newTheme;
	}


	void Theme::SwitchToDefault()
	{
		sCurrent = sDefault;
	}



} // namespace UI
} // namespace Yuni
