#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include "Math/Point.h"
#include "Color.h"

class Window {
public:
	/// Creates window and makes context current
	/// \see show()
	Window(WNDPROC windowProcedure, char const *title, Size<int> const &size, int antialiasingLevel);
	~Window();
	
	void setClearColor(Color const &color) const;
	/// Sets rendering context as current
	void setCurrent(bool current) const;
	/// Must be called from a thread that runs window's corresponding procedure
	void setFocus(bool focus) const;
	/// Sets size without moving window
	void setSize(Size<int> const &size) const;
	
	/// Get client left-down vertex position
	[[nodiscard]] Point<int> getPosition() const;
	/// Get window size
	[[nodiscard]] Size<int> getSize() const;
	/// Get client size
	[[nodiscard]] Size<int> getClientSize() const;
	
	enum class State { Normal, Maximized, Minimized };
	
	[[nodiscard]] State getState() const;
	
	enum class Show { Normal, Maximized };
	
	void show(Show show) const;
	void swapBuffers() const;
	void clear() const;
	void startEventLoop() const;
	
	int titleBarHeight;
	Size<int> screenSize, borderSize;
private:
	HWND__ *handle;
	HGLRC__ *glContextHandle;
	HDC__ *deviceContextHandle;
};

#endif // WINDOW_H
