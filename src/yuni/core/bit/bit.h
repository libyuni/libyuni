#ifndef __YUNI_CORE_BIT_BIT_H__
# define __YUNI_CORE_BIT_BIT_H__

# include "../../yuni.h"

/*!
** \brief Get if the Nth bit is set in a raw char* buffer
*/
# define YUNI_BIT_GET(DATA,INDEX)   \
	(((const unsigned char*)(DATA))[(INDEX) >> 3] & (1 << (7 - ((INDEX) & 7))))

/*!
** \brief Set the Nth bit in a raw char* buffer
*/
# define YUNI_BIT_SET(DATA,INDEX)  \
	(((unsigned char*)(DATA))[(INDEX) >> 3] |= (unsigned char)(1 << (7 - ((INDEX) & 7))))

/*!
** \brief Unset the Nth bit in a raw char* buffer
*/
# define YUNI_BIT_UNSET(DATA,INDEX)  \
	(((unsigned char*)(DATA))[(INDEX) >> 3] &= (unsigned char)~(1 << (7 - ((INDEX) & 7))))




namespace Yuni
{
namespace Bit
{

	/*!
	** \brief Get the number of bits that are set
	*/
	template<class T> uint Count(T data);

	/*!
	** \brief Get if the bit at a given index
	*/
	bool Get(const char*, uint index);

	/*!
	** \brief Set the Nth bit in a raw buffer
	*/
	void Set(char* data, uint index);

	/*!
	** \brief Unset the Nth bit in a raw buffer
	*/
	void Unset(char* data, uint index);




} // namespace Bit
} // namespace Yuni

# include "bit.hxx"

#endif // __YUNI_CORE_BIT_BIT_H__
