#ifndef VECTOR_H
#define VECTOR_H

#include "Libraries/geometry.h"

class Vector {
public:
	constexpr Vector();
	constexpr Vector(double length, double direction, Point<double> const &tail, Point<double> const &head);
	constexpr Vector(Point<double> const &tail, Point<double> const &head);
	
	constexpr Vector &operator+=(Vector const &vector);
	constexpr Vector &operator-=(Vector const &vector);
	constexpr Vector &operator*=(double scalar);
	constexpr Vector &operator/=(double scalar);
	[[nodiscard]] constexpr Vector operator+(Vector const &vector) const;
	[[nodiscard]] constexpr Vector operator-(Vector const &vector) const;
	[[nodiscard]] constexpr Vector operator*(double scalar) const;
	[[nodiscard]] constexpr Vector operator/(double scalar) const;
	[[nodiscard]] constexpr Vector operator-() const;
	
	double length, direction;
	Point<double> tail, head;
};


constexpr Vector::Vector() : length(0), direction(0) {}

constexpr Vector::Vector(double const length, double const direction, Point<double> const &tail, Point<double> const &head) : length(length),
                                                                                                                              direction(direction),
                                                                                                                              tail(tail),
                                                                                                                              head(head) {}

constexpr Vector::Vector(Point<double> const &tail, Point<double> const &head) : length(geometry::length(tail, head)),
                                                                                 direction(geometry::angle(tail, head)),
                                                                                 tail(tail),
                                                                                 head(head) {}

constexpr Vector &Vector::operator+=(Vector const &vector) { return *this = Vector {tail, geometry::point(head, vector.length, vector.direction)}; }

constexpr Vector &Vector::operator-=(Vector const &vector) { return *this += -vector; }

constexpr Vector &Vector::operator*=(double const scalar) {
	length *= scalar;
	if (scalar < 0) {
		length = -length;
		direction = geometry::turn(direction);
	}
	head = geometry::point(tail, length, direction);
	return *this;
}

constexpr Vector &Vector::operator/=(double const scalar) { return *this *= (1 / scalar); }

constexpr Vector Vector::operator+(Vector const &vector) const { return Vector {*this} += vector; }

constexpr Vector Vector::operator-(Vector const &vector) const { return Vector {*this} -= vector; }

constexpr Vector Vector::operator*(double const scalar) const { return Vector {*this} *= scalar; }

constexpr Vector Vector::operator/(double const scalar) const { return Vector {*this} /= scalar; }

constexpr Vector Vector::operator-() const { return {length, geometry::turn(direction), head, tail}; }

#endif // VECTOR_H
