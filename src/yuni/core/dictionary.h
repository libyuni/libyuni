#ifndef __YUNI_CORE_DICTIONARY_H__
# define __YUNI_CORE_DICTIONARY_H__

# include "../yuni.h"
# include "string.h"
# include <map>
# include <set>
# ifdef YUNI_HAS_STL_UNORDERED_MAP
#	include <unordered_map>
# endif
# ifdef YUNI_HAS_STL_UNORDERED_SET
#	include <unordered_set>
# endif



namespace Yuni
{

	template<class KeyT, class ValueT>
	class Dictionary final
	{
	public:
		# ifndef YUNI_HAS_STL_UNORDERED_MAP
		//! Un unordered dictionary table
		typedef std::map<KeyT, ValueT> Hash;
		//! Un unordered dictionary table
		typedef std::map<KeyT, ValueT> Unordered;
		# else
		//! Un unordered dictionary table
		typedef std::unordered_map<KeyT, ValueT> Hash;
		//! Un unordered dictionary table
		typedef std::unordered_map<KeyT, ValueT> Unordered;
		# endif

		//! Un ordered dictionary table
		typedef std::map<KeyT, ValueT> Map;
		//! Un ordered dictionary table
		typedef std::map<KeyT, ValueT> Ordered;

	}; // struct Dictionary



	template<class KeyT>
	class Set final
	{
	public:
		# ifndef YUNI_HAS_STL_UNORDERED_MAP
		//! Un unordered set table
		typedef std::set<KeyT> Hash;
		//! Un unordered set table
		typedef std::set<KeyT> Unordered;
		# else
		//! Un unordered set table
		typedef std::unordered_set<KeyT> Hash;
		//! Un unordered set table
		typedef std::unordered_set<KeyT> Unordered;
		# endif

		//! Un ordered dictionary table
		typedef std::set<KeyT> Ordered;

	}; // struct Set



	//! Convenient typedef for a key/value store
	typedef Dictionary<String, String>::Hash  KeyValueStore;


} // namespace Yuni

#endif // __YUNI_CORE_DICTIONARY_H__
