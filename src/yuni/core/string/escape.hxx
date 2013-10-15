#ifndef __YUNI_CORE_STRING_ESCAPE_HXX__
# define __YUNI_CORE_STRING_ESCAPE_HXX__



namespace Yuni
{


	template<class StringT>
	void AppendEscapedString(StringT& out, const AnyString& string, char quote)
	{
		if (not string.empty())
		{
			char escapequote[2] = {'\\', quote};
			uint offset = 0;
			do
			{
				uint pos = string.find(quote, offset);
				if (pos >= string.size())
				{
					out.append(string.c_str() + offset, string.size() - offset);
					break;
				}

				out.append(string.c_str() + offset, pos - offset);
				out.append(escapequote, 2);
				offset = pos + 1;
			}
			while (offset < string.size());
		}
		else
		{
			out += string;
		}
	}



} // namespace Yuni

#endif // __YUNI_CORE_STRING_ESCAPE_HXX__
