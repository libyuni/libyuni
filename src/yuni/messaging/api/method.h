/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#ifndef __YUNI_MESSAGE_API_METHOD_H__
# define __YUNI_MESSAGE_API_METHOD_H__

# include "../../yuni.h"
# include "fwd.h"
# include "../../core/string.h"
# include "../../core/dictionary.h"
# include "../../core/bind.h"
# include "../message.h"
# include "../threadcontext.h"
# include "../../marshal/object.h"


namespace Yuni
{
namespace Messaging
{
namespace API
{

	class YUNI_DECL Method final
	{
	public:
		//! Hash table
		typedef Dictionary<String, Method>::Hash  Hash;

		/*!
		** \brief Callback when a method is invoked
		**
		** C-Style callbacks are used for performance reasons. std::function or
		** Yuni::Bind should not be used. Anyway, using thread-context related data
		** should be encouraged instead of shared data between all treads.
		**
		** The return value is an http status code, even for non http transports. As
		** a consequence, you should use standard status, like 200 (ok),
		** 400 (bad request, invalid parameters), 401 (not enough credentials),
		** 403 (no credentials and access denied), and 404 (not found).
		*/
		typedef void (*Callback) (Messaging::Context& /*context*/, Marshal::Object& /*response*/);

		class Parameter
		{
		public:
			typedef Dictionary<String,Parameter>::Hash  Hash;
		public:
			//! Default constructor
			Parameter();
			//! Name of the parameter
			String name;
			//! Brief
			String brief;
			//! Default value
			String defvalue;
			//! Flag to know if there a default value
			bool hasDefault;
		};

	public:
		//! \name Constructor & Destructor
		//@{
		//! Default constructor
		Method();
		//@}

		//! \name General informations about the method
		//@{
		/*!
		** \brief Get the name of the method
		*/
		const String& name() const;

		//! Get the brief
		const String& brief() const;
		//! Set the brief
		Method& brief(const AnyString& text);
		//@}

		//! \name Parameters
		//@{
		//! Add a parameter
		Method& param(const AnyString& name, const AnyString& brief);
		//! Add a parameter, with a default value
		Method& param(const AnyString& name, const AnyString& brief, const AnyString& defvalue);
		//! Get all parameters
		const Parameter::Hash& params() const;
		//@}

		//! \name Options
		//@{
		//! Set an option
		Method& option(const AnyString& key, const AnyString& value);
		const String& option(const AnyString& key) const;
		//@}

		//! \name Callback invoker
		//@{
		/*!
		** \brief Set the callback to invoke when the method is invocated
		**
		** The method will not be used by transports if a callback
		** is not provided.
		*/
		Method& invoke(Callback callback);
		//! Get the callback
		Callback invoke() const;
		//@}

		//! \name Memory
		//@{
		//! Reduce the memory consumption as much as possible
		void shrinkMemory();
		//@}


	private:
		//! Name of the method
		String pName;
		//! Brief
		String pBrief;
		//! Options
		mutable KeyValueStore pOptions;
		//! Parameters
		Parameter::Hash pParams;
		//! Callback to invoke
		Callback pCallback;

		//! Temporary string
		mutable String pTmp;
		// Our friend !
		friend class Methods;

	}; // class Method




} // namespace API
} // namespace Messaging
} // namespace Yuni

# include "method.hxx"

#endif // __YUNI_MESSAGE_API_METHOD_H__
