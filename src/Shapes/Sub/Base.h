#ifndef SHAPE_BASE_H
#define SHAPE_BASE_H

#include "Graphics/renderFunctions.h"
#include "Graphics/Drawable.h"
#include "Graphics/Texture.h"
#include "Graphics/Vertex.h"

namespace shape::sub {
	namespace attribute {
		struct Format {
			int location;
			int memberCount;
			int glType;
			bool normalized;
			int stride;
			int offset;
		};
		
		constexpr int POSITION_SIZE {sizeof(Point<float>)};
		constexpr int TEX_COORDS_SIZE {sizeof(Point<float>)};
		constexpr int COLOR_SIZE {sizeof(Color)};
		
		namespace color {
			constexpr int STRIDE {POSITION_SIZE + COLOR_SIZE};
			constexpr Format POSITION_FORMAT {0, 2, GL_FLOAT, false, STRIDE, 0};
			constexpr Format COLOR_FORMAT {1, 4, GL_FLOAT, false, STRIDE, POSITION_SIZE};
			
			constexpr int FORMAT_COUNT {2};
			constexpr Format FORMAT[FORMAT_COUNT] {POSITION_FORMAT, COLOR_FORMAT};
		}
		
		namespace texture {
			constexpr int STRIDE {POSITION_SIZE + TEX_COORDS_SIZE};
			constexpr Format POSITION_FORMAT {0, 2, GL_FLOAT, false, STRIDE, 0};
			constexpr Format TEX_COORDS_FORMAT {1, 2, GL_FLOAT, false, STRIDE, POSITION_SIZE};
			
			constexpr int FORMAT_COUNT {2};
			constexpr Format FORMAT[FORMAT_COUNT] {POSITION_FORMAT, TEX_COORDS_FORMAT};
		}
		
		namespace colorAndTexture {
			constexpr int STRIDE {POSITION_SIZE + TEX_COORDS_SIZE + COLOR_SIZE};
			constexpr Format POSITION_FORMAT {0, 2, GL_FLOAT, false, STRIDE, 0};
			constexpr Format TEX_COORDS_FORMAT {1, 2, GL_FLOAT, false, STRIDE, POSITION_SIZE};
			constexpr Format COLOR_FORMAT {2, 4, GL_FLOAT, false, STRIDE, POSITION_SIZE + TEX_COORDS_SIZE};
			
			constexpr int FORMAT_COUNT {3};
			constexpr Format FORMAT[FORMAT_COUNT] {POSITION_FORMAT, TEX_COORDS_FORMAT, COLOR_FORMAT};
		}
	}
	
	class Base : public Drawable {
	public:
		enum class DrawType { Static = GL_STATIC_DRAW, Dynamic = GL_DYNAMIC_DRAW, Stream = GL_STREAM_DRAW };
		
		~Base() override;
		
		[[nodiscard]] int getVertexCount() const;
		[[nodiscard]] Vertex const &getVertex(int index) const;
		
		void draw() const override;
	protected:
		Base();
		void setAttributeFormat(attribute::Format const *attributeFormat, int attributeFormatCount);
		/// \param attributeData - position and texCoords must be in opengl coords
		void setAttributeData(void const *attributeData, int sizeOfAttributeDataArray, DrawType drawType) const;
		
		void setVertexCount(int vertexCount);
		
		Vertex *vertex;
	private:
		void activate() const;
		
		int vertexCount;
		unsigned vertexArrayId, vertexBufferId;
	};
	
	
	class ColorBase : virtual public Base {
	public:
		void setColor(::Color const &color);
		void setColor(::Color const *color);
		
		[[nodiscard]] ::Color const &getColor(int vertexIndex = 0) const;
	};
	
	class TextureBase : virtual public Base {
	public:
		void setTexture(::Texture texture);
		void setTexture(::Texture texture, Point<double> const *texCoords);
		
		[[nodiscard]] ::Texture const &getTexture() const;
		
		void draw() const override;
	private:
		::Texture texture;
	};
	
	
	class Color : public ColorBase {
	public:
		Color();
		
		void update(Size<int> const &clientSize, DrawType drawType);
	};
	
	class Texture : public TextureBase {
	public:
		Texture();
		
		void update(Size<int> const &clientSize, DrawType drawType, Size<int> const &imageSize);
	};
	
	class TextureAndColor : public TextureBase, public ColorBase {
	public:
		TextureAndColor();
		
		void update(Size<int> const &clientSize, DrawType drawType, Size<int> const &imageSize);
	};
}

#endif // SHAPE_BASE_H
