#ifndef __YUNI_THREAD_SIGNAL_H__
# define __YUNI_THREAD_SIGNAL_H__

# include "../yuni.h"
# include "../core/smartptr.h"
# include "pthread.h"


namespace Yuni
{
namespace Thread
{

	/*!
	** \brief Mecanism for notifying a waiting thread of the occurrence of a particular event
	*/
	class YUNI_DECL Signal final
	{
	public:
		//! Most suitable smart pointer for the class
		typedef Yuni::SmartPtr<Signal> Ptr;


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		Signal();
		//! Copy constructor
		Signal(const Signal&);
		//! Destructor
		~Signal();
		//@}


		//! \name Signal
		//@{
		/*!
		** \brief Reset the internal state
		*/
		bool reset();

		/*!
		** \brief Wait for being notified
		*/
		void wait();

		/*!
		** \brief Wait for being notified
		**
		** \param timeout A timeout, in milliseconds
		** \return True if the signal has been notified, false if the timeout has been reached
		*/
		bool wait(uint timeout);

		/*!
		** \brief Notify the waiter
		*/
		bool notify();


		/*!
		** \brief Get if the signal is valid
		*/
		bool valid() const;
		//@}


		//! \name Operators
		//@{
		//! Operator !
		bool operator ! () const;
		//! Operator =
		Signal& operator = (const Signal&);
		//@}


	private:
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		//! Event handle (HANDLE)
		void* pHandle;
		# else
		pthread_mutex_t pMutex;
		pthread_cond_t  pCondition;
		volatile bool pSignalled;
		# endif
		#endif

	}; // class Signal





} // namespace Thread
} // namespace Yuni

# include "signal.hxx"

#endif // __YUNI_THREAD_SIGNAL_H__
