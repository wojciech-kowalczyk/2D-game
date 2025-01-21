#ifndef ERROR_H
#define ERROR_H

#include <fstream>
#include "Libraries/List.h"

#define CheckErrors(errorType) Error::checkErrors(__FILE__, __LINE__, errorType)

class Error {
public:
	~Error();
	
	void openFile(char const *filePath);
	void closeLastOpenedFile();
	void closeAllFiles();
	/// \returns True on error
	static bool checkErrors(char const *srcFilePath, int line, int errorType);
	static void printError(char const *srcFilePath, int line, char const *description);
	static void printError(char const *srcFilePath, int line);
	
	enum Type { Windows = 1, Opengl = 1 << 1 };
	
	List<std::ofstream> file;
};

#endif // ERROR_H
