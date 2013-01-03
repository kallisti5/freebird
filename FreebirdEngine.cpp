/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */
 

#include <Entry.h>
#include <stdio.h>
#include <storage/Directory.h>

#include "FreebirdEngine.h"


#define DEBUG_ENGINE
#ifdef DEBUG_ENGINE
#define TRACE(x...) printf("FreebirdEngine: " x)
#define CALLED() TRACE("called %s\n", __PRETTY_FUNCTION__)
#else
#define TRACE(x...)
#define CALLED()
#endif

#define ERROR(x...) printf("FreebirdEngine: " x)


bool
FreebirdEngine::ReindexMusic(const char *path)
{
	CALLED();

	BDirectory dir(path);
	BEntry entry;

	dir.Rewind();
	while (dir.GetNextEntry(&entry) == B_OK) {
		TRACE("FreebirdEngine::ReindexMusic found a file");
	}
}
