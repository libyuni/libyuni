#ifndef __YUNI_CORE_STRING_ITERATOR_HXX__
# define __YUNI_CORE_STRING_ITERATOR_HXX__


namespace Yuni
{

	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::utf8iterator
	CString<ChunkSizeT,ExpandableT>::utf8begin(uint offset)
	{
		return utf8iterator(*this, offset);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::const_utf8iterator
	CString<ChunkSizeT,ExpandableT>::utf8begin(uint offset) const
	{
		return const_utf8iterator(*this, offset);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::null_iterator
	CString<ChunkSizeT,ExpandableT>::utf8end()
	{
		return null_iterator(*this);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::null_iterator
	CString<ChunkSizeT,ExpandableT>::utf8end() const
	{
		return null_iterator(*this);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::null_iterator
	CString<ChunkSizeT,ExpandableT>::end()
	{
		return null_iterator(*this);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::null_iterator
	CString<ChunkSizeT,ExpandableT>::end() const
	{
		return null_iterator(*this);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::iterator
	CString<ChunkSizeT,ExpandableT>::begin()
	{
		return iterator(*this, 0u);
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline typename CString<ChunkSizeT,ExpandableT>::const_iterator
	CString<ChunkSizeT,ExpandableT>::begin() const
	{
		return const_iterator(*this, 0u);
	}




} // namespace Yuni

#endif // __YUNI_CORE_STRING_ITERATOR_HXX__
