/*
obs-websocket
Copyright (C) 2016-2021 Stephane Lepin <stephane.lepin@gmail.com>
Copyright (C) 2020-2021 Kyle Manning <tt2468@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#pragma once

#include <QString>

// The Session Duration column of the settings dialog's session table:
// hh:mm:ss, with the hours counting on past 24. Not QTime: QTime(0, 0, s)
// is invalid from 60 seconds on, and QTime::addSecs() wraps at 24 hours.
static inline QString FormatSessionDuration(qint64 seconds)
{
	if (seconds < 0)
		seconds = 0;
	const QChar zero('0');
	return QString("%1:%2:%3")
		.arg(seconds / 3600, 2, 10, zero)
		.arg(seconds / 60 % 60, 2, 10, zero)
		.arg(seconds % 60, 2, 10, zero);
}
