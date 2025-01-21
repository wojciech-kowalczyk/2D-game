#include "Base.h"
#include "Libraries/geometry.h"

// Base

shape::sub::Base::Base() : vertexCount(0), vertexArrayId(0), vertexBufferId(0), vertex(nullptr) {}

shape::sub::Base::~Base() {
	delete[] vertex;
	renderFunctions::vertexArray::glDeleteVertexArrays(1, &vertexArrayId);
	renderFunctions::buffer::glDeleteBuffers(1, &vertexBufferId);
}

int shape::sub::Base::getVertexCount() const { return vertexCount; }

Vertex const &shape::sub::Base::getVertex(int const index) const { return vertex[index]; }

void shape::sub::Base::draw() const {
	activate();
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

void shape::sub::Base::setAttributeFormat(attribute::Format const *const attributeFormat, int const attributeFormatCount) {
	renderFunctions::vertexArray::glGenVertexArrays(1, &vertexArrayId);
	activate();

	renderFunctions::buffer::glGenBuffers(1, &vertexBufferId);
	renderFunctions::buffer::glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	
	for (int i = 0; i < attributeFormatCount; ++i) {
		renderFunctions::vertexArray::glVertexAttribPointer(attributeFormat[i].location, attributeFormat[i].memberCount, attributeFormat[i].glType,
		                                                    attributeFormat[i].normalized, attributeFormat[i].stride,
		                                                    reinterpret_cast<void *>(attributeFormat[i].offset));
		renderFunctions::vertexArray::glEnableVertexAttribArray(attributeFormat[i].location);
	}
}

void shape::sub::Base::setAttributeData(void const *const attributeData, int const sizeOfAttributeDataArray, DrawType const drawType) const {
	renderFunctions::buffer::glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(sizeOfAttributeDataArray), attributeData, static_cast<int>(drawType));
}

void shape::sub::Base::setVertexCount(int const vertexCountInit) { vertexCount = vertexCountInit; }

void shape::sub::Base::activate() const { renderFunctions::vertexArray::glBindVertexArray(vertexArrayId); }

// ColorBase

void shape::sub::ColorBase::setColor(::Color const &color) {
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].color = color;
}

void shape::sub::ColorBase::setColor(::Color const *const color) {
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].color = color[i];
}

::Color const &shape::sub::ColorBase::getColor(int const vertexIndex) const { return getVertex(vertexIndex).color; }

// TextureBase

void shape::sub::TextureBase::setTexture(::Texture textureInit) { texture = std::move(textureInit); }

void shape::sub::TextureBase::setTexture(::Texture textureInit, Point<double> const *const texCoords) {
	setTexture(std::move(textureInit));
	for (int i = 0; i < getVertexCount(); ++i)
		vertex[i].texCoords = texCoords[i];
}

::Texture const &shape::sub::TextureBase::getTexture() const { return texture; }

void shape::sub::TextureBase::draw() const {
	texture.activate();
	Base::draw();
}

// Color

shape::sub::Color::Color() { setAttributeFormat(attribute::color::FORMAT, attribute::color::FORMAT_COUNT); }

void shape::sub::Color::update(Size<int> const &clientSize, DrawType const drawType) {
	struct AttributeData {
		Point<float> position;
		::Color color;
	} attributeData[getVertexCount()];
	
	for (int i = 0; i < getVertexCount(); ++i) {
		attributeData[i].position = vertex[i].position.toOpenGlCoords(clientSize);
		attributeData[i].color = vertex[i].color;
	}
	
	setAttributeData(attributeData, static_cast<int>(sizeof(AttributeData)) * getVertexCount(), drawType);
}

// Texture

shape::sub::Texture::Texture() { setAttributeFormat(attribute::texture::FORMAT, attribute::texture::FORMAT_COUNT); }

void shape::sub::Texture::update(Size<int> const &clientSize, DrawType const drawType, Size<int> const &imageSize) {
	struct AttributeData { Point<float> position, texCoords; } attributeData[getVertexCount()];
	
	for (int i = 0; i < getVertexCount(); ++i) {
		attributeData[i].position = vertex[i].position.toOpenGlCoords(clientSize);
		attributeData[i].texCoords = vertex[i].texCoords.toOpenGlTextureCoords(imageSize);
	}
	
	setAttributeData(attributeData, static_cast<int>(sizeof(AttributeData)) * getVertexCount(), drawType);
}

// TextureAndColor

shape::sub::TextureAndColor::TextureAndColor() { TextureBase::setAttributeFormat(attribute::colorAndTexture::FORMAT, attribute::colorAndTexture::FORMAT_COUNT); }

void shape::sub::TextureAndColor::update(Size<int> const &clientSize, DrawType const drawType, Size<int> const &imageSize) {
	struct AttributeData {
		Point<float> position, texCoords;
		::Color color;
	} attributeData[TextureBase::getVertexCount()];
	
	for (int i = 0; i < TextureBase::getVertexCount(); ++i) {
		attributeData[i].position = TextureBase::vertex[i].position.toOpenGlCoords(clientSize);
		attributeData[i].texCoords = TextureBase::vertex[i].texCoords.toOpenGlTextureCoords(imageSize);
		attributeData[i].color = TextureBase::vertex[i].color;
	}
	
	TextureBase::setAttributeData(attributeData, static_cast<int>(sizeof(AttributeData)) * TextureBase::getVertexCount(), drawType);
}
