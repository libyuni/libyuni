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
#include "connector-data.h"
#include "../../datetime/timestamp.h"



namespace Yuni
{
namespace Private
{
namespace DBI
{

	ConnectorData::~ConnectorData()
	{
		delete instance;
	}



	ChannelPtr ConnectorData::createNewChannelWL(uint64 threadid)
	{
		ChannelPtr newchan = new Channel(settings, adapter);
		channels[threadid] = newchan;
		return newchan;
	}


	uint ConnectorData::closeTooOldChannels(uint idleTime, uint& remainingCount)
	{
		// the current timestamp
		sint64 now = Yuni::DateTime::Now();
		// the number of channel which have been removed
		uint removedCount = 0;

		// avoid concurrent access
		Yuni::MutexLocker locker(mutex);

		// checking each channel
		Channel::Table::iterator it = channels.begin();
		while (it != channels.end())
		{
			// alias to the channel itself
			Channel& channel = *(it->second);

			// we will first check the idle time of the channel, before
			// locking its own mutex

			// beware : the variable `lastUsed` might be in the future in comparison
			// of our variable `now`, since it can be modified without locking
			// our variable `mutex`
			if (now >= channel.lastUsed)
			{
				// checking idle time
				if (idleTime <= (now - channel.lastUsed))
				{
					// our channel may be idle for too long
					// checking if it is not currently in use
					if (channel.mutex.trylock())
					{
						// it is safe to unlock here since no transaction can lock it
						// (our own mutex is already locked)
						channel.mutex.unlock();
						// removing the channel
						channels.erase(it++);
						// statistics
						++removedCount;
						// iterate
						continue;
					}
					else
					{
						// currently in use
						// this channel can not be closed
					}

				} // idle time check
			} // future check

			// next channel
			++it;
		}

		// the thread which would periodically check for idle channels might
		// be no longer necessary. This variable will be the indicator
		remainingCount = (uint) channels.size();

		// return the number of dead channels
		return removedCount;
	}





} // namespace DBI
} // namespace Private
} // namespace Yuni

