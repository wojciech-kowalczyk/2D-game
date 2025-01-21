#ifndef ENTITIESTEMP_H
#define ENTITIESTEMP_H

#include <string>
#include "Shapes/Specialized.h"

class EntitiesTemp {
public:
	explicit EntitiesTemp(std::string levelName);
	~EntitiesTemp();
	
	shape::circle::color::Base *hole;
};

#endif // ENTITIESTEMP_H
