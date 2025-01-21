#include "Camera.h"
#include "Libraries/geometry.h"

Camera::Camera() : scale(1), manualMoveSpeed(3), autoMoveStatus(On) {}

void Camera::setScale(double const set, Point<int> const &mousePosition) {
	// TODO experimental, check V6 backup for last "working" version
	shift.x += mousePosition.x;
	shift.y += mousePosition.y;
	
	scale = set;
	autoMoveStatus = Off;
}

void Camera::setManualMoveSpeed(double const set) { manualMoveSpeed = set; }

void Camera::setShift(Shift const &set) { shift = set; }

void Camera::setMargin(Margin const &set) {
	margin.x = set.x;
	margin.y = set.y;
}

void Camera::setAutoMoveStatus(Status set) { autoMoveStatus = set; }

double Camera::getScale() const { return scale; }

Camera::Shift const &Camera::getShift() const { return shift; }

Status Camera::getAutoMoveStatus() const { return autoMoveStatus; }

void Camera::autoMove(Point<int> const &pointToKeepInWindow, Size<int> const &windowSize) {
	Point const pointRelativeToWindow {pointToKeepInWindow.x * scale - shift.x, pointToKeepInWindow.y * scale - shift.y};
	
	if (pointRelativeToWindow.x > windowSize.x - margin.x)
		shift.x += pointRelativeToWindow.x - windowSize.x + margin.x;
	else if (pointRelativeToWindow.x < margin.x)
		shift.x += pointRelativeToWindow.x - margin.x;
	
	if (pointRelativeToWindow.y > windowSize.y - margin.y)
		shift.y += pointRelativeToWindow.y - windowSize.y + margin.y;
	else if (pointRelativeToWindow.y < margin.y)
		shift.y += pointRelativeToWindow.y - margin.y;
}

void Camera::manualMove(double const direction, double const time) {
	shift = geometry::point(shift, manualMoveSpeed * time, direction);
	autoMoveStatus = Off;
}
