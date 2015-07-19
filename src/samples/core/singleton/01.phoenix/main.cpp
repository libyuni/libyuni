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
#include <iostream>
#include <yuni/core/singleton/singleton.h>

//
// A log class for logging errors or information
// It is defined as a phoenix singleton
//
class Log: Yuni::Singleton<Log, Yuni::Policy::Creation::EmptyConstructor, Yuni::Policy::Lifetime::Phoenix>
{
public:
	typedef Yuni::Singleton<Log, Yuni::Policy::Creation::EmptyConstructor, Yuni::Policy::Lifetime::Phoenix> Singleton;

public:
	void print(std::ostream& out, const char* message)
	{
		out << "Log : ";
		out << message;
	}


protected:
	Log()
	{
		std::cout << "Log created." << std::endl;
	}
	~Log()
	{
		std::cout << "Log destroyed." << std::endl;
	}

	// Hide the rest of the constructors and assignment operators
	Log(const Log&);
	Log& operator = (const Log&);

	friend class Yuni::Policy::Creation::EmptyConstructor<Log>;
};


//
// Keyboard writes to the log on creation and destruction
//
class Keyboard : public Yuni::Singleton<Keyboard>
{
public:
	typedef Yuni::Singleton<Keyboard> Singleton;

protected:
	Keyboard()
	{
		std::cout << "Keyboard created." << std::endl;
		Log::Singleton::Instance().print(std::cout, "Keyboard creation.\n");
	}
	~Keyboard()
	{
		Log::Singleton::Instance().print(std::cout, "Keyboard destruction.\n");
		std::cout << "Keyboard destroyed." << std::endl;
	}

	// Hide the rest of the constructors and assignment operators
	Keyboard(const Keyboard&);
	Keyboard& operator = (const Keyboard&);

	friend class Yuni::Policy::Creation::EmptyConstructor<Keyboard>;
};


//
// Display only writes to the log on destruction
//
class Display: public Yuni::Singleton<Display>
{
public:
	typedef Yuni::Singleton<Display> Singleton;

protected:
	Display()
	{
		std::cout << "Display created." << std::endl;
	}
	~Display()
	{
		Log::Singleton::Instance().print(std::cout, "Display destruction.\n");
		std::cout << "Display destroyed." << std::endl;
	}

private:
	// Hide the rest of the constructors and assignment operators
	Display(const Display&);
	Display& operator = (const Display&);

	friend class Yuni::Policy::Creation::EmptyConstructor<Display>;
};


// What happens in this sample is that the Display and Keyboard both use the Log
int main(void)
{
	// On creation of Display, Log will also be created in order to log information
	Display& display = Display::Singleton::Instance();
	// Keyboard will use the already existing Log
	Keyboard& keyboard = Keyboard::Singleton::Instance();

	(void) display; // remove warning
	(void) keyboard;
	return 0;
}
// And all the fun stuff happens now :
// Keyboard was created last and so it is destroyed first. No problem there.
// Then Log is (more or less) destroyed.
// When finally Display is destroyed, Log is reborn from its ashes to serve its needs.
// And in the end, Log is destroyed again and all is well with the world.

