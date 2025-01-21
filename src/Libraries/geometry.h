#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include "Math/LineFunction.h"
#include "Math/CircleFunction.h"

/// Functions for various geometry calculations
namespace geometry {
	[[nodiscard]] constexpr double toDegrees(double radians);
	[[nodiscard]] constexpr double toRadians(double degrees);
	
	/// \returns Angle in range [0°, limit°)
	/// \example limit(360°, 400°) = 40°
	[[nodiscard]] constexpr double limit(double limit, double angle);
	
	/// Turns angle by 180°
	[[nodiscard]] constexpr double turn(double angle);
	
	/// Order of points don't matter
	/// \returns Absolute value of length between given points
	template<typename T>
	[[nodiscard]] constexpr double length(Point<T> const &p1, Point<T> const &p2);
	
	/// Order of points matter
	/// \returns Angle, where p1 is treated like a starting point of 2D coordinate system
	/// \example angle({0, 0}, {1, 1}) = 45°
	/// \example angle({1, 1}, {0, 0}) = 225°
	template<typename T>
	[[nodiscard]] constexpr double angle(Point<T> const &p1, Point<T> const &p2);
	
	/// \returns Angle of lineFunction, always less than 180°
	[[nodiscard]] constexpr double angle(LineFunction const &lineFunction);
	
	template<typename T>
	[[nodiscard]] constexpr Point<T> point(Point<T> const &point, double length, double angle);
	
	template<typename T>
	[[nodiscard]] constexpr LineFunction lineFunction(Point<T> const &p1, Point<T> const &p2);
	
	template<typename T>
	[[nodiscard]] constexpr LineFunction lineFunction(Point<T> const &point, double direction);
	
	/// Finds crossing point of two linear functions, order of functions don't matter
	/// \returns True if crossing point exists, false if it doesn't
	template<typename T>
	constexpr bool crossingPoint(LineFunction const &f1, LineFunction const &f2, Point<T> &result);
	
	/// Finds crossing points of line and circle functions
	/// \returns True if crossing points exist, false if they don't
	template<typename T>
	constexpr bool crossingPoint(LineFunction const &lineFunction, CircleFunction const &circleFunction, Point<T> &result1, Point<T> &result2);
	
	/// \returns Perpendicular lineFunction passing through point
	template<typename T>
	[[nodiscard]] constexpr LineFunction perpendicularFunction(LineFunction const &lineFunction, Point<T> const &point);
	
	/// Checks, if given point is inside rectangle formed by two passed points, this rectangle is always not rotated
	template<typename T>
	[[nodiscard]] constexpr bool isPointInsideRectangle(Point<T> const &pointToCheck, Point<T> const &p1, Point<T> const &p2);
}


constexpr double geometry::toDegrees(double const radians) { return radians * (180 / M_PI); }

constexpr double geometry::toRadians(double const degrees) { return degrees / (180 / M_PI); }

constexpr double geometry::limit(double const limit, double angle) {
	if (angle < 0)
		angle += 360;
	while (angle >= limit)
		angle -= limit;
	return angle;
}

constexpr double geometry::turn(double const angle) { return limit(360, angle - 180); }

template<typename T>
constexpr double geometry::length(Point<T> const &p1, Point<T> const &p2) {
	T const sideX = p1.x - p2.x, sideY = p1.y - p2.y;
	return std::sqrt(sideX * sideX + sideY * sideY);
}

template<typename T>
constexpr double geometry::angle(Point<T> const &p1, Point<T> const &p2) {
	// I   Quarter - 0   <= angle < 90
	// II  Quarter - 90  <= angle < 180
	// III Quarter - 180 <= angle < 270
	// IV  Quarter - 270 <= angle < 360
	double const angle = toDegrees(std::asin((p2.y - p1.y) / length(p1, p2)));
	if (p2.y >= p1.y) {
		if (p2.x > p1.x)
			return angle;
		if (p1.y == p2.y)
			return 180;
		return 180 - angle;
	}
	if (p2.x < p1.x)
		return 180 - angle;
	return 360 + angle;
}

constexpr double geometry::angle(LineFunction const &lineFunction) {
	if (lineFunction.type == LineFunction::Vertical)
		return 90;
	if (lineFunction.type == LineFunction::Horizontal)
		return 0;
	if (lineFunction.a > 0)
		return toDegrees(std::atan(lineFunction.a));
	return 180 + toDegrees(std::atan(lineFunction.a));
}

template<typename T>
constexpr Point<T> geometry::point(Point<T> const &point, double const length, double const angle) {
	double const angleInRadian {geometry::toRadians(angle)};
	return {static_cast<T>(point.x + std::cos(angleInRadian) * length), static_cast<T>(point.y + std::sin(angleInRadian) * length)};
}

template<typename T>
constexpr LineFunction geometry::lineFunction(Point<T> const &p1, Point<T> const &p2) {
	double const divisor {p1.x - p2.x};
	
	if (std::abs(divisor) < 0.0001)
		return {p1.x, 0, LineFunction::Type::Vertical};
	
	LineFunction result;
	
	result.a = (p1.y - p2.y) / divisor;
	result.b = p1.y - result.a * p1.x;
	
	if (std::abs(result.a) < 0.0001) {
		result.a = 0;
		result.type = LineFunction::Type::Horizontal;
	} else
		result.type = LineFunction::Type::Oblique;
	
	return result;
}

template<typename T>
constexpr LineFunction geometry::lineFunction(Point<T> const &point, double direction) {
	direction = geometry::limit(180, direction);
	
	if (89.9999 < direction && direction < 90.0001)
		return {point.x, 0, LineFunction::Type::Vertical};
	
	LineFunction result;
	
	if (179.9999 < direction || direction < 0.0001) {
		result.a = 0;
		result.type = LineFunction::Type::Horizontal;
	} else {
		result.a = std::tan(geometry::toRadians(direction));
		result.type = LineFunction::Type::Oblique;
	}
	
	result.b = point.y - result.a * point.x;
	
	return result;
}

template<typename T>
constexpr bool geometry::crossingPoint(LineFunction const &f1, LineFunction const &f2, Point<T> &result) {
	double const divisor = f1.a - f2.a;
	if (std::abs(divisor) < 0.0001 && f1.type != LineFunction::Vertical && f2.type != LineFunction::Vertical ||
	    f1.type == LineFunction::Vertical && f2.type == LineFunction::Vertical)
		return false;
	if (f1.type == LineFunction::Vertical) {
		result.x = f1.a;
		result.y = f2(result.x);
	} else {
		if (f2.type == LineFunction::Vertical)
			result.x = f2.a;
		else
			result.x = (f2.b - f1.b) / divisor;
		result.y = f1(result.x);
	}
	return true;
}

template<typename T>
constexpr bool geometry::crossingPoint(LineFunction const &lineFunction, CircleFunction const &circleFunction, Point<T> &result1, Point<T> &result2) {
	double const a {lineFunction.a * lineFunction.a + 1};
	double const b {2 * (lineFunction.a * lineFunction.b - circleFunction.a - lineFunction.a * circleFunction.b)};
	double const c {
			lineFunction.b * lineFunction.b - 2 * lineFunction.b * circleFunction.b + circleFunction.b * circleFunction.b + circleFunction.a * circleFunction.a -
			circleFunction.radius * circleFunction.radius};
	
	double const delta {b * b - 4 * a * c};
	if (delta < 0)
		return false;
	double const deltaSqrt {std::sqrt(delta)};
	
	result1.x = (-b - deltaSqrt) / (2 * a);
	result2.x = (-b + deltaSqrt) / (2 * a);
	
	result1.y = lineFunction(result1.x);
	result2.y = lineFunction(result2.x);
	return true;
}

template<typename T>
constexpr LineFunction geometry::perpendicularFunction(LineFunction const &lineFunction, Point<T> const &point) {
	LineFunction result;
	if (lineFunction.type == LineFunction::Vertical) {
		result.a = 0;
		result.b = point.y;
		result.type = LineFunction::Horizontal;
	} else if (lineFunction.type == LineFunction::Horizontal) {
		result.a = point.x;
		result.type = LineFunction::Vertical;
	} else {
		result.a = -1 / lineFunction.a;
		result.b = point.y - result.a * point.x;
		result.type = LineFunction::Oblique;
	}
	return result;
}

template<typename T>
constexpr bool geometry::isPointInsideRectangle(Point<T> const &pointToCheck, Point<T> const &p1, Point<T> const &p2) {
	T minX, maxX, minY, maxY;
	if (p1.x <= p2.x) {
		minX = p1.x;
		maxX = p2.x;
	} else {
		minX = p2.x;
		maxX = p1.x;
	}
	if (p1.y <= p2.y) {
		minY = p1.y;
		maxY = p2.y;
	} else {
		minY = p2.y;
		maxY = p1.y;
	}
	return minX <= pointToCheck.x && pointToCheck.x <= maxX && minY <= pointToCheck.y && pointToCheck.y <= maxY;
}

#endif // GEOMETRY_H
