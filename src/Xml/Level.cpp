#include "Level.h"
#include <pugixml.hpp>
#include "Libraries/utility.h"

xml::Level::~Level() {
	delete[] hole;
	
	for (int i = 0; i < physical.polygonCount; ++i)
		delete[] physical.polygon[i].vertex;
	delete[] physical.polygon;
	
	delete[] physical.circle;
	
	for (int i = 0; i < surface.polygonCount; ++i)
		delete[] surface.polygon[i].vertex;
	delete[] surface.polygon;
	
	delete[] surface.circle;
}

void xml::Level::load(std::string levelName) {
	name = std::move(levelName);
	
	LoadDocument
	
	if (loadResult) {
		std::string basicErrorNodePrefix {R"(level name=")" + name + '"'};
		
		pugi::xml_node const levelNode {document.child("levels").find_child_by_attribute("level", "name", name.c_str())};
		if (!levelNode) {
			SetErrorNode(basicErrorNodePrefix);
			return;
		}
		
		errorNodePrefix = basicErrorNodePrefix;
		
		/// Loads color only if texture node is empty or doesn't exists
		auto loadTextureOrColor = [](auto &parent, pugi::xml_node const &parentNode) {
			GetValue(texture, parentNode, false)
			
			if (textureValue.empty()) {
				std::string lastErrorNodePrefix {errorNodePrefix};
				
				GetNode(color, parentNode, false)
				
				GetValue(red, colorNode, false)
				GetValue(green, colorNode, false)
				GetValue(blue, colorNode, false)
				GetValue(alpha, colorNode, false)
				
				errorNodePrefix = std::move(lastErrorNodePrefix);
				
				parent.color = {static_cast<float>(utility::toDouble(redValue)), static_cast<float>(utility::toDouble(greenValue)), static_cast<float>
				(utility::toDouble(blueValue)), static_cast<float>(utility::toDouble(alphaValue))};
			} else
				parent.texture = textureValue;
			
			return true;
		};
		
		// background
		{
			GetNode(background, levelNode,)
			
			if (!loadTextureOrColor(background, backgroundNode))
				return;
		}
		
		errorNodePrefix = basicErrorNodePrefix;
		
		// ball
		{
			GetNode(ball, levelNode,)
			
			GetValue(springiness, ballNode,)
			GetValue(radius, ballNode,)
			GetXYValue(position, ballNode,)
			
			if (!loadTextureOrColor(ball, ballNode))
				return;
			
			ball.springiness = utility::toDouble(springinessValue);
			ball.radius = utility::toDouble(radiusValue);
			ball.position = {utility::toDouble(xValue), utility::toDouble(yValue)};
		}
		
		errorNodePrefix = basicErrorNodePrefix;
		
		// arrow
		{
			GetNode(arrow, levelNode,)
			
			std::string arrowErrorNodePrefix {errorNodePrefix};
			
			// head
			{
				GetNode(head, arrowNode,)
				
				GetValue(height, arrowNode,)
				
				if (!loadTextureOrColor(arrow.head, headNode))
					return;
				
				arrow.head.height = utility::toDouble(heightValue);
			}
			
			errorNodePrefix = std::move(arrowErrorNodePrefix);
			
			// body
			{
				GetNode(body, arrowNode,)
				
				GetValue(width, arrowNode,)
				
				if (!loadTextureOrColor(arrow.body, bodyNode))
					return;
				
				arrow.body.width = utility::toDouble(widthValue);
			}
		}
		
		// holes
		{
			constexpr char holeString[] {"hole"}; // to not pass temporary
			pugi::xml_object_range<pugi::xml_named_node_iterator> const holeNodes {levelNode.children(holeString)};
			
			holeCount = static_cast<int>(std::distance(holeNodes.begin(), holeNodes.end()));
			hole = new Hole[holeCount];
			
			int i {0};
			for (pugi::xml_node const &holeNode : holeNodes) {
				errorNodePrefix = basicErrorNodePrefix + ".hole";
				
				GetAttribute(name, holeNode,)
				
				GetValue(radius, holeNode,)
				GetXYValue(position, holeNode,)
				
				if (!loadTextureOrColor(hole[i], holeNode))
					return;
				
				hole[i].name = std::move(nameAttribute);
				hole[i].radius = utility::toDouble(radiusValue);
				hole[i].position = {utility::toDouble(xValue), utility::toDouble(yValue)};
				
				++i;
			}
		}
		
		errorNodePrefix = basicErrorNodePrefix;
		
		/// Drag or springiness must be loaded separately. Appends name attribute to errorNodePrefix
		auto loadPolygon = [&loadTextureOrColor](auto &polygon, pugi::xml_node const &polygonNode) {
			GetAttribute(name, polygonNode, false)
			
			if (!loadTextureOrColor(polygon, polygonNode))
				return false;
			
			// load vertices
			{
				constexpr char vertexString[] {"vertex"}; // to not pass temporary
				pugi::xml_object_range<pugi::xml_named_node_iterator> const vertexNodes {polygonNode.children(vertexString)};
				
				polygon.vertexCount = static_cast<int>(std::distance(vertexNodes.begin(), vertexNodes.end()));
				
				if (polygon.vertexCount < 3) {
					error = true;
					errorDescription =
							R"(In file ")" + std::string {PATH} + R"(": Node ")" + errorNodePrefix + R"("" doesn't contain enough vertices (minimum is 3).)";
					return false;
				}
				
				polygon.vertex = new Vertex[polygon.vertexCount];
				
				std::string lastErrorNodePrefix {errorNodePrefix};
				errorNodePrefix += ".vertex";
				
				int i {0};
				for (pugi::xml_node const &vertexNode : vertexNodes) {
					GetXYValue(position, vertexNode, false)
					
					polygon.vertex[i++].position = {utility::toDouble(xValue), utility::toDouble(yValue)};
				}
				
				errorNodePrefix = std::move(lastErrorNodePrefix);
			}
			
			polygon.name = std::move(nameAttribute);
			
			return true;
		};
		
		/// Drag or springiness must be loaded separately
		auto loadCircle = [&loadTextureOrColor](auto &circle, pugi::xml_node const &circleNode) {
			GetAttribute(name, circleNode, false)
			
			if (!loadTextureOrColor(circle, circleNode))
				return false;
			
			GetValue(radius, circleNode, false)
			GetXYValue(position, circleNode, false)
			
			circle.name = std::move(nameAttribute);
			circle.radius = utility::toDouble(radiusValue);
			circle.position = {utility::toDouble(xValue), utility::toDouble(yValue)};
			
			return true;
		};
		
		constexpr char polygonString[] {"polygon"}; // to not pass temporary
		constexpr char circleString[] {"circle"}; // to not pass temporary
		
		// physical
		{
			GetNode(physical, levelNode,)
			
			// polygons
			{
				pugi::xml_object_range<pugi::xml_named_node_iterator> const polygonNodes {physicalNode.children(polygonString)};
				
				physical.polygonCount = static_cast<int>(std::distance(polygonNodes.begin(), polygonNodes.end()));
				physical.polygon = new Physical::Polygon[physical.polygonCount];
				
				int i {0};
				for (pugi::xml_node const &polygonNode : polygonNodes) {
					errorNodePrefix = basicErrorNodePrefix + ".physical.polygon";
					
					if (!loadPolygon(physical.polygon[i], polygonNode))
						return;
					
					GetValue(springiness, polygonNode,)
					
					physical.polygon[i++].springiness = utility::toDouble(springinessValue);
				}
			}
			
			// circles
			{
				pugi::xml_object_range<pugi::xml_named_node_iterator> const circleNodes {physicalNode.children(circleString)};
				
				physical.circleCount = static_cast<int>(std::distance(circleNodes.begin(), circleNodes.end()));
				physical.circle = new Physical::Circle[physical.circleCount];
				
				int i {0};
				for (pugi::xml_node const &circleNode : circleNodes) {
					errorNodePrefix = basicErrorNodePrefix + ".physical.circle";
					
					if (!loadCircle(physical.circle[i], circleNode))
						return;
					
					GetValue(springiness, circleNode,)
					
					physical.circle[i++].springiness = utility::toDouble(springinessValue);
				}
			}
		}
		
		// surface
		{
			GetNode(surface, levelNode,)
			
			// polygons
			{
				pugi::xml_object_range<pugi::xml_named_node_iterator> const polygonNodes {surfaceNode.children(polygonString)};
				
				surface.polygonCount = static_cast<int>(std::distance(polygonNodes.begin(), polygonNodes.end()));
				surface.polygon = new Surface::Polygon[surface.polygonCount];
				
				int i {0};
				for (pugi::xml_node const &polygonNode : polygonNodes) {
					errorNodePrefix = basicErrorNodePrefix + ".surface.polygon";
					
					if (!loadPolygon(surface.polygon[i], polygonNode))
						return;
					
					GetValue(drag, polygonNode,)
					
					surface.polygon[i++].drag = utility::toDouble(dragValue);
				}
			}
			
			// circles
			{
				pugi::xml_object_range<pugi::xml_named_node_iterator> const circleNodes {surfaceNode.children(circleString)};
				
				surface.circleCount = static_cast<int>(std::distance(circleNodes.begin(), circleNodes.end()));
				surface.circle = new Surface::Circle[surface.circleCount];
				
				int i {0};
				for (pugi::xml_node const &circleNode : circleNodes) {
					errorNodePrefix = basicErrorNodePrefix + ".surface.circle";
					
					if (!loadCircle(surface.circle[i], circleNode))
						return;
					
					GetValue(drag, circleNode,)
					
					surface.circle[i++].drag = utility::toDouble(dragValue);
				}
			}
		}
	} else { ErrorOnLoadFailure() }
}

void xml::Level::save() const {
	LoadDocument
	
	// Check if original document has root node, if it doesn't — add it
	if (loadResult.status == pugi::xml_parse_status::status_no_document_element || document.empty())
		document.append_child("levels");
	else if (loadResult.status == pugi::xml_parse_status::status_file_not_found) { ErrorOnLoadFailure() }
	
	pugi::xml_node levelsNode {document.child("levels")};
	
	// Remove old level node if it exists
	pugi::xml_node oldLevelNode {levelsNode.find_child_by_attribute("level", "name", name.c_str())};
	if (oldLevelNode)
		levelsNode.remove_child(oldLevelNode);
	
	// Save new level node
	pugi::xml_node levelNode {levelsNode.append_child("level")};
	levelNode.append_attribute("name").set_value(name.c_str());
	
	auto appendTextureAndColor = [](auto const &parent, pugi::xml_node &parentNode) {
		appendValue("texture", parent.texture, parentNode);
		
		pugi::xml_node colorNode {parentNode.append_child("position")};
		
		appendValue("red", utility::toString(parent.color.red), colorNode);
		appendValue("green", utility::toString(parent.color.green), colorNode);
		appendValue("blue", utility::toString(parent.color.blue), colorNode);
		appendValue("alpha", utility::toString(parent.color.alpha), colorNode);
	};
	
	// background
	{
		pugi::xml_node backgroundNode {levelNode.append_child("background")};
		
		appendTextureAndColor(background, backgroundNode);
	}
	
	// ball
	{
		pugi::xml_node ballNode {levelNode.append_child("ball")};
		
		appendValue("springiness", std::to_string(ball.springiness), ballNode);
		appendValue("radius", std::to_string(ball.radius), ballNode);
		
		AppendXYNode(position, ball.position, ballNode)
		
		appendTextureAndColor(ball, ballNode);
	}
	
	// arrow
	{
		pugi::xml_node arrowNode {levelNode.append_child("arrow")};
		
		// head
		{
			pugi::xml_node headNode {arrowNode.append_child("head")};
			
			appendValue("height", std::to_string(arrow.head.height), headNode);
			appendTextureAndColor(arrow.head, headNode);
		}
		
		// body
		{
			pugi::xml_node bodyNode {arrowNode.append_child("body")};
			
			appendValue("width", std::to_string(arrow.body.width), bodyNode);
			appendTextureAndColor(arrow.body, bodyNode);
		}
	}
	
	// holes
	for (int i = 0; i < holeCount; ++i) {
		Hole &holeRef {hole[i]};
		
		pugi::xml_node holeNode {levelNode.append_child("hole")};
		
		holeNode.append_attribute("name").set_value(holeRef.name.c_str());
		
		appendValue("radius", std::to_string(holeRef.radius), holeNode);
		
		AppendXYNode(position, holeRef.position, holeNode)
		
		appendTextureAndColor(holeRef, holeNode);
	}
	
	/// Springiness or drag must be set separately
	auto appendPolygon = [&appendTextureAndColor](auto const &polygon, pugi::xml_node &polygonNode) {
		polygonNode.append_attribute("name").set_value(polygon.name.c_str());
		
		for (int i = 0; i < polygon.vertexCount; ++i) {
			pugi::xml_node vertexNode {polygonNode.append_child("vertex")};
			
			AppendXYNode(position, polygon.vertex[i].position, vertexNode)
		}
		
		AppendXYNode(position, polygon.position, polygonNode)
		appendTextureAndColor(polygon, polygonNode);
	};
	
	/// Springiness or drag must be set separately
	auto appendCircle = [&appendTextureAndColor](auto const &circle, pugi::xml_node &circleNode) {
		circleNode.append_attribute("name").set_value(circle.name.c_str());
		
		appendValue(std::to_string(circle.radius), "radius", circleNode);
		
		AppendXYNode(position, circle.position, circleNode)
		appendTextureAndColor(circle, circleNode);
	};
	
	// physical
	{
		pugi::xml_node physicalNode {levelNode.append_child("physical")};
		
		// polygons
		for (int i = 0; i < physical.polygonCount; ++i) {
			pugi::xml_node polygonNode {physicalNode.append_child("polygon")};
			
			appendValue("springiness", std::to_string(physical.polygon[i].springiness), polygonNode);
			appendPolygon(physical.polygon[i], polygonNode);
		}
		
		// circles
		for (int i = 0; i < physical.circleCount; ++i) {
			pugi::xml_node circleNode {physicalNode.append_child("circle")};
			
			appendValue("springiness", std::to_string(physical.circle[i].springiness), circleNode);
			appendCircle(physical.circle[i], circleNode);
		}
	}
	
	// surface
	{
		pugi::xml_node surfaceNode {levelNode.append_child("surface")};
		
		// polygons
		for (int i = 0; i < surface.polygonCount; ++i) {
			pugi::xml_node polygonNode {surfaceNode.append_child("polygon")};
			
			appendValue("drag", std::to_string(surface.polygon[i].drag), polygonNode);
			appendPolygon(surface.polygon[i], polygonNode);
		}
		
		// circles
		for (int i = 0; i < surface.circleCount; ++i) {
			pugi::xml_node circleNode {surfaceNode.append_child("circle")};
			
			appendValue("drag", std::to_string(surface.circle[i].drag), circleNode);
			appendCircle(surface.circle[i], circleNode);
		}
	}
	
	SaveDocument()
}

void xml::Level::setDefault() {
	xml::Level::~Level();
	background = Background {};
	ball = Ball {};
	arrow = Arrow {};
}

std::string *xml::Level::getLevelsNames(int &levelsNamesCount) {
	LoadDocument
	
	if (loadResult) {
		GetNode(levels, document, nullptr)
		
		char const levelString[] {"level"}; // to not pass temporary
		pugi::xml_object_range<pugi::xml_named_node_iterator> const levelNodes {levelsNode.children(levelString)};
		
		int const levelCount {static_cast<int>(std::distance(levelNodes.begin(), levelNodes.end()))};
		
		if (!levelCount) {
			error = true;
			errorDescription = R"(Error occurred when loading file ")" + std::string {PATH} + R"(": no level nodes found.)";
			errorNodePrefix.clear();
			return nullptr;
		}
		
		auto *const levelNames = new std::string[levelCount];
		
		levelsNamesCount = 0;
		
		for (pugi::xml_node const &levelNode : levelNodes)
			levelNames[levelsNamesCount++] = levelNode.attribute("name").value();
		
		errorNodePrefix.clear();
		
		return levelNames;
	} else { ErrorOnLoadFailure(nullptr) }
}
