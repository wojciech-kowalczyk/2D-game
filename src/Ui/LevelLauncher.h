#ifndef LEVELLAUNCHER_H
#define LEVELLAUNCHER_H

#include "ResizableQWidget.h"
#include "ui_LevelLauncher.h"
#include <QMessageBox>

class MainMenu;

class LevelLauncher : public ResizableQWidget {
public:
	explicit LevelLauncher(MainMenu *parent);
private:
	Ui::LevelLauncher ui;
	QMessageBox messageBox;
private slots:
	void closePushButton_clicked();
	void launchLevelPushButton_clicked();
	void levelList_itemClicked();
};

#endif // LEVELLAUNCHER_H
