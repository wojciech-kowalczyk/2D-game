#ifndef RESIZABLEQWIDGET_H
#define RESIZABLEQWIDGET_H

#include <QWidget>
#include "WidgetPosition.h"

class ResizableQWidget : public QWidget {

Q_OBJECT

protected:
	explicit ResizableQWidget(QWidget *parent);
	
	WidgetPosition widgetPosition;
protected slots:
	void resized(int width, int height);
signals:
	void resizedSignal(int width, int height) const;
};

#endif // RESIZABLEQWIDGET_H
