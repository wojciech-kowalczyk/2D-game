#include "Level.h"
#include "Libraries/utility.h"
#include "Graphics/Shader.h"
#include "Xml/Settings.h"

Level *Level::thisPtr {nullptr};

Level::Level(std::string levelName) : run(true), renderThreadHandle(nullptr), window(nullptr), atWindowDestruction() {
	Level::thisPtr = this;
	
	// sound
	{
//		if (!ballSoundBuffer.loadFromFile("D:/Dokumenty/Projekty/Moje/Golf/Resources/Sound/ballReflection.wav")) {
//          error = true;
//			errorDescription = R"(Error occurred when loading file "D:/Dokumenty/Projekty/Moje/Golf/Resources/Sound/ballReflection.wav".)";
//			return;
//		}
//		ballSound.setBuffer(ballSoundBuffer);
	}

//	if (!entities.load(levelName)) {
//		errorDescription = entities.getErrorDescription();
//		return;
//	}
	
	xml::Settings settings;
	settings.load();
	
	if (!error) {
		// camera
//		camera.setManualMoveSpeed(settings.camera.manualMoveSpeed);
//		camera.setMargin({settings.camera.followMargin.x, settings.camera.followMargin.y});
		
		// window and background
		{
			firstWindowShow = settings.window.maximize ? Window::Show::Maximized : Window::Show::Normal;
			savedWindowSize = settings.window.size;
			
			window = new Window {windowProcedure, levelName.c_str(), {settings.window.size.x, settings.window.size.y}, settings.window.antialiasing};
			
			entities = new EntitiesTemp {std::move(levelName)};
			
			window->setCurrent(false);
		}
	}
}

void Level::start() {
	renderThreadHandle = new std::thread {[this] { this->renderThread(); }};
	
	window->startEventLoop();
	
	xml::Settings settings;
	settings.load();
	
	if (!error) {
		if (atWindowDestruction.maximized)
			settings.window.maximize = On;
		else {
			settings.window.maximize = Off;
			settings.window.size = atWindowDestruction.size;
		}
		
		settings.save();
	}
}

void Level::renderThread() {
	window->setCurrent(true);
	
	Shader shader {Shader::Type::Color}; // temp

//	Clock cameraMoveClock;
	
	window->show(firstWindowShow); // TODO to ma być po pierwszym renderze, a zaraz po pokazaniu okna trzeba swapować buffery

	entities->hole->update(window->getSize(), shape::sub::Base::DrawType::Static);
	
	while (run) {
//		// Different handling of resizing, because resize event stops event thread
//		{
//			int const lastWindowHeight {windowHeight};
//			windowHeight = [this] {
//				tagRECT rect {};
//				GetClientRect(handle.window, &rect);
//				return rect.bottom - rect.top;
//			}();
//
//			if (windowHeight != lastWindowHeight) {
//				if (IsIconic(handle.window))
//					camera.setAutoMoveStatus(Off);
//
//				camera.setShift({camera.getShift().x, camera.getShift().y + static_cast<float>(lastWindowHeight) - static_cast<float>(windowHeight)});
//				entities.background.adjustTextureAfterWindowResize(windowWidth, windowHeight);
//			}
//		}
		while (!eventQueue.empty()) {
			Event const event {eventQueue.removeFirst()};
			switch (event.type) {
				case Event::Type::Resized: {
//					windowSize.x = event.size.right - event.size.left;
//					windowSize.y = event.size.bottom - event.size.top;
//					SetWindowPos(handle.window, nullptr, 0, 0, windowWidth, windowHeight, SWP_ASYNCWINDOWPOS | SWP_NOMOVE);
					break;
				}
				case Event::Type::KeyPressed: {
					switch (event.key) {
						case Event::Key::Space:
//							camera.setAutoMoveStatus(!camera.getAutoMoveStatus());
							break;
					}
					break;
				}
//				case Event::Type::MouseKeyPressed: {
//					switch (event.mouseKey) {
//						case Event::MouseKey::Left:
//							if (entities.arrow.getStatus())
//								entities.ball.addForce(entities.arrow.getDrawingHeight() / camera.getScale(), entities.arrow.getDirection());
//							break;
//						case Event::MouseKey::Right:
//							entities.arrow.setStatus(!entities.arrow.getStatus());
//							break;
//					}
//					break;
//				}
//				case Event::Type::MouseWheelScrolled: {
//					if (camera.getScale() > 0.4 || event.scroll == Event::Scroll::Up)
//						// prevent stacking multiple inputs from one frame — limit of one wheel scroll per frame
//						camera.setScale(camera.getScale() + (event.scroll == Event::Scroll::Up ? 1 : -1) / 6., getMousePosition());
//					break;
//				}
				default:;
			}
		}
		
		// manual camera move
		{
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
//				camera.manualMove(90, cameraMoveClock.getElapsedTime().asSeconds());
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
//				camera.manualMove(180, cameraMoveClock.getElapsedTime().asSeconds());
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
//				camera.manualMove(270, cameraMoveClock.getElapsedTime().asSeconds());
//			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
//				camera.manualMove(0, cameraMoveClock.getElapsedTime().asSeconds());
//
//			cameraMoveClock.restart();
		}
		
		// move ball
//		if (entities.ball.isMoving()) {
//			int const moveReturnCode {entities.ball.move(entities.physical.polygon, entities.physical.polygonCount, entities.physical.circle,
//			                                             entities.physical.circleCount, entities.surface.polygon, entities.surface.polygonCount,
//			                                             entities.surface.circle, entities.surface.circleCount, entities.hole, entities.holeCount)};
//			if (moveReturnCode == 1); // TODO ball sound
//			else if (moveReturnCode == 2) {
//				run = false;
//				return;
//			}
//		}
		
		// auto move camera based on position of ball
//		if (camera.getAutoMoveStatus())
//			camera.autoMove(static_cast<Point<int>>(entities.ball.getPosition()), windowSize);
		
		// update positions
		{
//			entities.background.updateForDrawing(camera, windowSize);
//			entities.ball.updateForDrawing(camera);
//			entities.arrow.updateForDrawing(camera, entities.ball, getMousePosition());
//			for (int i = 0; i < entities.holeCount; ++i)
//				entities.hole[i].updateForDrawing(camera);
//			for (int i = 0; i < entities.physical.polygonCount; ++i)
//				entities.physical.polygon[i].updateForDrawing(camera);
//			for (int i = 0; i < entities.physical.circleCount; ++i)
//				entities.physical.circle[i].updateForDrawing(camera);
//			for (int i = 0; i < entities.surface.polygonCount; ++i)
//				entities.surface.polygon[i].updateForDrawing(camera);
//			for (int i = 0; i < entities.surface.circleCount; ++i)
//				entities.surface.circle[i].updateForDrawing(camera);
		}
		
		// draw onto window
		{
			entities->hole->shape::sub::Color::draw();
//			window.render.draw(entities.background);
//			for (int i = 0; i < entities.surface.polygonCount; ++i)
//				window.render.draw(entities.surface.polygon[i]);
//			for (int i = 0; i < entities.surface.circleCount; ++i)
//				window.render.draw(entities.surface.circle[i]);
//			for (int i = 0; i < entities.holeCount; ++i)
//				window.render.draw(entities.hole[i]);
//			for (int i = 0; i < entities.physical.polygonCount; ++i)
//				window.render.draw(entities.physical.polygon[i]);
//			for (int i = 0; i < entities.physical.circleCount; ++i)
//				window.render.draw(entities.physical.circle[i]);
//			window.render.draw(entities.ball);
//			if (entities.arrow.getStatus())
//				window.render.draw(entities.arrow);
			
			window->swapBuffers();
		}
	}
	
	window->setCurrent(false);
}

long long CALLBACK Level::windowProcedure(HWND__ *const windowHandle, unsigned const message, unsigned long long const wParam, long long const lParam) {
	auto close = [] {
		// End render thread - nothing can be rendered after context activation on this thread
		Level::thisPtr->run = false;
		Level::thisPtr->renderThreadHandle->join();
		delete Level::thisPtr->renderThreadHandle;
		
		// Data for later to save to settings.xml
		Level::thisPtr->atWindowDestruction.maximized = Level::thisPtr->window->getState() == Window::State::Maximized;
		Level::thisPtr->atWindowDestruction.size = Level::thisPtr->window->getSize();
		
		Level::thisPtr->window->setCurrent(true);
		
		// Delete all entities while context is active
		delete Level::thisPtr->entities;
		// Delete context and window
		delete Level::thisPtr->window;
		
		return 0;
	};
	
	switch (message) {
		case WM_SIZE: {
			if (wParam == SIZE_MAXIMIZED)
				Level::thisPtr->window->setFocus(true);
			return 0;
		}
		case WM_SHOWWINDOW: {
			if (wParam && !lParam)
				Level::thisPtr->window->setFocus(true);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			Event event {Event::Type::MouseKeyPressed};
			event.mouseKey = Event::MouseKey::Left;
			
			Level::thisPtr->eventQueue.addLast(event);
			return 0;
		}
		case WM_RBUTTONDOWN: {
			Event event {Event::Type::MouseKeyPressed};
			event.mouseKey = Event::MouseKey::Right;
			
			Level::thisPtr->eventQueue.addLast(event);
			return 0;
		}
		case WM_KEYDOWN: {
			Event event {Event::Type::KeyPressed};
			
			switch (wParam) {
				case VK_ESCAPE:
					return close();
				case VK_SPACE:
					event.key = Event::Key::Space;
					break;
				default:
					return DefWindowProcA(windowHandle, message, wParam, lParam);
			}
			
			Level::thisPtr->eventQueue.addLast(event);
			return 0;
		}
		case WM_MOUSEWHEEL: {
			Event event {Event::Type::MouseWheelScrolled};
			// check if high word is negative (has 1 as highest bit)
			event.scroll = static_cast<char>(wParam >> 31) == 0 ? Event::Scroll::Up : Event::Scroll::Down;
			
			Level::thisPtr->eventQueue.addLast(event);
			return 0;
		}
		case WM_SIZING: {
//			tagRECT *const windowRect {reinterpret_cast<tagRECT *>(lParam)};
//
//			Event event {Event::Resized};
//			event.size = {windowRect->left, windowRect->right, windowRect->top, windowRect->bottom};
//
//			Level::thisPtr->eventQueue.addLast(event);
//
//			return true;
			return DefWindowProcA(windowHandle, message, wParam, lParam);
		}
		case WM_CLOSE:
			return close();
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		default:
			return DefWindowProcA(windowHandle, message, wParam, lParam);
	}
}

Point<int> Level::getMousePosition() const {
	tagPOINT yInvertedMousePosition {};
	GetCursorPos(&yInvertedMousePosition);
	
	Point<int> const windowPosition {window->getPosition()};
	
	return {yInvertedMousePosition.x - windowPosition.x, window->screenSize.y - yInvertedMousePosition.y - windowPosition.y};
}
