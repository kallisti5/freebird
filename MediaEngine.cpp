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

#include "FreebirdUtils.h"
#include "MediaEngine.h"

status_t 
MediaEngine::SetAudioTrack( const char *path, BMediaTrack *track, media_format *format) {

	if (fAudioTrack != NULL)
		return (B_ERROR);

	fAudioTrack = track;

	/*
	fAudioEngine = new AudioEngine(fAudioTrack, BPath(path).Leaf());
	status_t err = fAudioEngine->InitCheck();
	if (error != B_NO_ERROR) {
		delete (fAudioEngine);
		fAudioEngine = NULL;
		fAudioTrack = NULL;
		
		return (err);
	}

	fAudioDumpingBuffer = malloc(format->u.raw_audio.buffer_size);
	*/

	return (B_NO_ERROR);
}

status_t
MediaEngine::SetSource(const char *path) {
	Utils util;

	status_t	err = B_ERROR;
	entry_ref	ref;

	err = get_ref_for_path(path, &ref);

	if (err != B_NO_ERROR)
		return (err);

	util.debug("MediaEngine::SetSource passing media file reference to BMediaFile", 0);

	fMediaFile = new BMediaFile(&ref);

	if (fMediaFile = NULL)
		return (B_ERROR);


	bool		foundTrack = false;
	int32		numTracks = fMediaFile->CountTracks();

	util.debug("MediaEngine::SetSource examining media file", 0);

	// for each track in media file
	for (int32 i = 0; i < numTracks; i++) {
		BMediaTrack *track = fMediaFile->TrackAt(i);
		
		if (track == NULL) {
			Reset();
			return (B_ERROR);
		}
		else {
			bool		trackUsed = false;
			media_format	mf;

			if (track->EncodedFormat(&mf) == B_NO_ERROR) {

				util.debug("MediaEngine::SetSource found a valid track",0);

				switch (mf.type) {
					case B_MEDIA_ENCODED_AUDIO:
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
				util.debug("Media::SetSource found valid track(s)",0);
				foundTrack = true;
			} else
				fMediaFile->ReleaseTrack(track);
		}
	}

	// TODO if (foundTrack)

	return B_NO_ERROR;
}

void MediaEngine::Reset()
{

fAudioTrack = NULL;

delete(fMediaFile);
fMediaFile = NULL;

}
