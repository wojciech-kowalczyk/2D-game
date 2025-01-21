#ifndef VERTEX_H
#define VERTEX_H

#include "Math/Point.h"
#include "Graphics/Color.h"

class Vertex {
public:
	Point<double> position, texCoords;
	Color color;
};

#endif // VERTEX_H
