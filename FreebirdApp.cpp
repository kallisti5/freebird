/*
 * Freebird, the Music Jukebox for Haiku
 * Copyright 2010-2013, Alexander von Gluck IV
 * Released under the terms of the MIT license
 *
 * http://unixzen.com, http://github.com/kallisti5
 */
 

#include <Application.h>
#include <Button.h>
#include <stdio.h>
#include <View.h>
#include <Window.h>

#include "FreebirdPlayer.h"
#include "FreebirdEngine.h"
#include "MediaEngine.h"


#define DEBUG_APP
#ifdef DEBUG_APP
#define TRACE(x...) printf("FreebirdApp: " x)
#define CALLED() TRACE("called %s\n", __PRETTY_FUNCTION__)
#else
#define TRACE(x...)
#define CALLED()
#endif

#define ERROR(x...) printf("FreebirdApp: " x)


class PlayerView : public BView {
        public:
                PlayerView(BRect frame);
                virtual void    Draw(BRect updateRect);
};


PlayerView::PlayerView(BRect frame)
                : BView(frame, "FreeBird Player", B_FOLLOW_ALL_SIDES, B_WILL_DRAW) {

}


// Draw our player view
void
PlayerView::Draw(BRect updateRect)
{
	CALLED();

	MovePenTo(BPoint(148,20));
	DrawString("Title: Awesomesauce");
	MovePenTo(BPoint(148,35));
	DrawString("Author: Elite Midgets");

	BButton *buttonPLAY;
	buttonPLAY = new BButton(BRect (10.0,145.0,60.0,30.0), "PLAYButton",
		"Play", new BMessage(BUTTON_PLAY_MSG));
	BButton *buttonPAUSE;
	buttonPAUSE = new BButton(BRect (65.0,145.0,120.0,30.0), "PAUSEButton",
		"Pause", new BMessage(BUTTON_PAUSE_MSG));
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
	:
	BWindow(frame, "Freebird Player", B_FLOATING_WINDOW_LOOK,
		B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE
		| B_ASYNCHRONOUS_CONTROLS)
{
			AddChild(new PlayerView(Bounds()));
			Show();
}


bool
PlayerWindow::QuitRequested()
{
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
	:
	BApplication("application/x-vnd.FreeBird")
{
        BRect windowRect;

        windowRect.Set(50,50,400,230);
        theWindow = new PlayerWindow(windowRect);
}


int
main(void)
{
	CALLED();

	MediaEngine mediaEngine;
	FreebirdEngine *theEngine;
	FreebirdApp *theApp;

	theApp = new(FreebirdApp);

	theEngine->ReindexMusic("/boot/home/Music");
	mediaEngine.SetSource("/Data/Projects/freebird/resources/testmp3.mp3");
	theApp->Run();
	delete theApp;

	return 0;
}
