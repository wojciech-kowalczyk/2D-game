#include "Base.h"
#include <pugixml.hpp>

std::string xml::Base::errorNodePrefix {};

void xml::Base::appendValue(std::string const &nodeName, std::string const &value, pugi::xml_node &parentNode) {
	parentNode.append_child(nodeName.c_str()).append_child(pugi::xml_node_type::node_pcdata).set_value(value.c_str());
}

void xml::Base::setErrorNode(std::string const &PATH, std::string nodeName)  {
	error = true;
	if (errorNodePrefix.empty())
		errorDescription = R"(In file ")" + PATH + R"(": Node ")" + std::move(nodeName) + R"(" is empty or doesn't exist.)";
	else
		errorDescription = R"(In file ")" + PATH + R"(": Node ")" + errorNodePrefix + '.' + std::move(nodeName) + R"(" is empty or doesn't exist.)";
}

void xml::Base::setErrorAttribute(std::string const &PATH, std::string attributeName) {
	error = true;
	errorDescription =
			R"(In file ")" + PATH + R"(": Attribute ")" + std::move(attributeName) + R"(" in node ")" + errorNodePrefix + R"(" is empty or doesn't exist.)";
}
