#include <windows.h>
#include <QApplication>
#include "Ui/MainMenu.h"

HINSTANCE__ *instanceHandle;
bool error; // All functions that can generate error say it in documentation
std::string errorDescription; // When function generates error it also updates errorDescription

int WINAPI WinMain(HINSTANCE__ *const instanceHandleInit, HINSTANCE, char *, int) {
	instanceHandle = instanceHandleInit;
	
	// Conversion of windows' cmd program arguments to argv style array of arguments
	// Actually because for now there is no use for command line arguments, only first argument (program's path) is converted and passed to QApplication
	char *path; // free this
	{
		char const *const cmdLine {GetCommandLineA()}; // don't free this
		
		if (cmdLine[0] == '"') {
			// 9 because the shortest possible path with quotes to executable has 10 characters: "C:\a.exe"
			int indexOfSecondQuote {9};
			
			while (cmdLine[indexOfSecondQuote] != '"') ++indexOfSecondQuote;
			
			path = new char[indexOfSecondQuote]; // -2 quotes, +1 null-terminated, +1 because it's index and pathSize is size
			
			for (int i = 0, end = indexOfSecondQuote - 1; i < end; ++i)
				path[i] = cmdLine[i + 1];
			
			path[indexOfSecondQuote - 1] = '\0';
		} else {
			// 8 because the shortest possible path to executable has 8 characters: C:\a.exe
			int indexOfFirstSpace {8};
			
			path = new char[indexOfFirstSpace + 1]; // -1 space, +1 null-terminated, +1 because it's index and pathSize is size
			
			for (int i = 0; i < indexOfFirstSpace; ++i)
				path[i] = cmdLine[i];
			
			path[indexOfFirstSpace] = '\0';
		}
	}
	
	int argc {1};
	
	QApplication const qApplication {argc, &path};
	MainMenu const mainMenu;
	
	delete[] path;
	
	return QApplication::exec();
}
