#pragma once



namespace Yuni
{

	/*!
	** \brief Prevent objects of a class from being copy-constructed or assigned to each other
	**
	** \code
	** class ClassThatCanNotBeCopied : private NonCopyable<ClassThatCanNotBeCopied>
	** {
	** // ...
	** };
	** \endcode
	*/
	template<class T>
	class YUNI_DECL NonCopyable
	{
	protected:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		NonCopyable() {}
		//! Protected non-virtual destructor
		~NonCopyable() {}
		//@}


	private:
		#if defined(YUNI_HAS_CPP_MOVE)
		// no copy constructor
		NonCopyable(const NonCopyable&) = delete;
		// no copy operator
		template<class U> NonCopyable& operator = (const U&) = delete;
		NonCopyable& operator = (const NonCopyable&) = delete;
		#else
		// Private copy constructor
		NonCopyable(const NonCopyable &) {}
		// Private copy operator
		template<class U> T& operator = (const U&) {return *static_cast<T*>(this);}
		NonCopyable& operator = (const NonCopyable&) {return *static_cast<T*>(this);}
		#endif

	}; // class NonCopyable






} // namespace Yuni
