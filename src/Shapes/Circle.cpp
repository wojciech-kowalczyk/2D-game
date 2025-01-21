#include "Circle.h"
#include "Libraries/geometry.h"

shape::Circle::Circle() : radius(0) {}

shape::Circle::Circle(Point<double> const &positionInit, double const radiusInit, int const vertexCount) {
	create(positionInit, radiusInit, vertexCount);
}

void shape::Circle::create(Point<double> const &positionInit, double const radiusInit, int const vertexCount) {
	radius = radiusInit;
	position = positionInit;
	
	setVertexCount(vertexCount);
	vertex = new Vertex[vertexCount];
	
	double const angle {360. / vertexCount};
	
	for (int i = 0; i < vertexCount; ++i)
		vertex[i].position = geometry::point(position, radius, angle * static_cast<double>(i));
}

void shape::Circle::setPosition(Point<double> const &positionInit) {
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].position = geometry::point(positionInit, geometry::length(position, getVertex(i).position), geometry::angle(position, getVertex(i).position));
	position = positionInit;
}

double shape::Circle::getRadius() const { return radius; }

Point<double> const &shape::Circle::getPosition() const { return position; }
