#ifndef __YUNI_CORE_TRIBOOL_H__
# define __YUNI_CORE_TRIBOOL_H__

# include "../yuni.h"
# include "string.h"



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
		//! Default constructor
		Tribool();
		//! default constructor
		Tribool(bool value);
		//! default constructor to indeterminate
		Tribool(const NullPtr&);
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

		/*!
		** \brief Print
		*/
		template<class StreamT> void print(StreamT&) const;

		//! \name Operators
		//@{
		Tribool& operator = (bool value);
		Tribool& operator = (const NullPtr&);
		Tribool& operator = (const Tribool&);

		bool operator == (bool value) const;
		bool operator == (const NullPtr&) const;
		bool operator == (const Tribool&) const;

		operator bool () const;
		//@}


	private:
		//! internal value
		yint8 pValue;
	};





} // namespace Yuni

# include "tribool.hxx"

#endif // __YUNI_CORE_TRIBOOL_H__
