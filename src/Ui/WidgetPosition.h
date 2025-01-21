#ifndef WIDGETPOSITION_H
#define WIDGETPOSITION_H

#include "Libraries/List.h"

class QWidget;

class WidgetPosition {
public:
	void adjustWidgetsToWindow(int windowWidth, int windowHeight);
	/// If only one margin needs to be set, then the other one should be set to DISABLE
	void addWidget(QWidget &widget, int rightMargin, int bottomMargin);
	
	enum { DISABLE = -100 };
private:
	struct WidgetWithMargin {
		QWidget &widget;
		int rightMargin, bottomMargin;
	};
	
	List<WidgetWithMargin> widgetList;
};

#endif // WIDGETPOSITION_H
