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

#include <Entry.h>
#include <storage/Directory.h>

#include "FreebirdUtils.h"
#include "FreebirdEngine.h"

bool FreebirdEngine::ReindexMusic(const char *path)
{
        Utils util; // utilities including debug statements

        BDirectory dir(path);
        BEntry entry;

        dir.Rewind();
        while (dir.GetNextEntry(&entry) == B_OK)
                {
                        util.debug("file",0);
                }
}