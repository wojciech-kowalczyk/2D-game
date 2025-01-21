#include "Error.h"
#include <GL/gl.h>
#include <iostream>
#include "config.h"

Error::~Error() { closeAllFiles(); }

void Error::openFile(char const *const filePath) {
	file.addLast(std::ofstream {});
	
	file.getLast().open(filePath);
	
	file.getLast() << project::NAME << ' ' << project::version::MAJOR << '.' << project::version::MINOR << "\n\n";
}

void Error::closeLastOpenedFile() {
	file.getLast().close();
	file.removeLast();
}

void Error::closeAllFiles() {
	while (!file.empty())
		closeLastOpenedFile();
}

bool Error::checkErrors(char const *const srcFilePath, int const line, int const errorType) {
	bool errorOccurred {false};
	
	if (errorType & Type::Windows) {
		unsigned long const errorCode {GetLastError()};
		if (errorCode) {
			char errorDescription[200] {};
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			               errorDescription, 200, nullptr);
			std::cout << "Windows error in file \"" << srcFilePath << "\" on line " << line << ", error code " << errorCode << ", description: " << errorDescription
			          << '\n';
			SetLastError(0);
			errorOccurred = true;
		}
	}
	
	if (errorType & Type::Opengl) {
		unsigned const errorCode {glGetError()};
		if (errorCode) {
			std::cout << "Opengl error in file \"" << srcFilePath << "\" on line " << line << ", error code " << errorCode << '\n';
			errorOccurred = true;
		}
	}
	
	return errorOccurred;
}

void Error::printError(char const *const srcFilePath, int const line, char const *const description) {
	std::cout << "Error in file \"" << srcFilePath << "\" on line " << line << ", error description: " << description << '\n';
}

void Error::printError(char const *const srcFilePath, int const line) {
	std::cout << "Error in file \"" << srcFilePath << "\" on line " << line << ".\n";
}
