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
#ifndef __YUNI_CORE_MATH_RANDOM_TABLE_H__
# define __YUNI_CORE_MATH_RANDOM_TABLE_H__

# include "../math.h"
# include "../../../thread/policy.h"
# include "../../static/assert.h"
# include "../../static/if.h"
# include "../../atomic/int.h"


namespace Yuni
{
namespace Math
{
namespace Random
{


	/*!
	** \brief Pre-Cached table of random numbers
	**
	** This class behaves like any other random number generator except random numbers
	** come from a prefetch cache.
	**
	** \code
	** #include <iostream>
	** #include <yuni/core/math/random/default.h>
	** #include <yuni/core/math/random/table.h>
	**
	** using namespace Yuni;
	**
	** int main()
	** {
	**		// A table for pre-cached random numbers
	**		Math::Random::Table<Math::Random::Default>  randomTable;
	**		// Generating a set pre-cached random numbers
	**		std::cout << "Generating " << randomTable.size() << " random numbers..." << std::endl;
	**		randomTable.reset();
	**
	**		std::cout << "A random number : " << randomTable() << std::endl;
	**		std::cout << "Another one     : " << randomTable() << std::endl;
	**		return 0;
	** }
	** \endcode
	**
	** Do not forget that you have to reset the random number generator yourself,
	** As this operation may be costly, You may want to control it. The other
	** reason is that some specific parameters may be required
	**
	** Even if it is possible to make this class thread-safe (using the godd threading
	** policy, it is no really advised.  The real purpose of this class
	** is to avoid the high cost of a Random number generation (in a real-time
	** game for example) and additional locks might be costly as well.
	** In most cases you would prefer a `table` per thread.
	**
	** \tparam D Any random number generator
	** \tparam TableSize The count of random numbers to cache
	** \tparam Cyclic True to Cycle through the list or False to regenerate a new set
	** when all pre-cached random numbers are exhausted
	** \tparam TP The Threading Policy
	*/
	template<class D,                                         // The distribution
		int TableSize = 200000,                               // Number of pre-cached numbers
		bool Cyclic = true,                                   // Cycle through the list or regenerate a new set ?
		template<class> class TP = Policy::SingleThreaded     // The Threading policy
		>
	class YUNI_DECL Table
		: public TP<Table<D,TableSize, Cyclic, TP> >
		, public D
	{
	public:
		//! Type for a single random number
		typedef typename D::Value Value;
		//! The threading policy
		typedef TP<Table<D,TableSize, Cyclic, TP> > ThreadingPolicy;

		// Assert about the table size
		YUNI_STATIC_ASSERT((TableSize > 10), MathRandomTable_InvalidTableSize);
		enum
		{
			//! The table size
			tableSize = TableSize,
		};

	public:
		/*!
		** \brief Name of the distribution
		**
		** This name is given by the template parameter D.
		*/
		static const char* Name();

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		**
		** The table will be filled with random numbers provided by the
		** given distribution.
		*/
		Table();
		//! Destructor
		~Table();
		//@}

		/*!
		** \brief Reset the random number generator
		**
		** The method `fill()` will be implicity called to generate a new set
		** of random numbers.
		*/
		void reset();

		//! Get the number of random number in the table
		static int size();

		//! Get the next random number
		const Value next();

		//! Fill the table with a new set of random numbers
		void fill();

		//! The operator `>>`
		template<class U> Table& operator >> (U& u);

		//! The operator `()`
		const Value operator () ();


	private:
		void fillWL();

	private:
		//! Offset
		typedef typename Static::If<(!ThreadingPolicy::threadSafe), Atomic::Int<>, int>::Type  OffsetType;
		//! Position in the cache
		OffsetType pOffset;
		//! The cache
		typename D::Value pCache[TableSize];

	}; // class Table






} // namespace Random
} // namespace Math
} // namespace Yuni

# include "table.hxx"

#endif // __YUNI_CORE_MATH_RANDOM_TABLE_H__
