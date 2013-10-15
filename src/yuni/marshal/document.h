#ifndef __YUNI_MARSHAL_DOCUMENT_H__
# define __YUNI_MARSHAL_DOCUMENT_H__

# include "../yuni.h"
# include "object.h"


namespace Yuni
{
namespace Marshal
{

	/*!
	** \brief Document which may contain several objects
	*/
	class Document
	{
	public:
		Document();
		~Document();

	public:
		//! Root object
		Object root;

	}; // class Document




} // namespace Marshal
} // namespace Yuni

#endif // __YUNI_MARSHAL_DOCUMENT_H__
