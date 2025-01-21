#ifndef SHAPE_RECTANGLE_H
#define SHAPE_RECTANGLE_H

#include "Sub/Base.h"

namespace shape {
	class Rectangle : public sub::Base {
	public:
		Rectangle() = default;
		Rectangle(Point<double> const &position, Size<double> const &size);
		void create(Point<double> const &position, Size<double> const &size);
		
		void setPosition(Point<double> const &position);
		
		[[nodiscard]] Point<double> const &getPosition() const;
		
		void rotate(double rotation);
	private:
		Point<double> position;
	};
}


#endif // SHAPE_RECTANGLE_H
