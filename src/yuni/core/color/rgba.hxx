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
#include "rgba.h"



namespace Yuni
{
namespace Extension
{
namespace Color
{

	template<class RGBType, class T>
	struct RGBA final
	{
		typedef typename  RGBType::Type Type;
		typedef typename  RGBType::template Calculation<T>::Type CalculationType;

		static inline void Assign(RGBType& color, const T& value)
		{
			enum
			{
				upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
				lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
			};
			// new value
			const CalculationType v = ((int)RGBType::upperBound == (int)upperBound and (int)RGBType::lowerBound == (int)lowerBound)
				? static_cast<CalculationType>(value)
				: static_cast<CalculationType>(value) * RGBType::upperBound / upperBound;
			// assignment
			color.red   = v;
			color.green = v;
			color.blue  = v;
			color.alpha = v;
		}

		static bool IsEqual(RGBType& color, const T& value)
		{
			enum
			{
				upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
				lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
			};
			// new value
			const CalculationType v = ((int)RGBType::upperBound == (int)upperBound and (int)RGBType::lowerBound == (int)lowerBound)
				? static_cast<CalculationType>(value)
				: static_cast<CalculationType>(value) * RGBType::upperBound / upperBound;

			return Math::Equals(color.red, v)
				and Math::Equals(color.green, v)
				and Math::Equals(color.blue, v)
				and Math::Equals(color.alpha, v);
		}

		template<bool PlusT>
		static inline void Increase(RGBType& color, const T& value)
		{
			enum
			{
				upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
				lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
			};
			// new value
			const CalculationType v = ((int)RGBType::upperBound == (int)upperBound and (int)RGBType::lowerBound == (int)lowerBound)
				? static_cast<CalculationType>(value)
				: static_cast<CalculationType>(value) * RGBType::upperBound / upperBound;
			// assignment
			if (PlusT)
			{
				color.red   = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.red)   + v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.green = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.green) + v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.blue  = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.blue)  + v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.alpha = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.alpha) + v, (CalculationType)lowerBound, (CalculationType)upperBound);
			}
			else
			{
				color.red   = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.red)   - v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.green = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.green) - v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.blue  = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.blue)  - v, (CalculationType)lowerBound, (CalculationType)upperBound);
				color.alpha = Math::MinMaxEx<Type>(static_cast<CalculationType>(color.alpha) - v, (CalculationType)lowerBound, (CalculationType)upperBound);
			}
		}

	}; // class RGBA


	template<class U, class V>
	struct RGBA<Yuni::Color::RGBA<U>, Yuni::Color::RGBA<V> > final
	{
		typedef typename Yuni::Color::RGBA<U>::template Calculation<V>::Type CalculationType;

		static inline void Assign(Yuni::Color::RGBA<U>& color, const Yuni::Color::RGBA<V>& value)
		{

			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGBA<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGBA<V>::lowerBound)
			{
				color.red   = static_cast<U>(value.red);
				color.green = static_cast<U>(value.green);
				color.blue  = static_cast<U>(value.blue);
				color.alpha = static_cast<U>(value.alpha);
			}
			else
			{
				color.red   = static_cast<U>(static_cast<CalculationType>(value.red)   * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound);
				color.green = static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound);
				color.blue  = static_cast<U>(static_cast<CalculationType>(value.blue)  * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound);
				color.alpha = static_cast<U>(static_cast<CalculationType>(value.alpha) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound);
			}
		}

		static inline bool IsEqual(const Yuni::Color::RGBA<U>& color, const Yuni::Color::RGBA<V>& value)
		{
			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGBA<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGBA<V>::lowerBound)
			{
				return
					Math::Equals(color.red, static_cast<U>(value.red))
					and Math::Equals(color.green, static_cast<U>(value.green))
					and Math::Equals(color.blue, static_cast<U>(value.blue))
					and Math::Equals(color.alpha, static_cast<U>(value.alpha));
			}
			else
			{
				return
					Math::Equals(color.red, static_cast<U>(static_cast<CalculationType>(value.red)   * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound))
					and Math::Equals(color.green, static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound))
					and Math::Equals(color.blue, static_cast<U>(static_cast<CalculationType>(value.blue)  * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound))
					and Math::Equals(color.alpha, static_cast<U>(static_cast<CalculationType>(value.alpha) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound));
			}
		}

		template<bool PlusT>
		static inline void Increase(Yuni::Color::RGBA<U>& color, const Yuni::Color::RGBA<V>& value)
		{
			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGBA<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGBA<V>::lowerBound)
			{
				if (PlusT)
				{
					color.red   = Math::MinMaxEx<U>(static_cast<CalculationType>(color.red)   + static_cast<U>(value.red),   Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(static_cast<CalculationType>(color.green) + static_cast<U>(value.green), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue  = Math::MinMaxEx<U>(static_cast<CalculationType>(color.blue)  + static_cast<U>(value.blue),  Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.alpha = Math::MinMaxEx<U>(static_cast<CalculationType>(color.alpha) + static_cast<U>(value.alpha), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
				else
				{
					color.red   = Math::MinMaxEx<U>(static_cast<CalculationType>(color.red)   - static_cast<U>(value.red),   Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(static_cast<CalculationType>(color.green) - static_cast<U>(value.green), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue  = Math::MinMaxEx<U>(static_cast<CalculationType>(color.blue)  - static_cast<U>(value.blue),  Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.alpha = Math::MinMaxEx<U>(static_cast<CalculationType>(color.alpha) - static_cast<U>(value.alpha), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
			}
			else
			{
				if (PlusT)
				{
					color.red = Math::MinMaxEx<U>(
						color.red   + static_cast<U>(static_cast<CalculationType>(value.red) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(
						color.green + static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue = Math::MinMaxEx<U>(
						color.blue  + static_cast<U>(static_cast<CalculationType>(value.blue) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.alpha = Math::MinMaxEx<U>(
						color.alpha + static_cast<U>(static_cast<CalculationType>(value.alpha) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
				else
				{
					color.red = Math::MinMaxEx<U>(
						color.red   - static_cast<U>(static_cast<CalculationType>(value.red) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(
						color.green - static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue = Math::MinMaxEx<U>(
						color.blue  - static_cast<U>(static_cast<CalculationType>(value.blue) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.alpha = Math::MinMaxEx<U>(
						color.alpha - static_cast<U>(static_cast<CalculationType>(value.alpha) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGBA<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
			}
		}

	}; // class RGBA



	template<class U, class V>
	struct RGBA<Yuni::Color::RGBA<U>, Yuni::Color::RGB<V> > final
	{
		typedef typename Yuni::Color::RGBA<U>::template Calculation<V>::Type CalculationType;

		static inline void Assign(Yuni::Color::RGBA<U>& color, const Yuni::Color::RGB<V>& value)
		{
			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGB<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGB<V>::lowerBound)
			{
				color.red   = static_cast<U>(value.red);
				color.green = static_cast<U>(value.green);
				color.blue  = static_cast<U>(value.blue);
			}
			else
			{
				color.red   = static_cast<U>(static_cast<CalculationType>(value.red)   * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound);
				color.green = static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound);
				color.blue  = static_cast<U>(static_cast<CalculationType>(value.blue)  * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound);
			}
			color.alpha = static_cast<U>(Yuni::Color::RGBA<U>::upperBound);
		}

		static inline bool IsEqual(Yuni::Color::RGBA<U>& color, const Yuni::Color::RGB<V>& value)
		{
			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGB<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGB<V>::lowerBound)
			{
				return
					Math::Equals(value.alpha, Yuni::Color::RGBA<U>::upperBound)
					and Math::Equals(color.red, static_cast<U>(value.red))
					and Math::Equals(color.green, static_cast<U>(value.green))
					and Math::Equals(color.blue, static_cast<U>(value.blue));
			}
			else
			{
				return
					Math::Equals(value.alpha, Yuni::Color::RGBA<U>::upperBound)
					and Math::Equals(color.red, static_cast<U>(static_cast<CalculationType>(value.red)   * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound))
					and Math::Equals(color.green, static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound))
					and Math::Equals(color.blue, static_cast<U>(static_cast<CalculationType>(value.blue)  * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound));
			}
		}

		template<bool PlusT>
		static inline void Increase(Yuni::Color::RGBA<U>& color, const Yuni::Color::RGB<V>& value)
		{
			if ((int)Yuni::Color::RGBA<U>::upperBound == (int)Yuni::Color::RGB<V>::upperBound
				and (int)Yuni::Color::RGBA<U>::lowerBound == (int)Yuni::Color::RGB<V>::lowerBound)
			{
				if (PlusT)
				{
					color.red   = Math::MinMaxEx<U>(static_cast<CalculationType>(color.red)   + static_cast<U>(value.red),   Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(static_cast<CalculationType>(color.green) + static_cast<U>(value.green), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue  = Math::MinMaxEx<U>(static_cast<CalculationType>(color.blue)  + static_cast<U>(value.blue),  Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
				else
				{
					color.red   = Math::MinMaxEx<U>(static_cast<CalculationType>(color.red)   - static_cast<U>(value.red),   Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(static_cast<CalculationType>(color.green) - static_cast<U>(value.green), Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue  = Math::MinMaxEx<U>(static_cast<CalculationType>(color.blue)  - static_cast<U>(value.blue),  Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
			}
			else
			{
				if (PlusT)
				{
					color.red = Math::MinMaxEx<U>(
						color.red   + static_cast<U>(static_cast<CalculationType>(value.red) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(
						color.green + static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue = Math::MinMaxEx<U>(
						color.blue  + static_cast<U>(static_cast<CalculationType>(value.blue) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
				else
				{
					color.red = Math::MinMaxEx<U>(
						color.red   - static_cast<U>(static_cast<CalculationType>(value.red) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.green = Math::MinMaxEx<U>(
						color.green - static_cast<U>(static_cast<CalculationType>(value.green) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
					color.blue = Math::MinMaxEx<U>(
						color.blue  - static_cast<U>(static_cast<CalculationType>(value.blue) * Yuni::Color::RGBA<U>::upperBound / Yuni::Color::RGB<V>::upperBound),
						Yuni::Color::RGBA<U>::lowerBound, Yuni::Color::RGBA<U>::upperBound);
				}
			}
		}

	}; // class RGBA



} // namespace Color
} // namespace Extension
} // namespace Yuni


namespace Yuni
{
namespace Color
{

	template<class T>
	inline RGBA<T>::RGBA()
		: red(lowerBound), green(lowerBound), blue(lowerBound), alpha(upperBound) // Opaque black
	{}


	template<class T>
	inline RGBA<T>::RGBA(const RGBA<T>& rhs)
		: red(rhs.red), green(rhs.green), blue(rhs.blue), alpha(rhs.alpha)
	{}


	template<class T>
	template<class U>
	inline RGBA<T>::RGBA(const U& value)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, U>::Assign(*this, value);
	}


	template<class T>
	template<class R, class G, class B, class A>
	inline RGBA<T>::RGBA(R r, G g, B b, A a)
		: red(Math::MinMaxEx<T>(r, static_cast<R>(lowerBound), static_cast<R>(upperBound)))
		, green(Math::MinMaxEx<T>(g, static_cast<G>(lowerBound), static_cast<G>(upperBound)))
		, blue(Math::MinMaxEx<T>(b, static_cast<B>(lowerBound), static_cast<B>(upperBound)))
		, alpha(Math::MinMaxEx<T>(a, static_cast<A>(lowerBound), static_cast<A>(upperBound)))
	{}


	template<class T>
	template<class R, class G, class B>
	inline RGBA<T>::RGBA(R r, G g, B b)
		: red( Math::MinMaxEx<T>(r, static_cast<R>(lowerBound), static_cast<R>(upperBound)))
		, green(Math::MinMaxEx<T>(g, static_cast<G>(lowerBound), static_cast<G>(upperBound)))
		, blue( Math::MinMaxEx<T>(b, static_cast<B>(lowerBound), static_cast<B>(upperBound)))
		, alpha(static_cast<T>(Yuni::Private::Color::DefaultValues<T>::upperBound))
	{}


	template<class T>
	template<class R, class G, class B, class A>
	inline void RGBA<T>::assign(R r, G g, B b, A a)
	{
		red   = Math::MinMaxEx<T>(r, static_cast<R>(lowerBound), static_cast<R>(upperBound));
		green = Math::MinMaxEx<T>(g, static_cast<G>(lowerBound), static_cast<G>(upperBound));
		blue  = Math::MinMaxEx<T>(b, static_cast<B>(lowerBound), static_cast<B>(upperBound));
		alpha = Math::MinMaxEx<T>(a, static_cast<A>(lowerBound), static_cast<A>(upperBound));
	}


	template<class T>
	template<class R, class G, class B>
	inline void RGBA<T>::assign(R r, G g, B b)
	{
		red   = Math::MinMaxEx<T>(r, static_cast<R>(lowerBound), static_cast<R>(upperBound));
		green = Math::MinMaxEx<T>(g, static_cast<G>(lowerBound), static_cast<G>(upperBound));
		blue  = Math::MinMaxEx<T>(b, static_cast<B>(lowerBound), static_cast<B>(upperBound));
		alpha = static_cast<T>(Yuni::Private::Color::DefaultValues<T>::upperBound);
	}


	template<class T>
	template<class U>
	inline void RGBA<T>::assign(const U& value)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, U>::Assign(*this, value);
	}


	template<class T>
	inline bool RGBA<T>::valid() const
	{
		return
			(    red   >= static_cast<T>(lowerBound) and red   <= static_cast<T>(upperBound))
			and (green >= static_cast<T>(lowerBound) and green <= static_cast<T>(upperBound))
			and (blue  >= static_cast<T>(lowerBound) and blue  <= static_cast<T>(upperBound))
			and (alpha >= static_cast<T>(lowerBound) and alpha <= static_cast<T>(upperBound));
	}


	template<class T>
	inline bool RGBA<T>::visible() const
	{
		return Yuni::Math::Equals<T>(alpha, static_cast<T>(lowerBound));
	}


	template<class T>
	inline RGBA<T>& RGBA<T>::operator = (const Yuni::NullPtr&)
	{
		red   = T();
		green = T();
		blue  = T();
		alpha = T();
		return *this;
	}


	template<class T>
	inline RGBA<T>& RGBA<T>::operator = (const RGBA<T>& rhs)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, RGBA<T> >::Assign(*this, rhs);
		return *this;
	}


	template<class T>
	template<class U>
	inline RGBA<T>& RGBA<T>::operator = (const U& rhs)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, U>::Assign(*this, rhs);
		return *this;
	}


	template<class T>
	template<class U>
	inline bool RGBA<T>::operator != (const U& rhs) const
	{
		return ! Yuni::Extension::Color::RGBA<RGBA<T>, U>::IsEqual(*this, rhs);
	}


	template<class T>
	template<class U>
	inline bool RGBA<T>::operator == (const U& rhs) const
	{
		return Yuni::Extension::Color::RGBA<RGBA<T>, U>::IsEqual(*this, rhs);
	}


	template<class T>
	template<class U>
	inline RGBA<T>& RGBA<T>::operator += (const U& rhs)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, U>::template Increase<true>(*this, rhs);
		return *this;
	}


	template<class T>
	template<class U>
	inline RGBA<T>& RGBA<T>::operator -= (const U& rhs)
	{
		Yuni::Extension::Color::RGBA<RGBA<T>, U>::template Increase<false>(*this, rhs);
		return *this;
	}


	template<class T>
	inline T& RGBA<T>::operator [] (const uint i)
	{
		switch (i)
		{
			case 0:
				return red;
			case 1:
				return green;
			case 2:
				return blue;
			case 3:
				return alpha;
			default:
				assert(false and "Index out of bounds !");
		}
		return red;
	}


	template<class T>
	template<class StreamT>
	void RGBA<T>::print(StreamT& stream) const
	{
		stream << "rgba("
			<< static_cast<PrintType>(red) << ','
			<< static_cast<PrintType>(green) << ','
			<< static_cast<PrintType>(blue) << ','
			<< static_cast<PrintType>(alpha) << ')';
	}




} // namespace Color
} // namespace Yuni


namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT, class T>
	class Append<CStringT, Yuni::Color::RGBA<T> > final
	{
	public:
		static void Perform(CStringT& s, const Yuni::Color::RGBA<T>& rhs)
		{
			rhs.print(s);
		}
	};


	template<class T>
	class Into<Yuni::Color::RGBA<T> > final
	{
	public:
		typedef Yuni::Color::RGBA<T> TargetType;
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, TargetType& out)
		{
			if (s.empty())
			{
				out = nullptr;
				return true;
			}
			if (!InternalPerform(s, out))
			{
				out = nullptr;
				return false;
			}
			return true;
		}

		template<class StringT> static TargetType Perform(const StringT& s)
		{
			return TargetType(s.c_str(), s.size());
		}

	private:
		template<class StringT>
		static bool InternalPerform(const StringT& s, TargetType& out)
		{
			enum
			{
				upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
				lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
			};

			const typename StringT::const_utf8iterator end = s.utf8end();
			typename StringT::const_utf8iterator i = s.utf8begin();

			if (!i.findFirstNonSpace(end.offset()))
			{
				out = nullptr; // empty string
				return true;
			}
			// trimming
			if (*i != 'r' or ++i == end or *i != 'g' or ++i == end or *i != 'b' or ++i == end)
				return false;
			bool hasAlpha = (*i == 'a');
			if (hasAlpha and ++i == end)
				return false;
			if (*i != '(')
			{
				if (!i.findFirstNonSpace(end.offset()) or *i != '(')
					return false;
			}
			if (++i == end or !i.findFirstNonSpace(end.offset()))
			{
				out = nullptr; // empty string
				return true;
			}

			// A temporary string buffer for convertion
			Yuni::CString<30, false> tmp;
			// A copy of the offset
			typename StringT::const_utf8iterator j = i;
			// A temporary value for calculations
			typename Yuni::Color::RGBA<T>::template Calculation<T>::Type channel;

			// RED CHANNEL
			if (!i.find(',', end.offset()))
				return false;
			tmp.assign(j, i);
			if (!tmp.to(channel) or ++i == end or !i.findFirstNonSpace(end.offset()))
				return false;
			out.red = Math::MinMaxEx<T>(channel, static_cast<T>(lowerBound), static_cast<T>(upperBound));

			// GREEN CHANNEL
			j = i;
			if (!i.find(',', end.offset()))
				return false;
			tmp.assign(j, i);
			if (!tmp.to(channel) or ++i == end or !i.findFirstNonSpace(end.offset()))
				return false;
			out.green = Math::MinMaxEx<T>(channel, static_cast<T>(lowerBound), static_cast<T>(upperBound));

			if (hasAlpha)
			{
				// BLUE CHANNEL
				j = i;
				if (!i.find(',', end.offset()))
					return false;
				tmp.assign(j, i);
				if (!tmp.to(channel) or ++i == end or !i.findFirstNonSpace(end.offset()))
					return false;
				out.blue = Math::MinMaxEx<T>(channel, static_cast<T>(lowerBound), static_cast<T>(upperBound));

				// ALPHA CHANNEL
				j = i;
				if (!i.find(')', end.rawOffset()))
					return false;
				tmp.assign(j, i);
				if (!tmp.to(channel))
					return false;
				out.alpha = Math::MinMaxEx<T>(channel, static_cast<T>(lowerBound), static_cast<T>(upperBound));
			}
			else
			{
				// BLUE CHANNEL
				j = i;
				if (!i.find(')', end.rawOffset()))
					return false;
				tmp.assign(j, i);
				if (!tmp.to(channel))
					return false;
				out.blue = static_cast<T>(channel);

				// default value for alpha
				out.alpha = static_cast<T>(upperBound);
			}

			return true;
		}
	};


} // namespace CString
} // namespace Extension
} // namespace Yuni


template<class T>
inline std::ostream& operator << (std::ostream& out, const Yuni::Color::RGBA<T>& rhs)
{
	rhs.print(out);
	return out;
}


template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator + (const Yuni::Color::RGBA<U> a, const Yuni::Color::RGBA<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r += b;
	return r;
}

template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator - (const Yuni::Color::RGBA<U>& a, const Yuni::Color::RGBA<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r -= b;
	return r;
}


template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator + (const Yuni::Color::RGB<U> a, const Yuni::Color::RGBA<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r += b;
	return r;
}

template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator - (const Yuni::Color::RGB<U>& a, const Yuni::Color::RGBA<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r -= b;
	return r;
}

template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator + (const Yuni::Color::RGBA<U> a, const Yuni::Color::RGB<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r += b;
	return r;
}

template<class U, class V>
inline
Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type>
operator - (const Yuni::Color::RGBA<U>& a, const Yuni::Color::RGB<V>& b)
{
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<U>::template Calculation<V>::Type> r = a;
	r -= b;
	return r;
}



# define YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(V) \
	template<class T> \
	inline \
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> \
	operator + (V v, const Yuni::Color::RGBA<T>& rhs) \
	{ \
		Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> r = rhs; \
		r += v; \
		return r; \
	} \
	template<class T> \
	inline \
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> \
	operator - (V v, const Yuni::Color::RGBA<T>& rhs) \
	{ \
		Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> r = rhs; \
		r -= v; \
		return r; \
	}


# define YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(V) \
	template<class T> \
	inline \
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> \
	operator + (const Yuni::Color::RGBA<T>& rhs, V v) \
	{ \
		Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> r = rhs; \
		r += v; \
		return r; \
	} \
	template<class T> \
	inline \
	Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> \
	operator - (const Yuni::Color::RGBA<T>& rhs, V v) \
	{ \
		Yuni::Color::RGBA<typename Yuni::Color::template RGBA<T>::template Calculation<V>::Type> r = rhs; \
		r -= v; \
		return r; \
	}


YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::sint8)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::sint16)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::sint32)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::sint64)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::uint8)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::uint16)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::uint32)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(Yuni::uint64)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(float)
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(double)
# ifdef YUNI_HAS_LONG_DOUBLE
YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA(long double)
# endif

YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::sint8)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::sint16)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::sint32)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::sint64)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::uint8)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::uint16)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::uint32)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(Yuni::uint64)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(float)
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(double)
# ifdef YUNI_HAS_LONG_DOUBLE
YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE(long double)
# endif

# undef YUNI_COLOR_RGBA_OPERATOR_TYPE_RGBA
# undef YUNI_COLOR_RGBA_OPERATOR_RGBA_TYPE

