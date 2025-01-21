#include "WidgetPosition.h"
#include <QWidget>

void WidgetPosition::adjustWidgetsToWindow(int const windowWidth, int const windowHeight) {
	for (WidgetWithMargin const &widgetWithMargin : widgetList)
		widgetWithMargin.widget.setGeometry(
				widgetWithMargin.rightMargin == DISABLE ? widgetWithMargin.widget.pos().x() : windowWidth - widgetWithMargin.widget.width() -
				                                                                              widgetWithMargin.rightMargin,
				widgetWithMargin.bottomMargin == DISABLE ? widgetWithMargin.widget.pos().y() : windowHeight - widgetWithMargin.widget.height() -
				                                                                               widgetWithMargin.bottomMargin, widgetWithMargin.widget.width(),
				widgetWithMargin.widget.height());
}

void WidgetPosition::addWidget(QWidget &widget, int const rightMargin, int const bottomMargin) {
	widgetList.addLast({widget, rightMargin, bottomMargin});
}
