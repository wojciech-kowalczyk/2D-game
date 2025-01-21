#include "Entities.h"
#include "Xml/Level.h"

Entities::Entities() : holeCount(0), hole(nullptr) {}

void Entities::load(std::string levelName) {
	xml::Level level;
	
	if (level.load(std::move(levelName))) {
		holeCount = level.optional.holeCount;
		physical.polygonCount = level.optional.physical.polygonCount;
		physical.circleCount = level.optional.physical.circleCount;
		surface.polygonCount = level.optional.surface.polygonCount;
		surface.circleCount = level.optional.surface.circleCount;
		
		unsigned long long const dataSize {
				sizeof(Background) + sizeof(Ball) + sizeof(Arrow) + sizeof(shape::Circle) * holeCount + sizeof(shape::PhysicalPolygon) * physical.polygonCount +
				sizeof(shape::PhysicalCircle) * physical.circleCount + sizeof(shape::SurfacePolygon) * surface.polygonCount +
				sizeof(shape::SurfaceCircle) * surface.circleCount};
		void *const data {new char[dataSize]};
		
		auto loadTextureAndColor = [&shader, this](shape::Base *const shape, auto const &parent) {
			shape.setColor(parent.color);
			
			if (!parent.texture.empty())
				shape->texture = new Texture {parent.texture, shader};
			
			extern bool error;
			if (error)
				errorDescription = shape->texture->getErrorDescription();
		};
		
		// required
		{
			// background
			{
				background = static_cast<Background *>(data);
				loadTextureAndColor(background, level.required.background);
			}
			
			
			
			// ball
			{
				ball.setSpringiness(level.required.ball.springiness);
				ball.setUp(level.required.ball.radius);
				ball.setPosition(level.required.ball.position);
				loadTextureAndColor(ball, level.required.ball);
			}
			
			// arrow
			{
				// head
				{
					arrow.head.height = level.required.arrow.head.height;
					loadTextureAndColor(arrow.head, level.required.arrow.head);
				}
				
				// body
				{
					arrow.body.width = level.required.arrow.body.width;
					loadTextureAndColor(arrow.body, level.required.arrow.body);
				}
			}
		}
		
		// optional
		{
			/// Springiness or drag must be initialized separately
			auto loadPolygon = [&loadTextureAndColor](shape::Polygon &polygon, auto const &parent) {
				polygon.setName(parent.name);
				polygon.setVertex(parent.vertex, parent.vertexCount);
				polygon.setOrigin(parent.position);
				loadTextureAndColor(polygon, parent);
			};
			
			/// Springiness or drag must be initialized separately
			auto loadCircle = [&loadTextureAndColor](shape::Circle &circle, auto const &parent) {
				circle.setName(parent.name);
				circle.setUp(parent.radius);
				circle.setPosition(parent.position);
				loadTextureAndColor(circle, parent);
			};
			
			// holes
			{
				holeCount = level.optional.holeCount;
				hole = new shape::Circle[holeCount];
				
				for (int i = 0; i < holeCount; ++i)
					loadCircle(hole[i], level.optional.hole[i]);
			}
			
			// physical
			{
				// polygons
				{
					physical.polygonCount = level.optional.physical.polygonCount;
					physical.polygon = new shape::PhysicalPolygon[physical.polygonCount];
					
					for (int i = 0; i < physical.polygonCount; ++i) {
						physical.polygon[i].setSpringiness(level.optional.physical.polygon[i].springiness);
						loadPolygon(physical.polygon[i], level.optional.physical.polygon[i]);
					}
				}
				
				// circles
				{
					physical.circleCount = level.optional.physical.circleCount;
					physical.circle = new shape::PhysicalCircle[physical.circleCount];
					
					for (int i = 0; i < physical.circleCount; ++i) {
						physical.circle[i].setSpringiness(level.optional.physical.circle[i].springiness);
						loadCircle(physical.circle[i], level.optional.physical.circle[i]);
					}
				}
			}
			
			// surface
			{
				// polygons
				{
					surface.polygonCount = level.optional.surface.polygonCount;
					surface.polygon = new shape::SurfacePolygon[surface.polygonCount];
					
					for (int i = 0; i < surface.polygonCount; ++i) {
						surface.polygon[i].setDrag(level.optional.surface.polygon[i].drag);
						loadPolygon(surface.polygon[i], level.optional.surface.polygon[i]);
					}
				}
				
				// circles
				{
					surface.circleCount = level.optional.surface.circleCount;
					surface.circle = new shape::SurfaceCircle[surface.circleCount];
					
					for (int i = 0; i < physical.circleCount; ++i) {
						surface.circle[i].setDrag(level.optional.surface.circle[i].drag);
						loadCircle(surface.circle[i], level.optional.surface.circle[i]);
					}
				}
			}
		}
	} else {
		errorDescription = level.getErrorDescription();
		return false;
	}
	
	return true;
}

Entities::~Entities() {
	delete background;
	delete ball;
	delete arrow;
	delete[] hole;
	delete[] physical.circle;
	delete[] physical.polygon;
	delete[] surface.circle;
	delete[] surface.polygon;
}

bool Entities::save(std::string levelName) {
	xml::Level level;
	
	level.name = std::move(levelName);
	
	auto saveTextureOrColor = [](shape::Base const &shape, auto &parent) {
		if (parent.texture.empty())
			parent.color = {static_cast<int>(shape.getColor().r), static_cast<int>(shape.getColor().g), static_cast<int>(shape.getColor().b)};
		else
			parent.texture = shape.getTexturePath();
	};
	
	// required
	{
		// background
		{
			level.required.background.color = {static_cast<int>(background.getColor().r),
			                                   static_cast<int>(background.getColor().g),
			                                   static_cast<int>(background.getColor().b)};
			level.required.background.texture = background.getTexturePath();
		}
		
		// ball
		{
			level.required.ball.springiness = ball.getSpringiness();
			level.required.ball.radius = ball.getRadius();
			level.required.ball.position = ball.getPosition();
			saveTextureOrColor(ball, level.required.ball);
		}
		
		// arrow
		{
			// head
			{
				level.required.arrow.head.height = arrow.head.height;
				saveTextureOrColor(arrow.head, level.required.arrow.head);
			}
			
			// body
			{
				level.required.arrow.body.width = arrow.body.width;
				saveTextureOrColor(arrow.body, level.required.arrow.body);
			}
		}
	}
	
	// optional
	{
		/// Springiness or drag must be initialized separately
		auto savePolygon = [&saveTextureOrColor](shape::Polygon const &polygon, auto &parent) {
			parent.name = polygon.getName();
			
			auto *const vertex = new Point<double>[polygon.getVertexCount()];
			for (int i = 0; i < polygon.getVertexCount(); ++i)
				vertex[i] = polygon.getVertex(i);
			
			parent.vertexCount = polygon.getVertexCount();
			parent.vertex = vertex;
			
			parent.position = polygon.getPosition();
			
			saveTextureOrColor(polygon, parent);
		};
		
		/// Springiness or drag must be initialized separately
		auto saveCircle = [&saveTextureOrColor](shape::Circle const &circle, auto &parent) {
			parent.name = circle.getName();
			parent.radius = circle.getRadius();
			parent.position = circle.getPosition();
			saveTextureOrColor(circle, parent);
		};
		
		// holes
		{
			level.optional.holeCount = holeCount;
			level.optional.hole = new xml::Level::Optional::Hole[holeCount];
			
			for (int i = 0; i < holeCount; ++i)
				saveCircle(hole[i], level.optional.hole[i]);
		}
		
		// physical
		{
			// polygons
			{
				level.optional.physical.polygonCount = physical.polygonCount;
				level.optional.physical.polygon = new xml::Level::Optional::Physical::Polygon[physical.polygonCount];
				
				for (int i = 0; i < physical.polygonCount; ++i) {
					level.optional.physical.polygon[i].springiness = physical.polygon[i].getSpringiness();
					savePolygon(physical.polygon[i], level.optional.physical.polygon[i]);
				}
			}
			
			// circles
			{
				level.optional.physical.circleCount = physical.circleCount;
				level.optional.physical.circle = new xml::Level::Optional::Physical::Circle[physical.circleCount];
				
				for (int i = 0; i < physical.circleCount; ++i) {
					level.optional.physical.circle[i].springiness = physical.circle[i].getSpringiness();
					saveCircle(physical.circle[i], level.optional.physical.circle[i]);
				}
			}
		}
		
		// surface
		{
			// polygons
			{
				level.optional.surface.polygonCount = surface.polygonCount;
				level.optional.surface.polygon = new xml::Level::Optional::Surface::Polygon[surface.polygonCount];
				
				for (int i = 0; i < surface.polygonCount; ++i) {
					level.optional.surface.polygon[i].drag = surface.polygon[i].getDrag();
					savePolygon(surface.polygon[i], level.optional.surface.polygon[i]);
				}
			}
			
			// circles
			{
				level.optional.surface.circleCount = surface.circleCount;
				level.optional.surface.circle = new xml::Level::Optional::Surface::Circle[surface.circleCount];
				
				for (int i = 0; i < surface.circleCount; ++i) {
					level.optional.surface.circle[i].drag = surface.circle[i].getDrag();
					saveCircle(surface.circle[i], level.optional.surface.circle[i]);
				}
			}
		}
	}
	
	if (!level.save()) {
		errorDescription = level.getErrorDescription();
		return false;
	}
	
	return true;
}
