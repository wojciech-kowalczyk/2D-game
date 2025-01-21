#ifndef LEVEL_H
#define LEVEL_H

#include <thread>
#include "Graphics/Window.h"
#include "Libraries/List.h"
//#include "Others/Camera.h"
#include "Shapes/EntitiesTemp.h"

class Level {
public:
	/// \warning Can generate error
	explicit Level(std::string levelName);
	
	void start();
private:
	void renderThread();
	static long long CALLBACK windowProcedure(HWND__ *windowHandle, unsigned message, unsigned long long wParam, long long lParam);
	[[nodiscard]] Point<int> getMousePosition() const;
	
	// For windowProcedure to have access to *this, because non-static method can't be passed to Windows' window class.
	// Obviously, it means that only one object of this class can exist at the same time, else thisPtr will be invalid for previous object,
	// because constructor sets thisPtr value.
	// Other way is to use setProp(hwnd, this) and getProp(hwnd, this) functions from Windows API, but this is slower and for now multiple objects of this class
	// at the same time are not needed.
	static Level *thisPtr;
	
	bool run;
	Size<int> savedWindowSize;
	Window::Show firstWindowShow;
	std::thread *renderThreadHandle;
	
	Window *window;
	EntitiesTemp *entities;
//	Camera camera;
	
	struct AtWindowDestruction {
		bool maximized;
		Size<int> size;
	} atWindowDestruction;
	
	struct Event {
		enum Key { Space };
		enum MouseKey { Left, Right };
		enum Scroll { Up, Down };
		
		enum Type { Resized, KeyPressed, MouseKeyPressed, MouseWheelScrolled } type;
		
		union {
			Key key;
			MouseKey mouseKey;
			Scroll scroll;
			struct { int left, right, top, bottom; } size;
		};
	};
	
	List<Event const> eventQueue;
};

#endif // LEVEL_H
