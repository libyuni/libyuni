#pragma once
#include <functional>


namespace Yuni
{

	/*!
	** \brief Hash combine, to create hash for complex struct
	**
	** \code
	** class Complex
	** {
	**		int a;
	**		std::string text;
	** };
	**
	** namespace std
	** {
	**		template<> struct hash<Complex>
	**		{
	**			inline size_t operator()(const Complex& a) const
	**			{
	**				size_t seed = 0;
	**				Yuni::HashComine(seed, name.x);
	**				Yuni::HashComine(seed, name.text);
	**				return seed;
	**			}
	**		};
	** }
	** \endcode
	** \note inspired from Boot's hash_combine
	*/
	template <class T>
	inline void HashCombine(std::size_t& seed, const T& value)
	{
		std::hash<T> hasher;
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}



} // namespace Yuni
