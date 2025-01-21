#include "Rectangle.h"
#include "Libraries/geometry.h"

shape::Rectangle::Rectangle(Point<double> const &positionInit, Size<double> const &size) { create(positionInit, size); }

void shape::Rectangle::create(Point<double> const &positionInit, Size<double> const &size) {
	// p0         width           p3
	//   —————————————————————————
	//  |                         |
	//  |         origin          | height
	//  |                         |
	//   —————————————————————————
	// p1      rotation = 0°      p2
	
	double const widthOnTwo {size.x / 2};
	double const heightOnTwo {size.y / 2};
	
	setVertexCount(4);
	vertex = new Vertex[4];
	
	vertex[0].position = {-widthOnTwo, heightOnTwo};
	vertex[1].position = {-widthOnTwo, -heightOnTwo};
	vertex[2].position = {widthOnTwo, -heightOnTwo};
	vertex[3].position = {widthOnTwo, heightOnTwo};
}

void shape::Rectangle::setPosition(Point<double> const &positionInit) {
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].position = geometry::point(positionInit, geometry::length(position, getVertex(i).position), geometry::angle(position, getVertex(i).position));
	position = positionInit;
}

Point<double> const &shape::Rectangle::getPosition() const { return position; }

void shape::Rectangle::rotate(double const rotation) {
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].position = geometry::point(getPosition(), geometry::length(getPosition(), vertex[i].position),
		                                     geometry::limit(360, geometry::angle(getPosition(), vertex[i].position) + rotation));
}
