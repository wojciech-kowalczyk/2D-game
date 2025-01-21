#include "EntitiesTemp.h"
#include <pugixml.hpp>
#include "config.h"
#include "Libraries/utility.h"

EntitiesTemp::EntitiesTemp(std::string levelName) {
	pugi::xml_document document;
	document.load_file(xml::LEVELS_FILE_PATH);
	pugi::xml_node const levelNode {document.child("levels").find_child_by_attribute("level", "name", "test")};
	pugi::xml_node const holeNode {levelNode.child("hole")};
	pugi::xml_node const positionNode {holeNode.child("position")};
	pugi::xml_node const colorNode {holeNode.child("color")};
	
	hole = new shape::circle::color::Base;
	hole->create({utility::toDouble(positionNode.child_value("x")), utility::toDouble(positionNode.child_value("y"))},
	             utility::toDouble(holeNode.child_value("radius")));
	hole->setColor(Color {static_cast<float>(utility::toDouble(colorNode.child_value("red"))),
	                      static_cast<float>(utility::toDouble(colorNode.child_value("green"))),
	                      static_cast<float>(utility::toDouble(colorNode.child_value("blue")))});
}

EntitiesTemp::~EntitiesTemp() { delete hole; }
