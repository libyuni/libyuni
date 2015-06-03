#pragma once
#include "../../yuni.h"



/*!
** \brief Get if the Nth bit is set in a raw char* buffer
** \ingroup Bit
*/
# define YUNI_BIT_GET(DATA,INDEX)   \
	(((const unsigned char*)(DATA))[(INDEX) >> 3] & (1 << (7 - ((INDEX) & 7))))

/*!
** \brief Set the Nth bit in a raw char* buffer
** \ingroup Bit
*/
# define YUNI_BIT_SET(DATA,INDEX)  \
	(((unsigned char*)(DATA))[(INDEX) >> 3] |= (unsigned char)(1 << (7 - ((INDEX) & 7))))

/*!
** \brief Unset the Nth bit in a raw char* buffer
** \ingroup Bit
*/
# define YUNI_BIT_UNSET(DATA,INDEX)  \
	(((unsigned char*)(DATA))[(INDEX) >> 3] &= (unsigned char)~(1 << (7 - ((INDEX) & 7))))




namespace Yuni
{
/*!
** \brief Bit manipulation
** \ingroup Bit
*/
namespace Bit
{

	/*!
	** \brief Get the number of bits that are set
	** \ingroup Bit
	*/
	template<class T> uint Count(T data);
	yuint32 Count(yuint32 data);

	/*!
	** \brief Get if the bit at a given index
	** \ingroup Bit
	*/
	bool Get(const char*, uint index);

	/*!
	** \brief Set the Nth bit in a raw buffer
	** \ingroup Bit
	*/
	void Set(char* data, uint index);

	/*!
	** \brief Unset the Nth bit in a raw buffer
	** \ingroup Bit
	*/
	void Unset(char* data, uint index);





} // namespace Bit
} // namespace Yuni

#include "bit.hxx"

