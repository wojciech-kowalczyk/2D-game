#include "Settings.h"
#include <pugixml.hpp>
#include "Libraries/utility.h"

void xml::Settings::load() {
	LoadDocument
	
	if (loadResult) {
		GetNode(settings, document,)
		
		// window
		{
			GetNode(window, settingsNode,)
			
			GetValue(frameRateLimit, windowNode,)
			GetValue(verticalSync, windowNode,)
			GetValue(antialiasing, windowNode,)
			GetValue(maximize, windowNode,)
			GetXYValue(size, windowNode,)
			
			window.frameRateLimit = utility::toInt(frameRateLimitValue);
			window.verticalSync = toStatus(verticalSyncValue.c_str());
			window.antialiasing = utility::toInt(antialiasingValue);
			window.maximize = toStatus(maximizeValue.c_str());
			window.size = {utility::toInt(xValue), utility::toInt(yValue)};
		}
		
		errorNodePrefix = "settings";
		
		// camera
		{
			GetNode(camera, settingsNode,)
			
			GetValue(moveSpeed, cameraNode,)
			GetXYValue(margin, cameraNode,)
			
			camera.moveSpeed = utility::toInt(moveSpeedValue);
			camera.margin = {utility::toInt(xValue), utility::toInt(yValue)};
		}
		
		errorNodePrefix.clear();
	} else { ErrorOnLoadFailure() }
}

void xml::Settings::save() const {
	pugi::xml_document document;
	pugi::xml_node settingsNode {document.append_child("settings")};
	
	// window
	{
		pugi::xml_node windowNode {settingsNode.append_child("window")};
		
		appendValue("frameRateLimit", utility::toString(window.frameRateLimit), windowNode);
		appendValue("verticalSync", window.verticalSync == On ? "on" : "off", windowNode);
		appendValue("antialiasing", utility::toString(window.antialiasing), windowNode);
		appendValue("maximize", window.maximize == On ? "on" : "off", windowNode);
		
		AppendXYNode(size, window.size, windowNode)
	}
	
	// camera
	{
		pugi::xml_node cameraNode {settingsNode.append_child("camera")};
		
		appendValue("moveSpeed", utility::toString(camera.moveSpeed), cameraNode);
		
		AppendXYNode(margin, camera.margin, cameraNode)
	}
	
	SaveDocument()
}

void xml::Settings::setDefault() {
	window = Window {};
	camera = Camera {};
}
