/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */
 

#include "MediaEngine.h"

#include <stdio.h>


#define DEBUG_MEDIAENGINE
#ifdef DEBUG_MEDIAENGINE
#define TRACE(x...) printf("MediaEngine: " x)
#define CALLED() TRACE("called %s\n", __PRETTY_FUNCTION__)
#else
#define TRACE(x...)
#define CALLED()
#endif

#define ERROR(x...) printf("MediaEngine: " x)


int32
MediaEngine::MediaPlayer( void *arg )
{
	MediaEngine* view = (MediaEngine *)arg;
	AudioEngine* audioEngine = view->fAudioEngine;
	BMediaTrack* audioTrack = view->fAudioTrack;

	bool scrubbing = false;
	bool seekNeeded = false;

	int64 numFrames = audioTrack->CountFrames();

	bigtime_t vStartTime, aStartTime, seekTime, snoozeTime, startTime;
	bigtime_t curScrubbing, lastScrubbing, lastTime;

	int64 numFramesToSkip = 0;
	int64 numSkippedFrames = 0;

	TRACE("%s: Initial Frame: %Ld, total frames: %Ld\n", __func__,
		audioTrack->CurrentFrame(), numFrames);

	// Main processing loop start,stop
	while (acquire_sem(view->fPlaySem) == B_OK) {
		TRACE("%s: Enter main processing loop\n", __func__);
		release_sem(view->fPlaySem);

		if (audioTrack != NULL)
			audioEngine->Play();
		startTime = system_time()-audioTrack->CurrentTime();

		while((audioTrack->CurrentFrame() < numFrames) || scrubbing) {

			// Entering Scrub mode!
			if (acquire_sem(view->fScrubSem) == B_OK) {
				curScrubbing = system_time();
	
				if (!scrubbing) {
					if (audioTrack != NULL)
						audioEngine->Stop();
					scrubbing = true;
				}
				seekNeeded = true;
				seekTime = view->fScrubTime;
			}
			// Eh.. who needs to scrub?
			else if (scrubbing) {
				if (audioTrack != NULL)
					audioEngine->Play();
				scrubbing = false;
			}
			// Normal play mode (finally!)
			else {
				// Estimate snoozeTime
				if (audioTrack != NULL)
					startTime = audioEngine->TrackTimebase();
				else
					snoozeTime = 25000;

				if (snoozeTime > 5000LL) {
					view->fSnoozing = true;
					snooze(snoozeTime-1000);
					view->fSnoozing = false;
				} else if (snoozeTime < -5000) {
					numSkippedFrames++;
					numFramesToSkip++;
				}

				if (!scrubbing) {
					view->fCurTime = system_time() - startTime;
					if (view->fCurTime < seekTime)
						view->fCurTime = seekTime;
				}

			}

			if (acquire_sem_etc(view->fPlaySem, 1, B_TIMEOUT, 0) == B_OK)
				release_sem(view->fPlaySem);
			else {
				if (audioTrack != NULL )
					audioEngine->Stop();
				goto do_restart;
			}

			TRACE("%s: Initial Frame: %Ld, total frames: %Ld\n", __func__,
				audioTrack->CurrentFrame(), numFrames);

		}

		if (audioTrack->CurrentFrame() >= numFrames) {
do_reset:
			if (audioTrack != NULL)
				audioEngine->Stop();

			seekNeeded = true;
			seekTime = 0LL;
			scrubbing = true;
		}
do_restart:;
	}

return(B_NO_ERROR);
}


status_t 
MediaEngine::SetAudioTrack(const char *path, BMediaTrack *track,
	media_format *format)
{
	CALLED();

	/*
	if (fAudioTrack != NULL)
	{
		util.debug("MediaEngine::SetAudioTrack: fAudioTrack is not null!",0);
		return (B_ERROR);
	}
	*/

	fAudioTrack = track;

	fAudioEngine = new AudioEngine(fAudioTrack, BPath(path).Leaf());
	status_t err = fAudioEngine->InitCheck();
	if (err != B_NO_ERROR) {
		delete (fAudioEngine);
		fAudioEngine = NULL;
		fAudioTrack = NULL;
		
		return (err);
	}

	fAudioDumpingBuffer = malloc(format->u.raw_audio.buffer_size);

	return (B_NO_ERROR);
}


status_t
MediaEngine::SetSource(const char *path)
{
	entry_ref ref;
	status_t err = get_ref_for_path(path, &ref);

	if (err != B_NO_ERROR)
		return (err);

	TRACE("%s: Passing media file references to BMediaFile...\n", __func__);

	fMediaFile = new BMediaFile(&ref);

	if (fMediaFile == NULL)
		return (B_ERROR);


	bool foundTrack = false;
	int32 numTracks = fMediaFile->CountTracks();

	TRACE("%s: examining media file...\n", __func__);

	// for each track in media file
	for (int32 i = 0; i < numTracks; i++) {
		BMediaTrack *track = fMediaFile->TrackAt(i);
		
		if (track == NULL) {
			Reset();
			return (B_ERROR);
		} else {
			bool trackUsed = false;
			media_format mf;

			if (track->EncodedFormat(&mf) == B_NO_ERROR) {
				TRACE("%s: found a valid track.\n", __func__);

				switch (mf.type) {
					case B_MEDIA_ENCODED_AUDIO:
						TRACE("%s: Field rate %f\n", __func__,
							mf.u.encoded_video.output.field_rate);
						if (track->DecodedFormat(&mf) == B_NO_ERROR)
							trackUsed = SetAudioTrack(path, track, &mf) == B_NO_ERROR;
						break;

					case B_MEDIA_RAW_AUDIO:
						trackUsed = SetAudioTrack(path, track, &mf) == B_NO_ERROR;
						break;

					default:
						break;
				}
			}
			
			if (trackUsed) {
				TRACE("%s: found valid track(s)\n", __func__);
				foundTrack = true;
			} else
				fMediaFile->ReleaseTrack(track);
		}
	}

	if (foundTrack) {
		status_t err = B_ERROR;
		fPlayerThread = spawn_thread(MediaEngine::MediaPlayer, "MediaEngine::MediaPlayer", B_NORMAL_PRIORITY, this);

		if (fPlayerThread < B_NO_ERROR) {
			err = fPlayerThread;
			fPlayerThread = B_ERROR;
			Reset();
			
			return (err);
		}

		fPlaySem = create_sem(0, "MediaEngine::fPlaySem");
		
		if (fPlaySem < B_NO_ERROR) {
			err = fPlaySem;
			fPlaySem = B_ERROR;
			Reset();
			
			return (err);
		}

		err = resume_thread(fPlayerThread);

		if (err != B_NO_ERROR) {
			kill_thread(fPlayerThread);
			fPlayerThread = B_ERROR;
			Reset();

			return (err);
		}

		//fMediaBar->SetTotalTime(fAudioTrack->Duration());
	}


	return B_NO_ERROR;
}


void
MediaEngine::Reset()
{
	fAudioTrack = NULL;

	delete(fMediaFile);
	fMediaFile = NULL;
}
