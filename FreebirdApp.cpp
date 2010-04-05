#include <Application.h>
#include <Window.h>
#include <View.h>

#include "Utils.h"

class PlayerView : public BView {
        public:
                PlayerView(BRect frame);
                virtual void    Draw(BRect updateRect);
};

PlayerView::PlayerView(BRect frame)
                : BView(frame, "FreeBird Player", B_FOLLOW_ALL_SIDES, B_WILL_DRAW) {
}

void PlayerView::Draw(BRect updateRect) {
        MovePenTo(BPoint(20,75));
        DrawString("Hello Freebird you crazy cat!");
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

        windowRect.Set(50,50,200,200);
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
