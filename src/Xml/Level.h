#ifndef XML_LEVEL_H
#define XML_LEVEL_H

#include "Base.h"
#include "config.h"
#include "Graphics/Vertex.h"
#include "Graphics/Color.h"

namespace xml {
	class Level : public Base {
	public:
		~Level() override;
		
		/// \warning Can generate error
		void load(std::string levelName);
		void save() const override;
		void setDefault() override;
		
		/// \warning User must free array
		/// \warning Can generate error
		[[nodiscard]] static std::string *getLevelsNames(int &levelsNamesCount);
		
		std::string name;
		
		struct Background {
			Color color {0.2, 0.2, 0.2};
			std::string texture;
		} background;
		
		struct Ball {
			double springiness {0.8};
			double radius {20};
			Point<double> position {20, 20};
			Color color {1, 1, 1};
			std::string texture;
		} ball;
		
		struct Arrow {
			struct Head {
				double height {25};
				Color color {1, 1, 1};
				std::string texture;
			} head;
			
			struct Body {
				double width {7};
				Color color {1, 1, 1};
				std::string texture;
			} body;
		} arrow;
		
		int holeCount {0};
		struct Hole {
			std::string name;
			
			double radius {0};
			Point<double> position {0, 0};
			Color color {0, 0, 0};
			std::string texture;
		} *hole {nullptr};
		
		struct Physical {
			int polygonCount {0};
			struct Polygon {
				std::string name;
				
				double springiness {0};
				
				int vertexCount {0};
				Vertex *vertex {nullptr};
				
				Point<double> position {0, 0};
				Color color {0, 0, 0};
				std::string texture;
			} *polygon {nullptr};
			
			int circleCount {0};
			struct Circle {
				std::string name;
				
				double springiness {0};
				double radius {0};
				Point<double> position {0, 0};
				Color color {0, 0, 0};
				std::string texture;
			} *circle {nullptr};
		} physical;
		
		struct Surface {
			int polygonCount {0};
			struct Polygon {
				std::string name;
				
				double drag {0};
				
				int vertexCount {0};
				Vertex *vertex {nullptr};
				
				Point<double> position {0, 0};
				Color color {0, 0, 0};
				std::string texture;
			} *polygon {nullptr};
			
			int circleCount {0};
			struct Circle {
				std::string name;
				
				double drag {0};
				double radius {0};
				Point<double> position {0, 0};
				Color color {0, 0, 0};
				std::string texture;
			} *circle {nullptr};
		} surface;
		
		static constexpr char const *PATH {LEVELS_FILE_PATH};
	};
}

#endif // XML_LEVEL_H
