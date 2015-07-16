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
#include "../../yuni.h"
#include "fwd.h"

#ifdef RGB
# undef RGB
#endif // RGB



namespace Yuni
{
/*!
** \brief Color models
** \ingroup Color
*/
namespace Color
{

	/*!
	** \brief 32Bits RGB Color Model (additive color model + transparency)
	** \ingroup Color
	*/
	template<class T = uint8>
	class YUNI_DECL RGB final
	{
	public:
		//! Type used for channels
		typedef T Type;
		//! The most appropriate type for printing
		typedef typename Yuni::Private::Color::Print<T>::Type  PrintType;
		//! The most appropriate type for calculations
		template<class V>
		struct Calculation
		{
			typedef typename Yuni::Private::Color::Calculation<T, V>::Type  Type;
		};
		enum
		{
			//! A non-zero value if the class has a transparency channel
			hasTransparency = 0,
			//! The upper bound for this color model
			upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
			//! The lower bound for this color model
			lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
		};

	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Default constructor
		*/
		RGB();
		/*!
		** \brief Copy constructor
		*/
		RGB(const RGB& rhs);
		/*!
		** \brief Constructor with default values
		*/
		template<class R, class G, class B, class A> RGB(R r, G g, B b, A a);
		/*!
		** \brief Constructor with default values
		*/
		template<class R, class G, class B> RGB(R r, G g, B b);
		/*!
		** \brief Constructor with default values
		*/
		template<class U> RGB(const U& value);
		//@}


		//! \name Assign
		//@{
		/*!
		** \brief Assign all channels at once
		*/
		template<class R, class G, class B, class A> void assign(R r, G g, B b, A a);
		/*!
		** \brief Reset all channels at once
		**
		** The alpha channel will have a default value (opaque)
		*/
		template<class R, class G, class B> void assign(R r, G g, B b);
		/*!
		** \brief Reset all channels at once from a single value
		*/
		template<class U> void assign(const U& value);
		//@}


		//! \name Misc
		//@{
		/*!
		** \brief Get if the color will be visible
		**
		** This method is equivalent to manually check the alpha channel
		** with the lower bound
		*/
		bool visible() const;

		/*!
		** \brief Check if all channels are valid
		**
		** This method is only useful when the channels are modified manually.
		*/
		bool valid() const;
		//@}


		//! \name Stream printing
		//@{
		//! Print the color value
		template<class StreamT> void print(StreamT& out) const;
		//@}


		//! \name Operators
		//@{
		//! Increase or decrease all channels
		template<class U> RGB& operator += (const U& value);
		//! Increase or decrease all channels
		template<class U> RGB& operator -= (const U& value);
		//! Assignment
		RGB& operator = (const RGB& rhs);
		//! Assignment
		RGB& operator = (const Yuni::NullPtr&);
		//! Assignment
		template<class U> RGB& operator = (const U& rhs);
		//! Comparison
		template<class U> bool operator == (const U& rhs) const;
		//! Comparison
		template<class U> bool operator != (const U& rhs) const;
		//! Index access
		T& operator [] (const uint i);
		//@}


	public:
		//! The red channel
		Type red;
		//! The green channel
		Type green;
		//! The blue channel
		Type blue;

	}; // class RGB





} // namespace Color
} // namespace Yuni

# include "rgb.hxx"

