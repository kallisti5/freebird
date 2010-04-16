/*
 * Freebird, the Music Jukebox for Haiku
 * 2010, Alexander von Gluck
 * http://unixzen.com, http://github.com/kallisti5
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MEDIA_ENGINE_H
#define _MEDIA_ENGINE_H

#include <Entry.h>
#include <SoundPlayer.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <MediaDefs.h>
#include <Path.h>
#include <malloc.h>

class MediaEngine {
	public:
		status_t	SetSource(const char *path);

	private:
		status_t	SetAudioTrack(const char *path, BMediaTrack *track, media_format *format);

	private:
		BMediaFile*	fMediaFile;
		BMediaTrack*	fAudioTrack;
		//AudioOutput*	fAudioOutput;
}

#endif
