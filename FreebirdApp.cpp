/*
 * FreeBird
 * here we take care of drawing the basics of the interface
 *
 */

#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Button.h>

#include "Utils.h"
#include "FreebirdPlayer.h"

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
		: BWindow(frame, "Freebird Player", B_TITLED_WINDOW, B_NOT_RESIZABLE|B_NOT_ZOOMABLE) {
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

void main(void) {
        Utils util; // utilities including debug statements
        util.debug("enter main()", 0);

	FreebirdApp *theApp;
	theApp = new(FreebirdApp);
	theApp->Run();
	delete theApp;
        util.debug("exit main()", 1);
}