#include "Ball.h"
#include <cmath>
#include "Libraries/geometry.h"
#include "Math/Vector.h"

int Ball::move(shape::PhysicalPolygon const *const physicalPolygon, int const physicalPolygonCount, shape::PhysicalCircle const *const physicalCircle,
               int const physicalCircleCount, shape::SurfacePolygon const *const surfacePolygon, int const surfacePolygonCount,
               shape::SurfaceCircle const *const surfaceCircle, int const surfaceCircleCount, shape::Circle const *const hole, int const holeCount) {
	int returnValue {0}; // see documentation for return codes
	Point<double> const lastPosition {getPosition()};
	Point<double> const pointFarFromMovingObject {geometry::point(lastPosition, 10000, getDirection())};
	LineFunction const movingObjectFunction {geometry::lineFunction(lastPosition, getDirection())};
	double const time {clock.restart().asSeconds()};
	
	double constexpr defaultDrag {80};
	
	// variables for calculating result of move in this frame
	struct {
		double velocity, direction;
		Point<double> position;
	} stateAfterMove {getVelocity() - defaultDrag * time, getDirection(), geometry::point(lastPosition, stateAfterMove.velocity * time, stateAfterMove.direction)};
	
	// drag is sum of surfaces' drags beneath the ball
	double const drag {[&] {
		double result {0};
		
		// polygons
		for (int i = 0; i < surfacePolygonCount; ++i) {
			shape::SurfacePolygon const &surfacePolygonRef {surfacePolygon[i]};
			
			auto crossingPoint = [&](Point<double> &result, int const vertexIndex) {
				Vector const surfaceVector {surfacePolygonRef.getVertex(vertexIndex),
				                            surfacePolygonRef.getVertex(vertexIndex == surfacePolygonRef.getVertexCount() - 1 ? 0 : vertexIndex + 1)};
				LineFunction const surfaceFunction {geometry::lineFunction(surfaceVector.tail, surfaceVector.head)};
				
				Point const potentialCrossingPoint {[&movingObjectFunction, &surfaceFunction] {
					Point<double> result;
					geometry::crossingPoint(movingObjectFunction, surfaceFunction, result);
					return result;
				}()};
				
				double minX, maxX;
				if (surfaceVector.tail.x <= surfaceVector.head.x) {
					minX = surfaceVector.tail.x;
					maxX = surfaceVector.head.x;
				} else {
					minX = surfaceVector.head.x;
					maxX = surfaceVector.tail.x;
				}
				double minY, maxY;
				if (surfaceVector.tail.y <= surfaceVector.head.y) {
					minY = surfaceVector.tail.y;
					maxY = surfaceVector.head.y;
				} else {
					minY = surfaceVector.head.y;
					maxY = surfaceVector.tail.y;
				}
				if (minX - 0.0001 <= potentialCrossingPoint.x && potentialCrossingPoint.x <= maxX + 0.0001 && minY - 0.0001 <= potentialCrossingPoint.y &&
				    potentialCrossingPoint.y <= maxY + 0.0001) {
					result = potentialCrossingPoint;
					return true;
				}
				return false;
			};
			
			Point<double> p1, p2; // finding crossing points of movingObjectFunction and surface polygon
			int j {0};
			while (!crossingPoint(p1, j) && j < surfacePolygonRef.getVertexCount())
				++j;
			++j;
			while (!crossingPoint(p2, j) && j < surfacePolygonRef.getVertexCount())
				++j;
			
			Point<double> constexpr DEFAULT_POINT;
			if (p1 != DEFAULT_POINT && p2 != DEFAULT_POINT &&
			    std::abs(geometry::angle(stateAfterMove.position, p1) - geometry::angle(stateAfterMove.position, p2)) > 0.0001)
				result += surfacePolygonRef.getDrag();
		}
		
		// circles
		for (int i = 0; i < surfaceCircleCount; ++i) {
			shape::SurfaceCircle const &surfaceCircleRef {surfaceCircle[i]};
			if (geometry::length(stateAfterMove.position, surfaceCircleRef.getPosition()) <= surfaceCircleRef.getRadius())
				result += surfaceCircleRef.getDrag();
		}
		
		return result != 0 ? result : defaultDrag;
	}()};
	
	// updating velocity and position because of possible drag change
	stateAfterMove.velocity = getVelocity() - drag * time;
	stateAfterMove.position = geometry::point(lastPosition, stateAfterMove.velocity * time, stateAfterMove.direction);
	
	auto reflectOffLine = [this, &stateAfterMove, &lastPosition, drag, time](double const angleOfReflectingLine, double const reflectionLineSpringiness,
	                                                                         double const availableDistance) {
		stateAfterMove.direction = geometry::limit(360, angleOfReflectingLine * 2 - getDirection());
		stateAfterMove.velocity = getVelocity() * getSpringiness() * reflectionLineSpringiness - drag * time;
		stateAfterMove.position = geometry::point(lastPosition, availableDistance * getSpringiness() * reflectionLineSpringiness, stateAfterMove.direction);
	};
	
	auto reflectOffCircle = [&reflectOffLine, this, &stateAfterMove, &movingObjectFunction, &lastPosition, &pointFarFromMovingObject, time](
			double const circleRadius, Point<double> const &circlePosition, double const circleSpringiness) {
		Point<double> const &crossingPointOfMovingAndPerpendicular {[&movingObjectFunction, &circlePosition] {
			Point<double> result;
			geometry::crossingPoint(geometry::perpendicularFunction(movingObjectFunction, circlePosition), movingObjectFunction, result);
			return result;
		}()};
		
		double const sumOfTwoRadiuses {getRadius() + circleRadius};
		double const alpha {geometry::toDegrees(std::acos(geometry::length(circlePosition, crossingPointOfMovingAndPerpendicular) / sumOfTwoRadiuses))};
		double const perpendicularAngle {geometry::angle(circlePosition, crossingPointOfMovingAndPerpendicular)};
		Point<double> const p1 {geometry::point(circlePosition, sumOfTwoRadiuses, geometry::limit(360, perpendicularAngle + alpha))};
		Point<double> const p2 {geometry::point(circlePosition, sumOfTwoRadiuses, geometry::limit(360, perpendicularAngle - alpha))};
		
		Point<double> const positionAtMomentOfImpact {geometry::length(pointFarFromMovingObject, p1) > geometry::length(pointFarFromMovingObject, p2) ? p1 : p2};
		
		reflectOffLine(geometry::angle(geometry::perpendicularFunction(geometry::lineFunction(positionAtMomentOfImpact, circlePosition), Point<double> {0, 0})),
		               circleSpringiness, stateAfterMove.velocity * time - geometry::length(lastPosition, positionAtMomentOfImpact));
	};
	
	// reflecting off polygons
	for (int i = 0; i < physicalPolygonCount; ++i) {
		shape::PhysicalPolygon const &physicalPolygonRef {physicalPolygon[i]};
		for (int j = 0; j < physicalPolygonRef.getVertexCount(); ++j) {
			Vector const physicalVector {physicalPolygonRef.getVertex(j), physicalPolygonRef.getVertex(j == physicalPolygonRef.getVertexCount() - 1 ? 0 : j + 1)};
			LineFunction const physicalFunction {geometry::lineFunction(physicalVector.tail, physicalVector.head)};
			
			Point const crossingPointOfPhysicalAndPerpendicular {[&physicalFunction, &stateAfterMove] {
				Point<double> result;
				geometry::crossingPoint(physicalFunction, geometry::perpendicularFunction(physicalFunction, stateAfterMove.position), result);
				return result;
			}()};
			
			if (geometry::length(stateAfterMove.position, crossingPointOfPhysicalAndPerpendicular) <= getRadius()) {
				// check, if crossingPointOfPhysicalAndPerpendicular is a part of line segment equal to physicalVector
				double minX, maxX;
				if (physicalVector.tail.x <= physicalVector.head.x) {
					minX = physicalVector.tail.x;
					maxX = physicalVector.head.x;
				} else {
					minX = physicalVector.head.x;
					maxX = physicalVector.tail.x;
				}
				double minY, maxY;
				if (physicalVector.tail.y <= physicalVector.head.y) {
					minY = physicalVector.tail.y;
					maxY = physicalVector.head.y;
				} else {
					minY = physicalVector.head.y;
					maxY = physicalVector.tail.y;
				}
				if (minX - 0.0001 <= crossingPointOfPhysicalAndPerpendicular.x && crossingPointOfPhysicalAndPerpendicular.x <= maxX + 0.0001 &&
				    minY - 0.0001 <= crossingPointOfPhysicalAndPerpendicular.y && crossingPointOfPhysicalAndPerpendicular.y <= maxY + 0.0001) {
					Point<double> const collisionPointOnMoving {[&] {
						return geometry::point(stateAfterMove.position, getRadius(), geometry::length(stateAfterMove.position, pointFarFromMovingObject) >
						                                                             geometry::length(crossingPointOfPhysicalAndPerpendicular,
						                                                                              pointFarFromMovingObject) ? geometry::angle(
								stateAfterMove.position, crossingPointOfPhysicalAndPerpendicular) : geometry::turn(
								geometry::angle(stateAfterMove.position, crossingPointOfPhysicalAndPerpendicular)));
					}()};
					
					Point<double> collisionPointOnPhysical;
					// for very rare occasions, when physicalFunction is parallel to movingFunction and in previous frame collision with "circle" of vertex wasn't
					// detected, because of high speed and/or almost missing vertex (being at a little bit further from collision point than radius length)
					if (geometry::crossingPoint(physicalFunction, geometry::lineFunction(collisionPointOnMoving, getDirection()), collisionPointOnPhysical)) {
						// (collisionPointOnMoving - stateAfterMoveWithoutCollision.position) is offset, because collisionPointOnMoving is absolute position at radius
						// length from stateAfterMoveWithoutCollision.position
						reflectOffLine(physicalVector.direction, physicalPolygonRef.getSpringiness(), stateAfterMove.velocity * time -
						                                                                              geometry::length(lastPosition, collisionPointOnPhysical -
						                                                                                                             (collisionPointOnMoving -
						                                                                                                              stateAfterMove.position)));
						returnValue = 1;
						goto exitAfterReflectionOccurred;
					}
				}
			}
		}
	}
	
	// reflecting off circles
	{
		// polygons' vertices are treated, like there are simplified physical circles
		for (int i = 0; i < physicalPolygonCount; ++i) {
			shape::PhysicalPolygon const &physicalPolygonRef {physicalPolygon[i]};
			for (int j = 0; j < physicalPolygonRef.getVertexCount(); ++j) {
				Point<double> const &vertex {physicalPolygonRef.getVertex(j)};
				if (geometry::length(stateAfterMove.position, vertex) <= getRadius()) {
					double constexpr RADIUS_BIGGER_THAN_0_FOR_CALCULATIONS {1};
					reflectOffCircle(RADIUS_BIGGER_THAN_0_FOR_CALCULATIONS, vertex, physicalPolygonRef.getSpringiness());
					returnValue = 1;
					goto exitAfterReflectionOccurred;
				}
			}
		}
		
		// actual circles
		for (int i = 0; i < physicalCircleCount; ++i) {
			shape::PhysicalCircle const &physicalCircleRef {physicalCircle[i]};
			if (geometry::length(stateAfterMove.position, physicalCircleRef.getPosition()) <= getRadius() + physicalCircleRef.getRadius()) {
				reflectOffCircle(physicalCircleRef.getRadius(), physicalCircleRef.getPosition(), physicalCircleRef.getSpringiness());
				returnValue = 1;
				break;
			}
		}
	}
	
	exitAfterReflectionOccurred:
	
	setPosition(stateAfterMove.position);
	setVelocity(stateAfterMove.velocity);
	setDirection(stateAfterMove.direction);
	
	// interacting with holes
	for (int i = 0; i < holeCount; ++i) {
		double const distanceFromBallCenterToHoleCenter {geometry::length(getPosition(), hole[i].getPosition())};
		if (distanceFromBallCenterToHoleCenter < hole[i].getRadius()) {
			double constexpr MAX_POSSIBLE_VELOCITY_OF_FALLING_IN_HOLE {1};
			if (distanceFromBallCenterToHoleCenter < getRadius() / 2 && getVelocity() * time < MAX_POSSIBLE_VELOCITY_OF_FALLING_IN_HOLE / 2 ||
			    distanceFromBallCenterToHoleCenter < getRadius() / 4 && getVelocity() * time < MAX_POSSIBLE_VELOCITY_OF_FALLING_IN_HOLE) {
				setVelocity({});
				returnValue = 2;
			} else
				addForce(geometry::length(getPosition(), hole[i].getPosition()) / distanceFromBallCenterToHoleCenter,
				         geometry::angle(getPosition(), hole[i].getPosition()));
		}
	}
	
	// complete stop
	if (getVelocity() < 0.1)
		setVelocity({});
	
	return returnValue;
}

void Ball::addForce(double const force, double const direction) {
	Point<double> constexpr P_00 {0, 0};
	Vector const newMoveVector {Vector {P_00, geometry::point(P_00, getVelocity(), getDirection())} + Vector {P_00, geometry::point(P_00, force * 1.3, direction)}};
	
	setVelocity(newMoveVector.length);
	setDirection(newMoveVector.direction);
	
	clock.restart();
}

bool Ball::isMoving() const { return getVelocity() != 0; }
