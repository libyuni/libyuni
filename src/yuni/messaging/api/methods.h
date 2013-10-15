#ifndef __YUNI_MESSAGING_API_METHODS_H__
# define __YUNI_MESSAGING_API_METHODS_H__

# include "../../yuni.h"
# include "fwd.h"
# include "method.h"


namespace Yuni
{
namespace Messaging
{
namespace API
{

	//! All methods
	class Methods final
	{
	public:
		/*!
		 ** \brief Add a new method
		 */
		Method& add(const AnyString& name);

		/*!
		** \brief Remove all methods
		*/
		void clear();

		//! All methods
		const Method::Hash& all() const;

		//! Reduce the memory consumption as much as possible
		void shrinkMemory();

	private:
		//! methods
		Method::Hash  pMethods;

	}; // class Methods





} // namespace API
} // namespace Messaging
} // namespace Yuni

# include "methods.hxx"

#endif // __YUNI_MESSAGING_API_METHODS_H__
