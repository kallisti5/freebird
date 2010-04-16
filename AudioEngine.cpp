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
