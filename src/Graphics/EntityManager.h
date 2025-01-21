#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <string>

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void loadShader(std::string const &shaderPath);
	
	// Classification by used shader
	struct Texture {};
	struct Color {};
	struct TextureColor {};
};

#endif // ENTITYMANAGER_H
