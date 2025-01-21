#include "LevelCreatorLauncher.h"
#include "MainMenu.h"
//#include "LevelCreator.h"
#include "Xml/Level.h"

LevelCreatorLauncher::LevelCreatorLauncher(MainMenu *const parent) : ResizableQWidget(parent), ui() {
	ui.setupUi(this);
	
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	
	QObject::connect(parent, &MainMenu::resizedSignal, this, &LevelCreatorLauncher::resized);
	QObject::connect(ui.closePushButton, &QPushButton::clicked, this, &LevelCreatorLauncher::closePushButton_clicked);
	QObject::connect(ui.launchLevelPushButton, &QPushButton::clicked, this, &LevelCreatorLauncher::launchLevelPushButton_clicked);
	QObject::connect(ui.createLevelPushButton, &QPushButton::clicked, this, &LevelCreatorLauncher::createLevelPushButton_clicked);
	QObject::connect(ui.levelList, &QListWidget::itemClicked, this, &LevelCreatorLauncher::levelList_itemClicked);
	QObject::connect(ui.newLevelNameLineEdit, &QLineEdit::textEdited, this, &LevelCreatorLauncher::newLevelNameLineEdit_textEdited);
	
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

void LevelCreatorLauncher::closePushButton_clicked() { close(); }

void LevelCreatorLauncher::launchLevelPushButton_clicked() {
	parentWidget()->hide();
	
//	LevelCreator const *const levelCreator {new LevelCreator {ui.levelList->selectedItems()[0]->text().toStdString(), false}};
//
//	QObject::connect(levelCreator, &LevelCreator::closeSignal, this, &LevelCreatorLauncher::levelCreatorCloseSlot);
}

void LevelCreatorLauncher::createLevelPushButton_clicked() {
	parentWidget()->hide();
	
//	LevelCreator const *const levelCreator {new LevelCreator {ui.newLevelNameLineEdit->text().toStdString(), true}};
//
//	QObject::connect(levelCreator, &LevelCreator::closeSignal, this, &LevelCreatorLauncher::levelCreatorCloseSlot);
}

void LevelCreatorLauncher::levelList_itemClicked() { ui.launchLevelPushButton->setEnabled(true); }

void LevelCreatorLauncher::newLevelNameLineEdit_textEdited(QString const &string) { ui.createLevelPushButton->setEnabled(!string.isEmpty()); }

void LevelCreatorLauncher::levelCreatorCloseSlot() {
	parentWidget()->show();
	
	if (!errorDescription.empty()) {
		messageBox.setText(errorDescription.c_str());
		messageBox.show();
	}
}
