#include "Arrow.h"
#include "Libraries/geometry.h"

Arrow::Arrow() : status(On) {}

void Arrow::setStatus(Status const set) { status = set; }

Status Arrow::getStatus() const { return status; }

double Arrow::getHeight() const { return geometry::length(body.getVertex(0).position, body.getVertex(1).position); }

double Arrow::getDirection() const { return geometry::angle(body.getVertex(1).position, body.getVertex(0).position); }

void Arrow::draw(shape::Circle const &ball, Point<int> const &mousePosition) {
	Point<double> const drawingBallPosition {camera.toDrawing(ball.getPosition())};
	
	double const tailToHeadAngle {geometry::angle(static_cast<Point<double>>(drawingMousePosition), drawingBallPosition)};
	double const drawingBallRadius {ball.getRadius() * camera.getScale()};
	
	// body
	{
		Point<double> const positionForBody {geometry::point(drawingBallPosition, drawingBallRadius + head.height, geometry::turn(tailToHeadAngle))};
		double const bodyHeight {geometry::length(static_cast<Point<double>>(drawingMousePosition), positionForBody)};
		
		// up left
		body.vertex[0].position = static_cast<sf::Vector2<float>>(geometry::point(positionForBody, body.width / 2, geometry::limit(360, tailToHeadAngle + 90)));
		// down left
		body.vertex[1].position = static_cast<sf::Vector2<float>>(geometry::point(static_cast<Point<double>>(body.vertex[0].position), bodyHeight,
		                                                                          geometry::turn(tailToHeadAngle)));
		// down right
		body.vertex[2].position = static_cast<sf::Vector2<float>>(geometry::point(static_cast<Point<double>>(body.vertex[1].position), body.width,
		                                                                          geometry::limit(360, tailToHeadAngle - 90)));
		// up right
		body.vertex[3].position = static_cast<sf::Vector2<float>>(geometry::point(positionForBody, body.width / 2, geometry::limit(360, tailToHeadAngle - 90)));
		
		// texture coordinates, because body height is changing
		if (!body.getTexturePath().empty()) {
			// down left
			body.vertex[1].texCoords.y = static_cast<float>(bodyHeight);
			// down right
			body.vertex[2].texCoords = {static_cast<float>(body.width), static_cast<float>(bodyHeight)};
		}
	}
	
	// head
	{
		Point<double> const positionForHead {geometry::point(drawingBallPosition, drawingBallRadius, geometry::turn(tailToHeadAngle))};
		
		// point sticking to ball
		head.vertex[0].position = static_cast<sf::Vector2<float>>(positionForHead);
		
		double const triangleWallLength {2 * head.height / std::sqrt(3)};
		
		// left
		head.vertex[1].position = static_cast<sf::Vector2<float>>(geometry::point(positionForHead, triangleWallLength, geometry::limit(360, tailToHeadAngle + 150)));
		// right
		head.vertex[2].position = static_cast<sf::Vector2<float>>(geometry::point(positionForHead, triangleWallLength, geometry::limit(360, tailToHeadAngle - 150)));
	}
	
	body.draw();
	head.draw();
}
