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
#pragma once
#include "process-info.h"


namespace Yuni
{
namespace Process
{


	inline Program::ProcessSharedInfo::ProcessSharedInfo()
		: running(false)
		, processID(-1)
		, processInput(-1)
		, thread(nullptr)
		, duration(0)
		, durationPrecision(dpSeconds)
		, timeout()
		, exitstatus(-1)
		, redirectToConsole(false)
		, timeoutThread(nullptr)
	{}




} // namespace Process
} // namespace Yuni
