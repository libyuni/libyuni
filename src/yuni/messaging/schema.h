#ifndef __YUNI_MESSAGING_API_SCHEMA_H__
# define __YUNI_MESSAGING_API_SCHEMA_H__

# include "../yuni.h"
# include "../core/dictionary.h"
# include "api/methods.h"
# include "api/method.h"


namespace Yuni
{
namespace Messaging
{

	/*!
	** \brief Schema for API methods
	**
	** A schema is some kink of namespace, where methods will belong
	*/
	class YUNI_DECL Schema final
	{
	public:
		//! The most suitable smart pointer
		typedef SmartPtr<Protocol> Ptr;
		//! Container for schemas
		typedef Dictionary<String, Schema>::Hash  Hash;


	public:
		//! Reduce the memory consumption as much as possible
		void shrinkMemory();

	public:
		//! All methods
		API::Methods methods;

		//! Default settings inherited by real method parameters
		class Defaults final
		{
		public:
			//! Add a default configuration for a single parameter
			Defaults& param(const AnyString& name, const AnyString& brief);
			//! Add a default condifuration for a single parameter, with a default value
			Defaults& param(const AnyString& name, const AnyString& brief, const AnyString& defvalue);
			//! Get all default configurations for parameters
			const API::Method::Parameter::Hash& params() const;

			//! Reduce the memory consumption as much as possible
			void shrinkMemory();

		private:
			//! Parameters
			API::Method::Parameter::Hash pParams;
			//! Temporary string
			String pTmp;
		}
		defaults;

	}; // class Schema




} // namespace Messaging
} // namespace Yuni

# include "schema.hxx"

#endif // __YUNI_MESSAGING_API_SCHEMA_H__
