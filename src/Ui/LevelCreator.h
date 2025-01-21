#ifndef LEVELCREATOR_H
#define LEVELCREATOR_H

#include <QWidget>
#include "ui_LevelCreator.h"
#include <thread>
#include <QMessageBox>
#include "Libraries/List.h"
#include "Others/Camera.h"
#include "Shapes/Entities.h"
#include "WidgetPosition.h"

namespace sf { class RenderWindow; }

class LevelCreator : public QWidget {

Q_OBJECT

public:
	LevelCreator(std::string levelName, bool newLevel);
private:
	void keyPressEvent(QKeyEvent *keyEvent) override;
	void closeEvent(QCloseEvent *closeEvent) override;
	void resizeEvent(QResizeEvent *resizeEvent) override;
	void wheelEvent(QWheelEvent *wheelEvent) override;
	
	Ui::LevelCreator ui;
	QMessageBox messageBox;
	WidgetPosition widgetPosition;
	std::string name, errorDescription;
	
	struct Simulation {
		bool endSimulationThread, stop;
		std::thread *thread;
		Camera camera;
		Entities entities;
	} simulation;
private slots:
	void launchLevelPushButton_clicked();
signals:
	void closeSignal(std::string const &errorDescription) const;
};

#endif // LEVELCREATOR_H
