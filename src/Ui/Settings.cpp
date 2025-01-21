#include "Settings.h"
#include <QResizeEvent>
#include "MainMenu.h"

Settings::Settings(MainMenu *const parent) : ResizableQWidget(parent), ui() {
	ui.setupUi(this);
	
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	
	QObject::connect(parent, &MainMenu::resizedSignal, this, &Settings::resized);
	QObject::connect(ui.closePushButton, &QPushButton::clicked, this, &Settings::closePushButton_clicked);
	QObject::connect(ui.savePushButton, &QPushButton::clicked, this, &Settings::savePushButton_clicked);
	QObject::connect(ui.setToDefaultPushButton, &QPushButton::clicked, this, &Settings::setToDefaultPushButton_clicked);
	
	ui.closePushButton->setIcon(QIcon {R"(D:\Dokumenty\Projekty\Moje\Golf\Resources\Icons\arrow_icon.png)"});
	
	widgetPosition.addWidget(*ui.closePushButton, -1, WidgetPosition::DISABLE);
	
	xmlSettings.load();
	if (error) {
		std::string errorDescriptionCopy {std::move(errorDescription)};
		error = false;
		
		xmlSettings.setDefault();
		xmlSettings.save();
		
		updateWidgetsWithSettingsXmlData();
		
		errorDescription = errorDescriptionCopy + (error ? " Couldn't apply default settings." : " Default settings applied.");
		
		messageBox.setText(errorDescription.c_str());
		messageBox.show();
		
		error = false;
		errorDescription.clear();
	} else
		updateWidgetsWithSettingsXmlData();
	
	show();
}

void Settings::updateWidgetsWithSettingsXmlData() {
	ui.antialiasingComboBox->setCurrentIndex([this] {
		switch (xmlSettings.window.antialiasing) {
			case 2:
				return 1;
			case 4:
				return 2;
			case 8:
				return 3;
			case 16:
				return 4;
			default:
				return 0;
		}
	}());
	
	ui.manualMoveSpeedSpinBox->setValue(xmlSettings.camera.moveSpeed);
	
	ui.horizontalSpinBox->setValue(xmlSettings.camera.margin.x);
	ui.verticalSpinBox->setValue(xmlSettings.camera.margin.y);
}

void Settings::closePushButton_clicked() { close(); }

void Settings::savePushButton_clicked() {
	xmlSettings.window.antialiasing = [this] {
		switch (ui.antialiasingComboBox->currentIndex()) {
			case 1:
				return 2;
			case 2:
				return 4;
			case 3:
				return 8;
			case 4:
				return 16;
			default:
				return 0;
		}
	}();
	
	xmlSettings.camera.moveSpeed = ui.manualMoveSpeedSpinBox->value();
	
	xmlSettings.camera.margin.x = ui.horizontalSpinBox->value();
	xmlSettings.camera.margin.y = ui.verticalSpinBox->value();
	
	xmlSettings.save();
	if (error) {
		messageBox.setText(errorDescription.c_str());
		messageBox.show();
		error = false;
		errorDescription.clear();
	}
}

void Settings::setToDefaultPushButton_clicked() {
	xmlSettings.setDefault();
	updateWidgetsWithSettingsXmlData();
}
