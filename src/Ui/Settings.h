#ifndef SETTINGS_H
#define SETTINGS_H

#include "ResizableQWidget.h"
#include "ui_Settings.h"
#include <QMessageBox>
#include "Xml/Settings.h"

class MainMenu;

class Settings : public ResizableQWidget {
public:
	explicit Settings(MainMenu *parent);
private:
	void updateWidgetsWithSettingsXmlData();
	
	Ui::Settings ui;
	QMessageBox messageBox;
	xml::Settings xmlSettings;
private slots:
	void closePushButton_clicked();
	void savePushButton_clicked();
	void setToDefaultPushButton_clicked();
};

#endif // SETTINGS_H
