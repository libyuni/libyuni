/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** github: https://github.com/libyuni/libyuni/
** gitlab: https://gitlab.com/libyuni/libyuni/ (mirror)
*/
#include <iostream>
#include <yuni/core/singleton/singleton.h>

class MyManager: public Yuni::Singleton<MyManager>
{
public:
	typedef Yuni::Singleton<MyManager> Singleton;

public:
	void doSomething()
	{
		std::cout << "Ping" << std::endl;
	}

	// In the simplest sample, the constructor is public
	// In more advanced samples, we prove that it can be made private for more safety,
	// if the correct friend declaration is added.
	MyManager() {}

private:
	// Hide the rest of the constructors and assignment operators
	MyManager(const MyManager&);
	MyManager& operator = (const MyManager&);
};


int main(void)
{
	MyManager& managerInstance = MyManager::Singleton::Instance();

	managerInstance.doSomething();
}
