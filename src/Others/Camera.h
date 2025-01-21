#ifndef CAMERA_H
#define CAMERA_H

#include "Status.h"
#include "Math/Point.h"

class Camera {
public:
	using Shift = Point<double>;
	using Margin = Point<int>;
	
	Camera();
	
	void setScale(double set, Point<int> const &mousePosition);
	void setManualMoveSpeed(double set);
	void setShift(Shift const &set);
	void setMargin(Margin const &set);
	void setAutoMoveStatus(Status set);
	
	[[nodiscard]] double getScale() const;
	[[nodiscard]] Shift const &getShift() const;
	[[nodiscard]] Status getAutoMoveStatus() const;
	
	/// Moves camera, that given position is always inside window (margins also can be set)
	void autoMove(Point<int> const &pointToKeepInWindow, Size<int> const &windowSize);
	void manualMove(double direction, double time);
	/// Converts global point to drawing point (used in drawing)
	template<typename T>
	[[nodiscard]] constexpr Point<T> toDrawing(Point<T> const &global) const;
	/// Converts drawing point to global point (used in physics)
	template<typename T>
	[[nodiscard]] constexpr Point<T> toGlobal(Point<T> const &drawing) const;
private:
	double scale, manualMoveSpeed;
	Shift shift;
	Margin margin;
	Status autoMoveStatus;
};

template<typename T>
constexpr Point<T> Camera::toDrawing(Point<T> const &global) const { return {global.x * scale - shift.x, global.y * scale - shift.y}; }

template<typename T>
constexpr Point<T> Camera::toGlobal(Point<T> const &drawing) const { return {drawing.x / scale + shift.x, drawing.y / scale + shift.y}; }

#endif // CAMERA_H
