#include "LevelCreator.h"
#include <QKeyEvent>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Libraries/utility.h"
#include "Xml/Level.h"
#include "Ui/Level.h"

LevelCreator::LevelCreator(std::string levelName, bool const newLevel) : ui(), name(std::move(levelName)), simulation(false, false, nullptr, Camera {}) {
	ui.setupUi(this);
	
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	
	QObject::connect(ui.launchLevelPushButton, &QPushButton::clicked, this, &LevelCreator::launchLevelPushButton_clicked);
	
	widgetPosition.addWidget(*ui.launchLevelPushButton, 0, WidgetPosition::DISABLE);
	
	// create new level node
	if (newLevel) {
		xml::Level level;
		level.name = name;
		if (!level.save()) {
			errorDescription = level.getErrorDescription();
			close();
		}
	}
	
	if (!simulation.entities.load(name)) {
		errorDescription = simulation.entities.getErrorDescription();
		close();
	}
	simulation.entities.background.setUp({ui.simulation->width(), ui.simulation->height()});
	
	showMaximized();
}

void LevelCreator::keyPressEvent(QKeyEvent *const keyEvent) {
}

void LevelCreator::closeEvent(QCloseEvent *const closeEvent) {
	if (simulation.thread) {
		simulation.endSimulationThread = true;
		simulation.thread->join();
	}
	delete simulation.thread;
	emit closeSignal(errorDescription);
	closeEvent->accept();
}

void LevelCreator::resizeEvent(QResizeEvent *const resizeEvent) {
	/*simulation.stop = true;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1000ms);
	simulation.window.setActive(true);
	ui.simulation->setGeometry(ui.simulation->pos().x(), ui.simulation->pos().y(), resizeEvent->size().width() - 400, resizeEvent->size().height() - 100);
	simulation.window.setPosition({200, 0});
	simulation.window.setSize({static_cast<unsigned>(resizeEvent->size().width() - 400), static_cast<unsigned>(resizeEvent->size().height() - 100)});
	simulation.window.setView({{(resizeEvent->size().width() - 400.f) / 2.f, (resizeEvent->size().height() - 100.f) / 2.f},
	                           {resizeEvent->size().width() - 400.f,         resizeEvent->size().height() - 100.f}});
	simulation.window.setActive(false);
	simulation.stop = false;*/
	resizeEvent->accept();
}

void LevelCreator::wheelEvent(QWheelEvent *const wheelEvent) {
	double const wheelDelta {wheelEvent->angleDelta().y() / 15.};
	
	// prevent stacking multiple inputs from one frame — limit of one wheel scroll per frame
	/*if (simulation.camera.getScale() > 0.4 || wheelDelta > 0)
		simulation.camera.setScale(simulation.camera.getScale() + (wheelDelta > 0 ? 1 : -1) / 6., sf::Mouse::getPosition(simulation.window));*/
}

void LevelCreator::launchLevelPushButton_clicked() {
	if (!simulation.entities.save(name)) {
		messageBox.setText(simulation.entities.getErrorDescription().c_str());
		messageBox.show();
	} else {
		hide();
		
		Level level {name};
		
		if (!level.start()) {
			messageBox.setText(level.getErrorDescription().c_str());
			messageBox.show();
		}
		
		show();
	}
}
