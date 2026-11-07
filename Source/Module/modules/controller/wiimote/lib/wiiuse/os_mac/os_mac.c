

/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */


#pragma warning(disable:4206)

#if defined __APPLE__

//#include "os.h"

#include <mach/mach_time.h>
#include <stdint.h>

unsigned long wiiuse_os_ticks(void) {
    mach_timebase_info_data_t scale;
    mach_timebase_info(&scale);
    // Monotonic time keeps Bluetooth timeouts valid across clock changes.
    uint64_t ticks = mach_absolute_time();
    return (unsigned long)((ticks / 1000000) * scale.numer / scale.denom);
}
#endif
