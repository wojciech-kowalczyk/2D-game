#include "Drag.h"

shape::sub::Drag::Drag() : drag(0) {}

void shape::sub::Drag::setDrag(double const dragInit) { drag = dragInit; }

double shape::sub::Drag::getDrag() const { return drag; }
