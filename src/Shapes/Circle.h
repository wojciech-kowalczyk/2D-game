#ifndef SHAPE_CIRCLE_H
#define SHAPE_CIRCLE_H

#include "Sub/Base.h"

/// After changing any property that has visual impact, update() method must be called for made changes to take effect
namespace shape {
	/// Circle that is actually a polygon with many vertices, can be used to make equilateral shapes, 3 points = equilateral triangle, 4 = square, ...
	class Circle : virtual public sub::Base {
	public:
		Circle();
		Circle(Point<double> const &position, double radius, int vertexCount = 40);
		void create(Point<double> const &position, double radius, int vertexCount = 40);
		
		void setPosition(Point<double> const &position);
		
		[[nodiscard]] double getRadius() const;
		[[nodiscard]] Point<double> const &getPosition() const;
	private:
		double radius;
		Point<double> position;
	};
}

#endif // SHAPE_CIRCLE_H
