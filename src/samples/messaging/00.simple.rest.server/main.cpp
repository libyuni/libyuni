
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

