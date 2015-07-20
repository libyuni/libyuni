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
#include <yuni/yuni.h>
#include <yuni/messaging/service.h>
#include <yuni/messaging/transport/rest/server.h>
#include <yuni/core/logs.h>

using namespace Yuni;

//! Logging facility
Logs::Logger<> logs;



static void APIHelloWorld(Messaging::Context&, Marshal::Object& response)
{
	// log server
	logs.info() << "[method:hello_world] sending hello world !";
	// the response
	response["message"] = "Hellow world !";
}


static void APISum(Messaging::Context& context, Marshal::Object& response)
{
	// our parameters
	double a, b;

	if (not context.params["a"].to(a) or not context.params["b"].to(b))
	{
		// the parameters are not valid, exiting with a 400 status code
		// (Bad Request, The request cannot be fulfilled due to bad syntax.)
		// see http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
		context.httpStatus = 400;
		// log server
		logs.error() << "[method:sum] invalid parameters";
		return;
	}

	// log server
	logs.info() << "[method:sum] " << a << " + " << b << " = " << (a + b);
	// the response
	response["a"] = a;
	response["b"] = b;
	response["sum"] = (a + b);
}




static void PrepareTheAPI(Messaging::Service& service)
{
	logs.debug() << "preparing protocol";
	// note: contrary to the object `service`, `myapi` is not thread-safe
	Messaging::Protocol* myapi = new Messaging::Protocol();
	// retrieving the default schema
	Messaging::Schema& schema = myapi->schema();

	// method: hello_world
	schema.methods.add("hello_world")
		.brief("Always answer 'hello world to any request'")
		.option("http.method", "GET")
		.invoke(& APIHelloWorld);

	// method: sum
	schema.methods.add("sum")
		.brief("Computes the sum of two numbers")
		.option("http.method", "GET")
		.param("a", "First operand", "0")
		.param("b", "Second operand", "0")
		.invoke(& APISum);

	// -- Switching to the new protocol
	// note: The method `protocol()` must be call to make any modification to
	//   the protocol visible, and can be called anytime, even if started
	//   (useful for reloading the service while running)
	// note: this method will take ownership of the pointer, thus the pointer
	//   must not be used after this statement
	service.protocol(myapi);
}


static void PrepareTransports(Messaging::Service& service)
{
	logs.debug() << "preparing transports";
	service.transports.add("*", 6042, new Messaging::Transport::REST::Server());
}


static bool StartService(Messaging::Service& service)
{
	logs.info() << "starting";
	Net::Error error = service.start();

	switch (error)
	{
		case Net::errNone:
			break;
		default:
			logs.fatal() << "impossible to start the service: " << error;
			return false;
	}

	logs.info() << "examples:";
	logs.info() << "# curl -i -X GET 'http://localhost:6042/hello_world'";
	logs.info() << "# curl -i -X GET 'http://localhost:6042/sum?a=50&b=42'";
	logs.info() << "<ctrl> + C to quit";
	logs.info(); // empty line for beauty
	return true;
}





int main(int, char**)
{
	// Welcome !
	logs.notice() << "Yuni REST Server Example";
	logs.info() << "http://www.libyuni.org";
	logs.info(); // empty line

	// The service itself
	Messaging::Service  service;

	// Prepare the API
	PrepareTheAPI(service);
	// Preparing all transports
	PrepareTransports(service);

	// Starting the new service
	if (not StartService(service))
		return EXIT_FAILURE;

	// Waiting for the end of the server
	service.wait();

	return 0;
}

