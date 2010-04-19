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

#include "AudioEngine.h"
#include "MediaEngine.h"

void AudioPlay(void *cookie, void *buffer, size_t bufferSize, const media_raw_audio_format &format)
{

// TODO: AudioPlay

}

AudioEngine::AudioEngine(BMediaTrack *new_track, const char *name) {
	media_format	format;
	
	lock_count = 0;
	lock_sem = create_sem(0,"audio_engine ben");
	track = new_track;
	isPlaying = false;
	perfTime = -1;
	trackTime = 0;

	track->DecodedFormat(&format);
	switch (format.u.raw_audio.format) {
		case media_raw_audio_format::B_AUDIO_UCHAR :
			default_data = 0x80;
			frame_size = 1;
			break;
		case media_raw_audio_format::B_AUDIO_SHORT :
			default_data = 0;
			frame_size = 2;
			break;
		case media_raw_audio_format::B_AUDIO_INT :
			default_data = 0;
			frame_size = 4;
			break;
		case media_raw_audio_format::B_AUDIO_FLOAT :
			default_data = 0;
			frame_size = 4;
			break;
		default :
			player = NULL;
			return;
		}

	channelCount = format.u.raw_audio.channel_count;
	frame_size *= channelCount;
	buffer_size = format.u.raw_audio.buffer_size;
	frame_rate = format.u.raw_audio.frame_rate;

	player = new BSoundPlayer(&format.u.raw_audio, name, AudioPlay);
	if (player->InitCheck() != B_OK) {
		delete player;
		player = NULL;
	} else {
		player->SetCookie(this);
		player->Start();
		player->SetHasData(true);
	}
	
}

status_t AudioEngine::Play() {
	Lock();	// Lock our semaphores
	isPlaying = true;
	Unlock(); // Release our semaphores
	return B_NO_ERROR;
}

status_t AudioEngine::Stop() {
	isPlaying = false;
	return B_NO_ERROR;
}

void AudioEngine::Lock() {
	if (atomic_add(&lock_count, 1) > 0)
		acquire_sem(lock_sem);
}

void AudioEngine::Unlock() {
	if (atomic_add(&lock_count, -1) > 1)
		release_sem(lock_sem);
}
