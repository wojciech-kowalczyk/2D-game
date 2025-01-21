#include "ResizableQWidget.h"

ResizableQWidget::ResizableQWidget(QWidget *const parent) : QWidget(parent) {}

void ResizableQWidget::resized(int width, int height) {
	resize(width, height);
	widgetPosition.adjustWidgetsToWindow(width, height);
	emit resizedSignal(width, height);
}
