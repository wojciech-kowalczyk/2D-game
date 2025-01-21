#ifndef LEVELCREATORLAUNCHER_H
#define LEVELCREATORLAUNCHER_H

#include "ResizableQWidget.h"
#include "ui_LevelCreatorLauncher.h"
#include <QMessageBox>

class MainMenu;

class LevelCreatorLauncher : public ResizableQWidget {
public:
	explicit LevelCreatorLauncher(MainMenu *parent);
private:
	Ui::LevelCreatorLauncher ui;
	QMessageBox messageBox;
private slots:
	void closePushButton_clicked();
	void launchLevelPushButton_clicked();
	void createLevelPushButton_clicked();
	void levelList_itemClicked();
	void newLevelNameLineEdit_textEdited(QString const &string);
	void levelCreatorCloseSlot();
};

#endif // LEVELCREATORLAUNCHER_H
