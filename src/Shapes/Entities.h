#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include "Shapes/Specialized.h"

/// Class that contains all drawable objects available for level
class Entities {
public:
	Entities();
	~Entities();
	
	/// \warning Can produce error
	void load(std::string levelName);
	/// \warning Can produce error
	void save(std::string levelName);
	
	Background *background;
	Ball *ball;
	Arrow *arrow;
	
	int holeCount;
	shape::circle::color::Base *hole;
	
	struct {
		int polygonCount;
		shape::PhysicalPolygon *polygon;
		int circleCount;
		shape::PhysicalCircle *circle;
	} physical;
	
	struct {
		int polygonCount;
		shape::SurfacePolygon *polygon;
		int circleCount;
		shape::SurfaceCircle *circle;
	} surface;
private:
	std::string errorDescription;
};

#endif // ENTITIES_H
