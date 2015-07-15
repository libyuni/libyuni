#pragma once
#include "../yuni.h"
#include "object.h"



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
