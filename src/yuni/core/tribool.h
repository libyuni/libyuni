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
#include "../yuni.h"
#include "string.h"



namespace Yuni
{

	/*!
	** \brief 3-state boolean
	*/
	class Tribool final
	{
	public:
		//! \name Constructors
		//@{
		//! Default constructor (indeterminate by default)
		Tribool();
		//! default constructor, with a default when indeterminate
		Tribool(bool value, bool defvalue = false);
		//! default constructor to indeterminate
		Tribool(const NullPtr&, bool defvalue = false);
		//! copy constructor
		Tribool(const Tribool&);
		//@}


		/*!
		** \brief Set to indeterminate
		*/
		void clear();

		/*!
		** \brief Get if the value is indeterminate
		*/
		bool indeterminate() const;

		//! Get the default value, for bool conversion when indeterminate
		bool defaultValue() const;
		//! Set the default value, for bool conversion when indeterminate
		void defaultValue(bool defvalue);

		/*!
		** \brief Convert to a bool value, using the default value when indeterminate
		*/
		bool toBool() const;

		/*!
		** \brief Print
		*/
		template<class StreamT> void print(StreamT&) const;


		//! \name Operators
		//@{
		//! Assign from a bool value
		Tribool& operator = (bool value);
		//! Reset to 'indeterminate'
		Tribool& operator = (const NullPtr&);
		//! Copy from another tribool
		Tribool& operator = (const Tribool&);

		//! Get if equal to a bool value (see toBool())
		bool operator == (bool value) const;
		//! Get if indeterminate
		bool operator == (const NullPtr&) const;
		//! Get if strictly equal to another tribool
		bool operator == (const Tribool&) const;

		//! Get if equal to a bool value (see toBool())
		bool operator != (bool value) const;
		//! Get if indeterminate
		bool operator != (const NullPtr&) const;
		//! Get if strictly equal to another tribool
		bool operator != (const Tribool&) const;

		//! Get the bool representation of this tribool, see toBool()
		operator bool () const;
		//@}


	private:
		//! internal value (0: false, 1: true, -1: indeterminate)
		// The second value is the default one
		union TriboolValue
		{
			yint8 flags[2];
			uint u32;
		} pValue;

	}; // class Tribool





} // namespace Yuni

#include "tribool.hxx"
