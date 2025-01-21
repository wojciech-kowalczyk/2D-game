#ifndef SHAPE_SUB_SPRINGINESS_H
#define SHAPE_SUB_SPRINGINESS_H

namespace shape::sub {
	class Springiness {
	public:
		Springiness();
		virtual ~Springiness() = default;
		
		void setSpringiness(double springiness);
		
		[[nodiscard]] double getSpringiness() const;
	private:
		double springiness;
	};
}

#endif // SHAPE_SUB_SPRINGINESS_H
