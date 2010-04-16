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

#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Button.h>

#include "FreebirdUtils.h"	// Debug stuff and generic functions
#include "FreebirdPlayer.h"	// Stuff for the player window
#include "FreebirdEngine.h"	// Engine stuff (file management)
#include "MediaEngine.h"	// Media Engine stuff (audio playback)


class PlayerView : public BView {
        public:
                PlayerView(BRect frame);
                virtual void    Draw(BRect updateRect);
};

PlayerView::PlayerView(BRect frame)
                : BView(frame, "FreeBird Player", B_FOLLOW_ALL_SIDES, B_WILL_DRAW) {

}

// Draw our player view
void PlayerView::Draw(BRect updateRect) {
	Utils util;

	util.debug("PlayerView::Draw called",0);

        MovePenTo(BPoint(148,20));
        DrawString("Title: Awesomesauce");
        MovePenTo(BPoint(148,35));
        DrawString("Author: Elite Midgets");

        BButton *buttonPLAY;
        buttonPLAY = new BButton(BRect (10.0,145.0,60.0,30.0), "PLAYButton", "Play", new BMessage(BUTTON_PLAY_MSG));
        BButton *buttonPAUSE;
        buttonPAUSE = new BButton(BRect (65.0,145.0,120.0,30.0), "PAUSEButton", "Pause", new BMessage(BUTTON_PAUSE_MSG));
        AddChild(buttonPLAY);
        AddChild(buttonPAUSE);

	FillRect(BRect(10,10,138,138), B_SOLID_HIGH);	// Album artwork TODO
}


class PlayerWindow : public BWindow {
	public:
			PlayerWindow(BRect frame);
	virtual bool	QuitRequested();
};

PlayerWindow::PlayerWindow(BRect frame)
		: BWindow(frame, "Freebird Player",
			B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_ASYNCHRONOUS_CONTROLS) {
			AddChild(new PlayerView(Bounds()));
			Show();
}

bool PlayerWindow::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

// Our Application
class FreebirdApp : public BApplication {
        public:
                FreebirdApp();

        private:
                PlayerWindow    *theWindow;
};

FreebirdApp::FreebirdApp()
                : BApplication("application/x-vnd.FreeBird") {
        BRect windowRect;

        windowRect.Set(50,50,400,230);
        theWindow = new PlayerWindow(windowRect);
}


int main(void) {

        Utils util; // utilities including debug statements
        util.debug("enter main()", 0);

	MediaEngine *mediaEngine;
	FreebirdEngine *theEngine;
	//FreebirdApp *theApp;

	//theApp = new(FreebirdApp);

	//theEngine->ReindexMusic("/boot/home/Music");
	mediaEngine->SetSource("/boot/home/develop/freebird/resources/testmp3.mp3");
	//theApp->Run();
	//delete theApp;
        util.debug("exit main()", 1);
	return 0;
}
