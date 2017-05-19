// (c) 2017 Dewey Taylor

#ifndef APP_H
#define APP_H

#include <Application.h>

#include "MainWindow.h"

#define	APP_SIG		"application/x-vnd.jdt-codepal"
// moved to ProjectController.h
//#define PROJ_SIG	"text/x-vnd.jdt-codepal.project"

class App : public BApplication
{
	public:
					App(void);
		void		RefsReceived(BMessage*);

	private:
		WindowRef*	fWindowList;

		void		_InstallMimeType();
};

#endif
