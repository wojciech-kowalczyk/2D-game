#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include "ui_MainMenu.h"

class MainMenu : public QWidget {

Q_OBJECT

public:
	MainMenu();
private:
	void resizeEvent(QResizeEvent *resizeEvent) override;
	
	Ui::MainMenu ui;
private slots:
	void levelLauncherPushButton_clicked();
	void levelCreatorLauncherPushButton_clicked();
	void settingsPushButton_clicked();
signals:
	void resizedSignal(int width, int height) const;
};

#endif // MAINMENU_H
