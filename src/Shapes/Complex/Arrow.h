#ifndef ARROW_H
#define ARROW_H

#include "Shapes/Specialized.h"
#include "Others/Status.h"

/// Arrow that points to connected ball
class Arrow {
public:
	Arrow();
	
	void setStatus(Status set);
	
	[[nodiscard]] Status getStatus() const;
	[[nodiscard]] double getHeight() const;
	[[nodiscard]] double getDirection() const;
	
	void draw(shape::Circle const &ball, Point<int> const &mousePosition);
	
	class Head : public shape::sub::Base {
	public:
		double height;
	} head;
	
	class Body : public shape::sub::Base {
	public:
		double width;
	} body;
private:
	Status status;
};

#endif // ARROW_H
