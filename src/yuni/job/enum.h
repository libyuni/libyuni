/*
** This file is part of libyuni, a cross-platform C++ framework (http://libyuni.org).
**
** This Source Code Form is subject to the terms of the Mozilla Public License
** v.2.0. If a copy of the MPL was not distributed with this file, You can
** obtain one at http://mozilla.org/MPL/2.0/.
**
** gitlab: https://gitlab.com/libyuni/libyuni/
** github: https://github.com/libyuni/libyuni/ {mirror}
*/
#pragma once



namespace Yuni
{
namespace Job
{

	/*!
	** \brief Set of possible states for a single job
	**
	** \ingroup Jobs
	*/
	enum class State
	{
		//! The job does nothing (can be already done)
		idle = 0,
		//! The job is waiting for being executed
		waiting,
		//! The job is currently running
		running,
	};



	enum Priority
	{
		//! Low priority
		low = 0,
		//! Default priority
		normal = 1,
		//! High priority
		high = 2,
	};

	constexpr const uint priorityCount = 3;



} // namespace Job
} // namespace Yuni
