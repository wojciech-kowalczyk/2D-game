#ifndef CIRCLEFUNCTION_H
#define CIRCLEFUNCTION_H

#include "Point.h"

class CircleFunction {
public:
	constexpr CircleFunction();
	template<typename T>
	constexpr CircleFunction(double radius, Point<T> const &position);
	
	double a, b, radius;
};


constexpr CircleFunction::CircleFunction() : a(0), b(0), radius(0) {}

template<typename T>
constexpr CircleFunction::CircleFunction(double const radius, Point<T> const &position) : a(position.x), b(position.y), radius(radius) {}

#endif // CIRCLEFUNCTION_H
