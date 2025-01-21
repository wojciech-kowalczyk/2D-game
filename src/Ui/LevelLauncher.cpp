#include "LevelLauncher.h"
#include "MainMenu.h"
#include "Level.h"
#include "Xml/Level.h"

LevelLauncher::LevelLauncher(MainMenu *const parent) : ResizableQWidget(parent), ui() {
	ui.setupUi(this);
	
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	
	QObject::connect(parent, &MainMenu::resizedSignal, this, &LevelLauncher::resized);
	QObject::connect(ui.closePushButton, &QPushButton::clicked, this, &LevelLauncher::closePushButton_clicked);
	QObject::connect(ui.launchLevelPushButton, &QPushButton::clicked, this, &LevelLauncher::launchLevelPushButton_clicked);
	QObject::connect(ui.levelList, &QListWidget::itemClicked, this, &LevelLauncher::levelList_itemClicked);
	
	ui.closePushButton->setIcon(QIcon {R"(D:\Dokumenty\Projekty\Moje\Golf\Resources\Icons\arrow_icon.png)"});
	
	widgetPosition.addWidget(*ui.closePushButton, -1, WidgetPosition::DISABLE);
	
	int levelsNamesCount;
	std::string const *const levelsNames{xml::Level::getLevelsNames(levelsNamesCount)};
	
	if (!error) {
		for (int i = 0; i < levelsNamesCount; ++i)
			ui.levelList->addItem(levelsNames[i].c_str());
		delete[] levelsNames;
	} else {
		messageBox.setText(errorDescription.c_str());
		messageBox.show();
	}
	
	show();
}

void LevelLauncher::closePushButton_clicked() { close(); }

void LevelLauncher::launchLevelPushButton_clicked() {
	parentWidget()->hide();
	
	Level level {ui.levelList->selectedItems()[0]->text().toStdString()};
	
	if (error) {
		messageBox.setText(errorDescription.c_str());
		messageBox.show();
	} else
		level.start();
	
	parentWidget()->show();
}

void LevelLauncher::levelList_itemClicked() { ui.launchLevelPushButton->setEnabled(true); }
