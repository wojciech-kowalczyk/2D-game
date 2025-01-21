#ifndef BALL_H
#define BALL_H

#include "Libraries/Clock.h"
#include "Shapes/Specialized.h"

/// Physical circle with ability to move based on added force, met obstacles and surface underneath
class Ball : public shape::circle:: {
public:
	/// \returns 0 usually; 1 if collision occurred; 2 if ball fell in hole
	int move(shape::PhysicalPolygon const *physicalPolygon, int physicalPolygonCount, shape::PhysicalCircle const *physicalCircle, int physicalCircleCount,
	         shape::SurfacePolygon const *surfacePolygon, int surfacePolygonCount, shape::SurfaceCircle const *surfaceCircle, int surfaceCircleCount,
	         shape::Circle const *hole, int holeCount);
	void addForce(double force, double direction);
	
	[[nodiscard]] bool isMoving() const;
private:
	Clock clock;
};

#endif // BALL_H
