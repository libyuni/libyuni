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



namespace Yuni
{
namespace Math
{
namespace Random
{


	/*!
	** \brief Random Number Distribution (Abstract)
	**
	** This class is an abstract class for any random number generators.
	**
	** Do not forget that you have to reset the random number generator yourself,
	** As this operation may be costly, You may want to control it. The other
	** reason is that some specific parameters may be required.
	**
	** \tparam T The type of the random numbers
	** \param Derived The real class (CRTP)
	*/
	template<typename T, class Derived>
	class YUNI_DECL ADistribution
	{
	public:
		//! Type of a single random number
		typedef T Value;

	public:
		/*!
		** \brief Get the name of the distribution
		*/
		static const char* Name();

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		ADistribution();
		//! Destructor
		~ADistribution();
		//@}

		//! \name Random number Generator
		//@{
		/*!
		** \brief (Re)Iniitalize the random number generator
		*/
		void reset();

		/*!
		** \brief Generate the next random number
		*/
		const Value next();
		//@}


		//! \name Informations about the random number generator
		//@{
		//! Get the name of the distribution
		const char* name() const;

		//! The lower bound (including itself)
		const Value min() const;
		//! The Upper bound (but not including itself)
		const Value max() const;
		//@}

		//! \name Operators
		//@{
		//! \see next()
		const Value operator () ();
		//! \see next()
		template<class U> Derived& operator >> (U& u);
		//@}

	}; // class ADistribution





} // namespace Random
} // namespace Math
} // namespace Yuni

#include "distribution.hxx"
