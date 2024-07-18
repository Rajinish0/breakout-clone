#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {

public :
	
    unsigned int shaderProg;

    Shader() = default;
	Shader(std::string vertexSource,
		   std::string fragmentSource,
		   std::string geometrySource = "");

	Shader& use();
	Shader& setBool(const std::string& name, bool value);
	Shader& setInt(const std::string& name, int value);
	Shader& setFloat(const std::string& name, float value);
	Shader& setVec4(const std::string& name, glm::vec4 value);
	Shader& setVec3(const std::string& name, glm::vec3 value);
	Shader& setVec2(const std::string& name, glm::vec2 value);
	Shader& setMatrix(const std::string& name, glm::mat4 value);
	Shader& setUniformBlockBinding(const std::string& name, int bindingPoint);
	unsigned int getLoc(const std::string& name) const;

	//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	
	static std::string loadSource(std::string source);
	static unsigned int compileShader(GLenum type, std::string& sourceCode);

    ~Shader();
private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type, std::string sourceFile = "")
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR" << "(" << sourceFile << ")"<< "of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};