#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
	enum Type { Texture, Color, TextureColor };
	
	/// Creates shader of given type and activates it
	explicit Shader(Type type);
	~Shader();
	
	void activate() const;
private:
	unsigned id;
};

#endif // SHADER_H
