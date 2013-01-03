/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
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

#include "AudioEngine.h"


class MediaEngine {
	public:
		status_t	SetSource(const char *path);

	private:
		static int32	MediaPlayer(void *arg);
		status_t	SetAudioTrack(const char *path, BMediaTrack *track, media_format *format);
		void		Reset();
		AudioEngine*	fAudioEngine;
		BMediaFile*	fMediaFile;
		BMediaTrack*	fAudioTrack;
		void*		fAudioDumpingBuffer;
		thread_id	fPlayerThread;
		bigtime_t	fCurTime;
		bigtime_t	fScrubTime;

		bool		fSnoozing;
		bool		fPlaying;

		sem_id		fPlaySem;
		sem_id		fScrubSem;
};


#endif
