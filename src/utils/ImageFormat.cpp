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

#include <map>

#include "ImageFormat.h"

std::string Utils::ImageFormat::MimeType(const std::string &format)
{
	// Format names are file extensions, and "image/" + name is not a MIME type for most of
	// them ("image/jpg"). Names from QImageWriter and its usual plugins; anything else keeps
	// "image/" + name. A fixed table, not QMimeDatabase: that one depends on the system's
	// shared-mime-info and answers application/octet-stream without it.
	static const std::map<std::string, std::string> mimeTypes = {
		{"jpg", "image/jpeg"},
		{"jpeg", "image/jpeg"},
		{"jfif", "image/jpeg"},
		{"ico", "image/vnd.microsoft.icon"},
		{"cur", "image/x-win-bitmap"},
		{"pbm", "image/x-portable-bitmap"},
		{"pgm", "image/x-portable-graymap"},
		{"ppm", "image/x-portable-pixmap"},
		{"xbm", "image/x-xbitmap"},
		{"xpm", "image/x-xpixmap"},
		{"tif", "image/tiff"},
		{"tiff", "image/tiff"},
		{"svg", "image/svg+xml"},
	};

	auto it = mimeTypes.find(format);
	return it != mimeTypes.end() ? it->second : "image/" + format;
}
