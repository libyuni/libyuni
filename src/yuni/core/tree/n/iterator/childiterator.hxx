#ifndef __YUNI_CORE_TREE_N_CHILD_ITERATOR_HXX__
# define __YUNI_CORE_TREE_N_CHILD_ITERATOR_HXX__

namespace Yuni
{
namespace Private
{
namespace Core
{
namespace Tree
{


	template<class NodeT>
	inline ChildIterator<NodeT>::ChildIterator():
		pNode(nullptr)
	{}


	template<class NodeT>
	inline ChildIterator<NodeT>::ChildIterator(const Type& it):
		pNode(it.pNode)
	{}


	template<class NodeT>
	template<class N>
	inline ChildIterator<NodeT>::ChildIterator(const ChildIterator<N>& p):
		pNode(p.pNode)
	{}


	template<class NodeT>
	inline ChildIterator<NodeT>::ChildIterator(const NodePtr& p):
		pNode(p)
	{}


	template<class NodeT>
	inline typename ChildIterator<NodeT>::reference
	ChildIterator<NodeT>::operator * ()
	{
		return *pNode;
	}


	template<class NodeT>
	inline typename ChildIterator<NodeT>::const_reference
	ChildIterator<NodeT>::operator * () const
	{
		return *pNode;
	}


	template<class NodeT>
	inline typename ChildIterator<NodeT>::pointer
	ChildIterator<NodeT>::operator -> ()
	{
		return pNode.pointer();
	}


	template<class NodeT>
	inline typename ChildIterator<NodeT>::const_pointer
	ChildIterator<NodeT>::operator -> () const
	{
		return pNode.pointer();
	}


	template<class NodeT>
	template<class N>
	inline bool ChildIterator<NodeT>::equals(const ChildIterator<N>& rhs) const
	{
		return pNode == rhs.pNode;
	}


	template<class NodeT>
	template<class N>
	inline void ChildIterator<NodeT>::reset(const ChildIterator<N>& rhs)
	{
		pNode = rhs.pNode;
	}


	template<class NodeT>
	void ChildIterator<NodeT>::forward()
	{
		pNode = pNode->nextSibling();
	}


	template<class NodeT>
	void ChildIterator<NodeT>::forward(difference_type n)
	{
		while (n--)
		{
			forward();
		}
	}


	template<class NodeT>
	void ChildIterator<NodeT>::backward()
	{
		pNode = pNode->previousSibling();
	}


	template<class NodeT>
	void ChildIterator<NodeT>::backward(difference_type n)
	{
		while (n--)
		{
			backward();
		}
	}



} // namespace Tree
} // namespace Core
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_TREE_N_CHILD_ITERATOR_HXX__
