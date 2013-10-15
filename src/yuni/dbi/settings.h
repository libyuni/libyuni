#ifndef __YUNI_DBI_SETTINGS_H__
# define __YUNI_DBI_SETTINGS_H__

# include "../yuni.h"
# include "../core/string.h"


namespace Yuni
{
namespace DBI
{

	class Default
	{
	public:
		enum
		{
			maxReconnectionAttempts = 30,
			delayBetweenReconnection = 1000, // ms
			idleTime = 60 // seconds
		};
	};


	class Settings
	{
	public:
		//! Default constructor
		Settings();

		//! Clear all settings
		void clear();


	public:
		//! host (empty means localhost) or filename
		String host;
		//! port (0 means default)
		uint port;
		//! username
		ShortString128 username;
		//! password
		String password;
		//! Database name
		ShortString128 database;
		//! schema to open (only if the database supports it)
		ShortString128 schema;

		//! Maximum number of Attempts of reconnection before declaring the connexion dead (-1 infinite)
		uint maxReconnectionAttempts;
		//! Delay (in milliseconds) to wait before each attempt
		uint delayBetweenReconnectionAttempt;

		//! Minimum time (in seconds) to wait before closing an idle connection
		uint idleTime;

	}; // class Settings





} // namespace DBI
} // namespace Yuni

# include "settings.hxx"

#endif // __YUNI_DBI_SETTINGS_H__
