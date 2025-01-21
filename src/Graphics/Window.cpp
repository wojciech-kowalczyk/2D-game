#include "Window.h"
#include "renderFunctions.h"

static long long CALLBACK dummyWindowProc(HWND__ *const windowHandle, unsigned const message, unsigned long long const wParam, long long const lParam) {
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(windowHandle, message, wParam, lParam);
}

Window::Window(WNDPROC windowProcedure, char const *const title, Size<int> const &size, int const antialiasingLevel) {
	extern HINSTANCE__ *instanceHandle;
	
	HWND__ *const dummyWindowHandle {[] {
		tagWNDCLASSA dummyWindowClass {};
		
		dummyWindowClass.lpfnWndProc = dummyWindowProc;
		dummyWindowClass.hInstance = instanceHandle;
		dummyWindowClass.style = CS_OWNDC;
		dummyWindowClass.lpszClassName = "dummyWindowClass";
		
		RegisterClassA(&dummyWindowClass);
		
		return CreateWindowExA(0, "dummyWindowClass", nullptr, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, instanceHandle, nullptr);
	}()};
	HDC__ *const dummyDeviceContextHandle {GetDC(dummyWindowHandle)};
	
	// Set pixel format
	{
		tagPIXELFORMATDESCRIPTOR const dummyPixelFormatDescriptor {sizeof(tagPIXELFORMATDESCRIPTOR),
		                                                           1,
		                                                           PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		                                                           PFD_TYPE_RGBA,
		                                                           32,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           24, // Number of bits for the depth buffer
		                                                           8,  // Number of bits for the stencil buffer
		                                                           0,  // Number of Aux buffers in the frame buffer.
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0,
		                                                           0};
		
		SetPixelFormat(dummyDeviceContextHandle, ChoosePixelFormat(dummyDeviceContextHandle, &dummyPixelFormatDescriptor), &dummyPixelFormatDescriptor);
	}
	
	HGLRC__ *const dummyGlContextHandle {wglCreateContext(dummyDeviceContextHandle)};
	wglMakeCurrent(dummyDeviceContextHandle, dummyGlContextHandle);
	
	renderFunctions::init();
	
	// Destroy dummy window and context
	{
		wglMakeCurrent(nullptr, nullptr);
		ReleaseDC(dummyWindowHandle, dummyDeviceContextHandle);
		wglDeleteContext(dummyGlContextHandle);
		DestroyWindow(dummyWindowHandle);
		
		tagMSG dummyMessage {};
		while (GetMessageA(&dummyMessage, nullptr, 0, 0)) {
			TranslateMessage(&dummyMessage);
			DispatchMessageA(&dummyMessage);
		}
	}
	
	handle = [&windowProcedure, &size, title] {
		// window class
		{
			tagWNDCLASSA windowClass {};
			
			windowClass.lpfnWndProc = windowProcedure;
			windowClass.hInstance = instanceHandle;
			windowClass.style = CS_OWNDC;
			windowClass.lpszClassName = "windowClass";
			windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			
			RegisterClassA(&windowClass);
		}
		
		int const screenWidth {GetSystemMetrics(SM_CXSCREEN)};
		int const screenHeight {GetSystemMetrics(SM_CYSCREEN)};
		
		return CreateWindowExA(0, "windowClass", title, WS_OVERLAPPEDWINDOW, screenWidth / 2 - size.x / 2, screenHeight / 2 - size.y / 2,
		                       size.x, size.y, nullptr, nullptr, instanceHandle, nullptr);
	}();
	deviceContextHandle = GetDC(handle);
	
	titleBarHeight = GetSystemMetrics(SM_CYSIZE);
	borderSize = {GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME)};
	screenSize = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	
	using namespace renderFunctions::window;
	
	// Pixel format
	{
		auto setPixelFormat = [this](int const *const attributes) {
			int pixelFormat;
			unsigned formatCount;
			wglChoosePixelFormatARB(deviceContextHandle, attributes, nullptr, 1, &pixelFormat, &formatCount);
			SetPixelFormat(deviceContextHandle, pixelFormat, nullptr);
		};
		
		if (antialiasingLevel) {
			int pixelFormatAttributeList[19] {WGL_DRAW_TO_WINDOW_ARB,
			                                  true,
			                                  WGL_SUPPORT_OPENGL_ARB,
			                                  true,
			                                  WGL_DOUBLE_BUFFER_ARB,
			                                  true,
			                                  WGL_PIXEL_TYPE_ARB,
			                                  WGL_TYPE_RGBA_ARB,
			                                  WGL_COLOR_BITS_ARB,
			                                  32,
			                                  WGL_DEPTH_BITS_ARB,
			                                  24,
			                                  WGL_STENCIL_BITS_ARB,
			                                  8,
			                                  WGL_SAMPLE_BUFFERS_ARB,
			                                  true,
			                                  WGL_SAMPLES_ARB,
			                                  antialiasingLevel,
			                                  0};
			setPixelFormat(pixelFormatAttributeList);
		} else {
			int pixelFormatAttributeList[15] {WGL_DRAW_TO_WINDOW_ARB,
			                                  true,
			                                  WGL_SUPPORT_OPENGL_ARB,
			                                  true,
			                                  WGL_DOUBLE_BUFFER_ARB,
			                                  true,
			                                  WGL_PIXEL_TYPE_ARB,
			                                  WGL_TYPE_RGBA_ARB,
			                                  WGL_COLOR_BITS_ARB,
			                                  32,
			                                  WGL_DEPTH_BITS_ARB,
			                                  24,
			                                  WGL_STENCIL_BITS_ARB,
			                                  8,
			                                  0};
			setPixelFormat(pixelFormatAttributeList);
		}
	}
	
	// Context handle
	{
		// Int array of pairs attribute/value that ends with 0.
		constexpr int contextAttributes[] {WGL_CONTEXT_MAJOR_VERSION_ARB,
		                                   4,
		                                   WGL_CONTEXT_MINOR_VERSION_ARB,
		                                   6,
		                                   WGL_CONTEXT_PROFILE_MASK_ARB,
		                                   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		                                   0};
		glContextHandle = wglCreateContextAttribsARB(deviceContextHandle, nullptr, contextAttributes);
		wglMakeCurrent(deviceContextHandle, glContextHandle);
	}
	
	glEnable(GL_MULTISAMPLE);
}

Window::~Window() {
	setCurrent(false);
	ReleaseDC(handle, deviceContextHandle);
	wglDeleteContext(glContextHandle);
	DestroyWindow(handle);
}

void Window::setClearColor(Color const &colorInit) const { glClearColor(colorInit.red, colorInit.green, colorInit.blue, colorInit.alpha); }

void Window::setCurrent(bool const current) const {
	if (current)
		wglMakeCurrent(deviceContextHandle, glContextHandle);
	else
		wglMakeCurrent(nullptr, nullptr);
}

void Window::setFocus(bool const focus) const { SetFocus(focus ? handle : nullptr); }

void Window::setSize(Size<int> const &size) const { SetWindowPos(handle, nullptr, 0, 0, size.x, size.y, SWP_NOMOVE); }

Point<int> Window::getPosition() const {
	tagRECT windowRect {};
	GetWindowRect(handle, &windowRect);
	return {windowRect.left + borderSize.x, screenSize.y - windowRect.bottom + borderSize.y};
}

Size<int> Window::getSize() const {
	tagRECT windowRect {};
	GetWindowRect(handle, &windowRect);
	return {windowRect.right - windowRect.left, windowRect.bottom - windowRect.top};
}

Size<int> Window::getClientSize() const {
	tagRECT clientRect {};
	GetClientRect(handle, &clientRect);
	return {clientRect.right - clientRect.left, clientRect.bottom - clientRect.top};
}

Window::State Window::getState() const {
	tagWINDOWPLACEMENT windowPlacement {};
	GetWindowPlacement(handle, &windowPlacement);
	
	if (windowPlacement.showCmd == SW_SHOWMAXIMIZED)
		return State::Maximized;
	if (windowPlacement.showCmd == SW_SHOWMINIMIZED)
		return State::Minimized;
	return State::Normal;
}

void Window::show(Show const show) const { ShowWindow(handle, show == Show::Normal ? SW_NORMAL : SW_MAXIMIZE); }

void Window::swapBuffers() const { SwapBuffers(deviceContextHandle); }

void Window::clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Window::startEventLoop() const {
	tagMSG message {};
	while (GetMessageA(&message, nullptr, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}
}
