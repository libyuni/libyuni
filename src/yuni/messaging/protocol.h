#ifndef __YUNI_MESSAGING_PROTOCOL_H__
# define __YUNI_MESSAGING_PROTOCOL_H__

# include "api/fwd.h"
# include "api/methods.h"
# include "schema.h"


namespace Yuni
{
namespace Messaging
{

	class YUNI_DECL Protocol final
	{
	public:
		//! The most suitable smart pointer
		typedef SmartPtr<Protocol> Ptr;

	public:
		/*!
		** \brief Get the default schema
		*/
		Schema& schema();

		/*!
		** \brief Get a specific schema from its name
		*/
		Schema& schema(const AnyString& name);

		/*!
		** \brief All schemas
		*/
		const Schema::Hash& allSchemas() const;


		/*!
		** \brief Shrink the memory as much as possible
		*/
		void shrinkMemory();


	private:
		//! All schemas
		Schema::Hash pSchemas;
		//! Temporary string for container access
		mutable String pTmp;

	}; // class Protocol





} // namespace Messaging
} // namespace Yuni

# include "protocol.hxx"

#endif // __YUNI_MESSAGING_PROTOCOL_H__
