#ifndef __YUNI_MESSAGING_MESSAGE_H__
# define __YUNI_MESSAGING_MESSAGE_H__

# include "../yuni.h"
# include "../core/dictionary.h"


namespace Yuni
{
namespace Messaging
{

	class Message final
	{
	public:
		Message() :
			httpStatus(200)
		{}

	public:
		// returned message
		Clob body;
		// HTTP Error
		uint httpStatus;
		// parameters
		KeyValueStore params;
		// Name of the method invoked
		AnyString method;
		// Schema
		AnyString schema;

		//! Temporary string provided for convenient uses (to reduce memory reallocation)
		String key;

	}; // class Message




} // namespace Messaging
} // namespace Yuni

#endif // __YUNI_MESSAGING_MESSAGE_H__
