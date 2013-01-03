/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */
 

#include <Entry.h>
#include <storage/Directory.h>

#include "FreebirdUtils.h"
#include "FreebirdEngine.h"


bool
FreebirdEngine::ReindexMusic(const char *path)
{
	Utils util; // utilities including debug statements
	util.debug("FreebirdEngine::ReindexMusic called",0);

	BDirectory dir(path);
	BEntry entry;

	dir.Rewind();
	while (dir.GetNextEntry(&entry) == B_OK) {
		util.debug("FreebirdEngine::ReindexMusic found a file",0);
	}
}
