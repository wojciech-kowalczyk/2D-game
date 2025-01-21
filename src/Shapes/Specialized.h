#ifndef SHAPE_SPECIALIZED_H
#define SHAPE_SPECIALIZED_H

#include "Circle.h"
#include "Rectangle.h"
#include "Shapes/Sub/Velocity.h"
#include "Shapes/Sub/Springiness.h"
#include "Shapes/Sub/Drag.h"

namespace shape {
	namespace polygon {
		namespace color {
			class Base : public shape::sub::Color {};
			class Velocity : public shape::sub::Color, public shape::sub::Velocity {};
			class Springiness : public shape::sub::Color, public shape::sub::Springiness {};
			class VelocitySpringiness : public shape::sub::Color, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public shape::sub::Color, public shape::sub::Drag {};
		}
		
		namespace texture {
			class Base : public shape::sub::Texture {};
			class Velocity : public shape::sub::Texture, public shape::sub::Velocity {};
			class Springiness : public shape::sub::Texture, public shape::sub::Springiness {};
			class VelocitySpringiness : public shape::sub::Texture, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public shape::sub::Texture, public shape::sub::Drag {};
		}
		
		namespace textureAndColor {
			class Base : public shape::sub::TextureAndColor {};
			class Velocity : public shape::sub::TextureAndColor, public shape::sub::Velocity {};
			class Springiness : public shape::sub::TextureAndColor, public shape::sub::Springiness {};
			class VelocitySpringiness : public shape::sub::TextureAndColor, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public shape::sub::TextureAndColor, public shape::sub::Drag {};
		}
	}
	
	namespace circle {
		namespace color {
			class Base : public Circle, public shape::sub::Color {};
			class Velocity : public Circle, public shape::sub::Color, public shape::sub::Velocity {};
			class Springiness : public Circle, public shape::sub::Color, public shape::sub::Springiness {};
			class VelocitySpringiness : public Circle, public shape::sub::Color, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Circle, public shape::sub::Color, public shape::sub::Drag {};
		}
		
		namespace texture {
			class Base : public Circle, public shape::sub::Texture {};
			class Velocity : public Circle, public shape::sub::Texture, public shape::sub::Velocity {};
			class Springiness : public Circle, public shape::sub::Texture, public shape::sub::Springiness {};
			class VelocitySpringiness : public Circle, public shape::sub::Texture, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Circle, public shape::sub::Texture, public shape::sub::Drag {};
		}
		
		namespace textureAndColor {
			class Base : public Circle, public shape::sub::TextureAndColor {};
			class Velocity : public Circle, public shape::sub::TextureAndColor, public shape::sub::Velocity {};
			class Springiness : public Circle, public shape::sub::TextureAndColor, public shape::sub::Springiness {};
			class VelocitySpringiness : public Circle, public shape::sub::TextureAndColor, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Circle, public shape::sub::TextureAndColor, public shape::sub::Drag {};
		}
	}
	
	namespace rectangle {
		namespace color {
			class Base : public Rectangle, public shape::sub::Color {};
			class Velocity : public Rectangle, public shape::sub::Color, public shape::sub::Velocity {};
			class Springiness : public Rectangle, public shape::sub::Color, public shape::sub::Springiness {};
			class VelocitySpringiness : public Rectangle, public shape::sub::Color, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Rectangle, public shape::sub::Color, public shape::sub::Drag {};
		}
		
		namespace texture {
			class Base : public Rectangle, public shape::sub::Texture {};
			class Velocity : public Rectangle, public shape::sub::Texture, public shape::sub::Velocity {};
			class Springiness : public Rectangle, public shape::sub::Texture, public shape::sub::Springiness {};
			class VelocitySpringiness : public Rectangle, public shape::sub::Texture, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Rectangle, public shape::sub::Texture, public shape::sub::Drag {};
		}
		
		namespace textureAndColor {
			class Base : public Rectangle, public shape::sub::TextureAndColor {};
			class Velocity : public Rectangle, public shape::sub::TextureAndColor, public shape::sub::Velocity {};
			class Springiness : public Rectangle, public shape::sub::TextureAndColor, public shape::sub::Springiness {};
			class VelocitySpringiness : public Rectangle, public shape::sub::TextureAndColor, public shape::sub::Velocity, public shape::sub::Springiness {};
			class Drag : public Rectangle, public shape::sub::TextureAndColor, public shape::sub::Drag {};
		}
	}
}

#endif // SHAPE_SPECIALIZED_H
