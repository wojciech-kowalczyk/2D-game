#ifndef SHAPE_SUB_DRAG_H
#define SHAPE_SUB_DRAG_H

namespace shape::sub {
	// For surfaces
	class Drag {
	public:
		Drag();
		virtual ~Drag() = default;
		
		void setDrag(double drag);
		
		[[nodiscard]] double getDrag() const;
	private:
		double drag;
	};
}

#endif // SHAPE_SUB_DRAG_H
