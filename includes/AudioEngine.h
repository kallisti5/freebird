/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */
#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H


#include <Entry.h>
#include <SoundPlayer.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <MediaDefs.h>
#include <Path.h>
#include <malloc.h>


class AudioEngine {
	public:
				AudioEngine(BMediaTrack *track, const char *name);
		bool		IsPlaying() { return isPlaying; };

		status_t	InitCheck() { return ((player != NULL)?B_OK:B_ERROR); };
		status_t	Play();
		status_t	Stop();
		
		bigtime_t	TrackTimebase();
		
	private:
	friend	void		AudioPlay(void *, void *, size_t, const media_raw_audio_format &);
		bool		isPlaying;
		int32		frameSize;
		int32		channelCount;
		int8		default_data;
		uint32		frame_size;
		float		frame_rate;
		uint32		buffer_size;
		bigtime_t	perfTime, trackTime;

		BSoundPlayer	*player;
		BMediaTrack*	track;

		void		Lock();			// Semaphore locking
		void		Unlock();
		sem_id		lock_sem;
		int32		lock_count;

};

#endif
