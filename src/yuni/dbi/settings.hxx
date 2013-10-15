#ifndef __YUNI_DBI_SETTINGS_HXX__
# define __YUNI_DBI_SETTINGS_HXX__


namespace Yuni
{
namespace DBI
{

	inline Settings::Settings() :
		port(),
		maxReconnectionAttempts(Default::maxReconnectionAttempts),
		delayBetweenReconnectionAttempt(Default::delayBetweenReconnection),
		idleTime(Default::idleTime)
	{}


	inline void Settings::clear()
	{
		host.clear();
		username.clear();
		password.clear();
		database.clear();
		schema.clear();
		port = 0;

		maxReconnectionAttempts = Default::maxReconnectionAttempts;
		delayBetweenReconnectionAttempt = Default::delayBetweenReconnection;
		idleTime = Default::idleTime;
	}





} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_SETTINGS_H__
