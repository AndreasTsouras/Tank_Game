#ifndef TANKGAME_SHADER_H
#define TANKGAME_SHADER_H

#include <GL/glew.h>
#include <string>

/**
 * Handles input of vertex and fragment shaders from file and src
 */
class Shader
{

public:

	//! Load shaders from file
	static GLuint LoadFromFile(std::string vertexFile, std::string fragmentFile);

	//! load shaders from src
	static GLuint LoadFromSrc(std::string vertexFile, std::string fragmentFile);

};

#endif //TANKGAME_SHADER_H
