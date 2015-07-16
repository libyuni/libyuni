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
#pragma once

#include <yuni/core/math.h>
#include <yuni/core/math/trigonometric.h>


namespace Yuni
{
namespace UI
{
namespace Animation
{
namespace Easing
{

	//! Prototype for an interpolation / easing function (cf Robert Penner's equations)
	typedef Yuni::Bind<float (float ratio)>  Func;


	namespace // anonymous
	{
		template<class T>
		inline float Invert(const T& func, float ratio)
		{
			return 1.0f - func(1.0f - ratio);
		}

		template<class T>
		inline float MakeInOut(const T& func, float ratio)
		{
			return (ratio < 0.5f) ?
				func(ratio * 2.0f) / 2.0f :
				1.0f - func((1.0f - ratio) * 2.0f) / 2.0f;
		}

	} // namespace anonymous



	inline float None(float /*ratio*/)
	{
		return 0.0f;
	}


	inline float Linear(float ratio)
	{
		return ratio;
	}


	inline float QuadIn(float ratio)
	{
		return ratio * ratio;
	}


	inline float QuadOut(float ratio)
	{
		return Invert(QuadIn, ratio);
	}


	inline float QuadInOut(float ratio)
	{
		return MakeInOut(QuadIn, ratio);
	}


	inline float CubicIn(float ratio)
	{
		return Math::Power(ratio, 3);
	}


	inline float CubicOut(float ratio)
	{
		return Invert(CubicIn, ratio);
	}


	inline float CubicInOut(float ratio)
	{
		return MakeInOut(CubicIn, ratio);
	}


	inline float QuarticIn(float ratio)
	{
		return Math::Power(ratio, 4);
	}


	inline float QuarticOut(float ratio)
	{
		return Invert(QuarticIn, ratio);
	}


	inline float QuarticInOut(float ratio)
	{
		return MakeInOut(QuarticIn, ratio);
	}


	inline float QuinticIn(float ratio)
	{
		return Math::Power(ratio, 5);
	}


	inline float QuinticOut(float ratio)
	{
		return Invert(QuinticIn, ratio);
	}


	inline float QuinticInOut(float ratio)
	{
		return MakeInOut(QuinticIn, ratio);
	}


	inline float CircIn(float ratio)
	{
		return 1.0f - Math::SquareRootNoCheck(1.0f - ratio * ratio);
	}


	inline float CircOut(float ratio)
	{
		return Invert(CircIn, ratio);
	}


	inline float CircInOut(float ratio)
	{
		return MakeInOut(CircIn, ratio);
	}


	inline float Back(float ratio)
	{
		static const float v = 1.70158f;
		ratio -= 1.0f;
		return ratio * ratio * ((v + 1.0f) * ratio + v) + 1.0f;
	}


	inline float ElasticOut(float ratio)
	{
		static const float p = 0.3;
		return (float)(Math::Power(2, -10 * ratio) *
			Math::Sin((ratio - p / 4.0) * (2.0 * YUNI_PI) / p) + 1.0);
	}

	inline float ElasticIn(float ratio)
	{
		return Invert(ElasticOut, ratio);
	}


	inline float ElasticInOut(float ratio)
	{
		return MakeInOut(ElasticIn, ratio);
	}


	inline float BounceOut(float ratio)
	{
		if (ratio < 1 / 2.75)
		{
			return 7.5625 * ratio * ratio;
		}
		else if (ratio < 2 / 2.75)
		{
			ratio -= 1.5 / 2.75;
			return 7.5625 * ratio * ratio + .75;
		}
		else if (ratio < 2.5 / 2.75)
		{
			ratio -= 2.25 / 2.75;
			return 7.5625 * ratio * ratio + .9375;
		}
		ratio -= 2.625 / 2.75;
		return 7.5625 * ratio * ratio + .984375;
	}


	inline float BounceIn(float ratio)
	{
		return Invert(BounceOut, ratio);
	}



} // namespace Easing
} // namespace Animation
} // namespace UI
} // namespace Yuni
