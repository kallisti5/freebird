/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */


#include <stdio.h>
#include "FreebirdUtils.h"


void
Utils::debug(const char * message, int fatal)
{
	printf("DEBUG: %s\n", message);
}
