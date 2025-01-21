#include "Velocity.h"

shape::sub::Velocity::Velocity() : velocity(0), direction(0) {}

void shape::sub::Velocity::setVelocity(double const velocityInit) { velocity = velocityInit; }

void shape::sub::Velocity::setDirection(double const directionInit) { direction = directionInit; }

double shape::sub::Velocity::getVelocity() const { return velocity; }

double shape::sub::Velocity::getDirection() const { return direction; }
