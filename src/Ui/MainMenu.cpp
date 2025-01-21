#include "MainMenu.h"
#include <QResizeEvent>
#include "LevelLauncher.h"
#include "LevelCreatorLauncher.h"
#include "Settings.h"

MainMenu::MainMenu() : ui() {
	ui.setupUi(this);
	
	setAttribute(Qt::WidgetAttribute::WA_QuitOnClose);
	
	QObject::connect(ui.levelSelectorPushButton, &QPushButton::clicked, this, &MainMenu::levelLauncherPushButton_clicked);
	QObject::connect(ui.levelCreatorPushButton, &QPushButton::clicked, this, &MainMenu::levelCreatorLauncherPushButton_clicked);
	QObject::connect(ui.settingsPushButton, &QPushButton::clicked, this, &MainMenu::settingsPushButton_clicked);
	
	show();
}

void MainMenu::resizeEvent(QResizeEvent *const resizeEvent) {
	emit resizedSignal(resizeEvent->size().width(), resizeEvent->size().height());
	resizeEvent->accept();
}

void MainMenu::levelLauncherPushButton_clicked() {
	new LevelLauncher {this};
	emit resizedSignal(QWidget::width(), QWidget::height());
}

void MainMenu::levelCreatorLauncherPushButton_clicked() {
	new LevelCreatorLauncher {this};
	emit resizedSignal(QWidget::width(), QWidget::height());
}

void MainMenu::settingsPushButton_clicked() {
	new Settings {this};
	emit resizedSignal(QWidget::width(), QWidget::height());
}
