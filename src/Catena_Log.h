/*

Module:  Catena_Log.h

Function:
	McciCatena::cLog and associated functions.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	March 2017

*/

#ifndef _CATENA_LOG_H_		/* prevent multiple includes */
#define _CATENA_LOG_H_

#include <cstdint>

#pragma once

namespace McciCatena {

class cLog
	{
public:
	enum DebugFlags : uint32_t
		{
		kAlways		= 0u,
		kFatal		= 0xFFFFFFFFu,

                kBug            = 0x00000001u,
		kError		= 0x00000002u,
		kWarning	= 0x00000004u,
		kTrace		= 0x00000008u,
		kInfo		= 0x00000010u,
		};

	cLog(DebugFlags flags = DebugFlags(0))
                : m_uDebugFlags(flags)
                {};

	// neither copyable nor movable.
	cLog(const cLog&) = delete;
	cLog& operator=(const cLog&) = delete;
	cLog(const cLog&&) = delete;
	cLog& operator=(const cLog&&) = delete;

	// initialize
	bool begin(DebugFlags uDebugFlags = DebugFlags(kError | kBug))
		{
		this->m_uDebugFlags = uDebugFlags;
		}

	// old, incorrect polarity: return false if isEnabled() is true..
	// [[deprecated("use isEnabled(), check polarities; see issue #165")]]
	__attribute__((deprecated("use isEnabled(), check polarities; see issue #165")))
	bool isenabled(DebugFlags uDebugFlags) const
		{
		return !this->isEnabled(uDebugFlags);
		}

	// check whether flags are enabled ... inline for speed
	bool isEnabled(DebugFlags uDebugFlags) const
		{
		return ((uDebugFlags & this->m_uDebugFlags) != 0 ||
		         uDebugFlags == kAlways ||
			 uDebugFlags == kFatal);
		}

	// log, using debug flags. Note that args are evaluated even if no print.
	void printf(
		DebugFlags uDebugFlags,
		const char *pFmt,
		...
		) __attribute__((__format__(__printf__, 3, 4)));
		/* format counts start with 2 for non-static C++ member fns */

	// log, using debug flags and a template; you can write:
	//  gLog.cond(gLog.kTrace,
	//	[&](){gCatena.SafePrintf("message", arg1, arg2); })
	// and the debug code will only be called if trace is enabled.
	// But it may be nicer to write:
	// if (gLog.isEnabled(gLog.kTrace))
	//	gCatena.SafePrintf("message", arg1, arg2...);
	template <typename Functor>
	void cond(
		DebugFlags uDebugFlags,
		Functor &f
		)
		{
		if (this->isEnabled(uDebugFlags))
			f();
		}

	// fetch current log flags
	DebugFlags getFlags(void) const
		{
		return this->m_uDebugFlags;
		}

	// set log flags and return previous value
	DebugFlags setFlags(DebugFlags flags)
		{
		DebugFlags const oldFlags = this->m_uDebugFlags;

		this->m_uDebugFlags = flags;
		return oldFlags;
		}

private:
	DebugFlags m_uDebugFlags;
	};

extern cLog gLog;

}; // namespace McciCatena

/**** end of Catena_Log.h ****/
#endif /* _CATENA_LOG_H_ */
