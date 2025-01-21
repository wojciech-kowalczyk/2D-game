#include "Springiness.h"

shape::sub::Springiness::Springiness() : springiness(0) {}

void shape::sub::Springiness::setSpringiness(double const springinessInit) { springiness = springinessInit; }

double shape::sub::Springiness::getSpringiness() const { return springiness; }
