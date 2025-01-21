#ifndef XML_BASE_H
#define XML_BASE_H

#include <string>

#define SetErrorNode(std_string_nodeName) setErrorNode(PATH, std_string_nodeName)
#define SetErrorAttribute(std_string_attributeName) setErrorAttribute(PATH, std_string_attributeName)

#define LoadDocument                                                    \
    pugi::xml_document document;                                        \
    pugi::xml_parse_result const loadResult {document.load_file(PATH)};

#define SaveDocument(returnValue)                                                                   \
    if (!document.save_file(PATH)) {                                                                \
        error = true;                                                                               \
        errorDescription = R"(Error occurred when saving file ")" + std::string {PATH} + R"(".)";   \
        return returnValue;                                                                         \
    }

#define ErrorOnLoadFailure(returnValue)                                                                                             \
    error = true;                                                                                                                   \
    errorDescription = R"(Error occurred when loading file ")" + std::string {PATH} + R"(": )" + loadResult.description() + '.';    \
    errorNodePrefix.clear();                                                                                                        \
    return returnValue;

#define AppendXYNode(nodeName, parent, xml_node_parentNode)                         \
    pugi::xml_node nodeName##Node {xml_node_parentNode.append_child(#nodeName)};    \
    appendValue("x", utility::toString(parent.x).c_str(), nodeName##Node);          \
    appendValue("y", utility::toString(parent.y).c_str(), nodeName##Node);

/// Creates node variable of given name from parent. Returns returnValue if node is invalid. Appends ".[nodeName]" to [errorNodePrefix]
#define GetNode(nodeName, xml_node_parentNode, returnValue)                     \
    pugi::xml_node const nodeName##Node {xml_node_parentNode.child(#nodeName)}; \
    if (!nodeName##Node) {                                                      \
        SetErrorNode(#nodeName);                                                \
        return returnValue;                                                     \
    }                                                                           \
    if (!errorNodePrefix.empty()) errorNodePrefix += '.';                       \
    errorNodePrefix += #nodeName;

/// Creates string variable of given name from parent. Returns returnValue if value is invalid.
#define GetValue(valueName, xml_node_parentNode, returnValue)                           \
    std::string const valueName##Value {xml_node_parentNode.child_value(#valueName)};   \
    if (valueName##Value.empty()) {                                                     \
        SetErrorNode(#valueName);                                                       \
        return returnValue;                                                             \
    }

#define GetXYValue(nodeName, xml_node_parentNode, returnValue)  \
    std::string _lastErrorNodePrefix {errorNodePrefix};         \
    GetNode(nodeName, xml_node_parentNode, returnValue)         \
    GetValue(x, nodeName##Node, returnValue)                    \
    GetValue(y, nodeName##Node, returnValue)                    \
    errorNodePrefix = std::move(_lastErrorNodePrefix);

#define GetAttribute(attributeName, xml_node_node, returnValue)                             \
    std::string attributeName##Attribute {xml_node_node.attribute(#attributeName).value()}; \
    if (attributeName##Attribute.empty()) {                                                 \
        SetErrorAttribute(#attributeName);                                                  \
        return returnValue;                                                                 \
    }                                                                                       \
    errorNodePrefix += ' ';                                                                 \
    errorNodePrefix += #attributeName;                                                      \
    errorNodePrefix += R"(=")";                                                             \
    errorNodePrefix += attributeName##Attribute;                                            \
    errorNodePrefix += '"';

extern bool error;
extern std::string errorDescription;

namespace pugi { class xml_node; }

namespace xml {
	class Base {
	public:
		virtual ~Base() = default;
		
		/// \warning Can generate error
		virtual void save() const = 0;
		/// Sets xml tree to default, doesn't save to file
		virtual void setDefault() = 0;
	protected:
		static void appendValue(std::string const &nodeName, std::string const &value, pugi::xml_node &parentNode);
		
		/// Sets error to true and sets following error description: In file "[PATH]": Node "[errorNodePrefix].[nodeName]" is empty or doesn't exist.
		static void setErrorNode(std::string const &PATH, std::string nodeName);
		/// Sets error to true and sets following error description: In file "[PATH]": Attribute "[attributeName]" in node "[errorNodePrefix]"
		/// is empty or doesn't exist.
		static void setErrorAttribute(std::string const &PATH, std::string attributeName);
		
		static std::string errorNodePrefix;
	};
}

#endif // XML_BASE_H
