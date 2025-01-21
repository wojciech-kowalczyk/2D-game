#ifndef SHAPE_SUB_VELOCITY_H
#define SHAPE_SUB_VELOCITY_H

namespace shape::sub {
	class Velocity {
	public:
		Velocity();
		virtual ~Velocity() = default;
		
		void setVelocity(double velocity);
		void setDirection(double direction);
		
		[[nodiscard]] double getVelocity() const;
		[[nodiscard]] double getDirection() const;
	private:
		double velocity, direction;
	};
}

#endif // SHAPE_SUB_VELOCITY_H
