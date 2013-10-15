#ifndef __YUNI_MESSAGING_THREAD_CONTEXT_HXX__
# define __YUNI_MESSAGING_THREAD_CONTEXT_HXX__


namespace Yuni
{
namespace Messaging
{

	inline void Context::clear()
	{
		text.clear();
		clob.clear();
	}


	template<uint MaxSize, class StringT>
	inline void Context::AutoShrink(StringT& variable)
	{
		variable.clear();
		if (variable.capacity() > MaxSize)
			variable.shrink();
	}


	inline void Context::autoshrink()
	{
		AutoShrink<6 * 1024>(text);
		AutoShrink<5 * 1024 * 1024>(clob);
		AutoShrink<1 * 1024 * 1024>(buffer);
	}




} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_THREAD_CONTEXT_H__
