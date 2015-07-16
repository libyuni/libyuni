/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#ifndef __YUNI_EXTENSION_UI_ANIMATION_APPLY_H__
# define __YUNI_EXTENSION_UI_ANIMATION_APPLY_H__

# include <yuni/core/bind.h>
# include <yuni/core/point2D.h>
# include <yuni/core/point3D.h>
# include <yuni/core/color/rgb.h>
# include <yuni/core/color/rgba.h>
# include "easing.h"

namespace Yuni
{
namespace Extension
{
namespace UI
{
namespace Animation
{

	//! Easing function prototype
	typedef Yuni::UI::Animation::Easing::Func EasingFunc;


	template<class T>
	class Apply final
	{
	public:
		static T Transition(const EasingFunc& easing, const T& start, const T& end, float ratio)
		{
			return start + (end - start) * easing(ratio);
		}
	};



	template<class T>
	class Apply<Point2D<T> > final
	{
	public:
		static Point2D<T> Transition(const EasingFunc& easing,
			const Point2D<T>& start, const Point2D<T>& end, float ratio)
		{
			float newRatio = easing(ratio);
			return Point2D<T>(
				start.x + (end.x - start.x) * newRatio,
				start.y + (end.y - start.y) * newRatio);
		}
	};


	template<class T>
	class Apply<Point3D<T> > final
	{
	public:
		static Point3D<T> Transition(const EasingFunc& easing,
			const Point3D<T>& start, const Point3D<T>& end, float ratio)
		{
			float newRatio = easing(ratio);
			return Point3D<T>(
				start.x + (end.x - start.x) * newRatio,
				start.y + (end.y - start.y) * newRatio,
				start.z + (end.z - start.z) * newRatio);
		}
	};


	template<class T>
	class Apply<Yuni::Color::RGB<T> > final
	{
	public:
		static Yuni::Color::RGB<T> Transition(const EasingFunc& easing,
			const Yuni::Color::RGB<T>& start, const Yuni::Color::RGB<T>& end, float ratio)
		{
			float newRatio = easing(ratio);
			return Yuni::Color::RGB<T>(
				start.red + (end.red - start.red) * newRatio,
				start.green + (end.green - start.green) * newRatio,
				start.blue + (end.blue - start.blue) * newRatio);
		}
	};


	template<class T>
	class Apply<Yuni::Color::RGBA<T> > final
	{
	public:
		static Yuni::Color::RGBA<T> Transition(const EasingFunc& easing,
			const Yuni::Color::RGBA<T>& start, const Yuni::Color::RGBA<T>& end, float ratio)
		{
			float newRatio = easing(ratio);
			return Yuni::Color::RGBA<T>(
				start.red + (end.red - start.red) * newRatio,
				start.green + (end.green - start.green) * newRatio,
				start.blue + (end.blue - start.blue) * newRatio,
				start.alpha + (end.alpha - start.alpha) * newRatio);
		}
	};


} // namespace Animation
} // namespace UI
} // namespace Extension
} // namespace Yuni

#endif // __YUNI_EXTENSION_UI_ANIMATION_APPLY_H__
