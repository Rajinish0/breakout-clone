#include "shader.h"
#include <exception>

std::string Shader::loadSource(std::string source) {
	std::ifstream file{source};
	if (!file) {
		throw std::runtime_error("Could not load file " + source);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

Shader::Shader(std::string vertexSource,
	std::string fragmentSource,
	std::string geometrySource) 
	:shaderProg(glCreateProgram()){

	std::string vSourceCode = loadSource(vertexSource);
	std::string fSourceCode = loadSource(fragmentSource);
	std::string gSourceCode = "";
	if (geometrySource != "")
		gSourceCode = loadSource(geometrySource);


	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vSourceCode);
	checkCompileErrors(vertexShader, "VERTEX", vertexSource);
	unsigned int fragShader = compileShader(GL_FRAGMENT_SHADER, fSourceCode);
	checkCompileErrors(fragShader, "FRAGMENT", fragmentSource);
	unsigned int geomShader;
	if (geometrySource != "") {
		geomShader = compileShader(GL_GEOMETRY_SHADER, gSourceCode);
		checkCompileErrors(geomShader, "GEOMETRY", geometrySource);
	}


	glAttachShader(shaderProg, vertexShader);
	if (geometrySource != "")
		glAttachShader(shaderProg, geomShader);
	glAttachShader(shaderProg, fragShader);
	glLinkProgram(shaderProg);
	checkCompileErrors(shaderProg, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	if (geometrySource != "")
		glDeleteShader(geomShader);

}

Shader& Shader::setBool(const std::string& name, bool value) {
	glUniform1i(getLoc(name), (int)value);
    return (*this);
}

Shader& Shader::setInt(const std::string& name, int value){
	glUniform1i(getLoc(name), value);
    return (*this);
}

Shader& Shader::setFloat(const std::string& name, float value){
	glUniform1f(getLoc(name), value);
    return (*this);
}

Shader& Shader::setVec4(const std::string& name, glm::vec4 value){
	glUniform4f(getLoc(name), value.x, value.y, value.z, value.w);
    return (*this);
}

Shader& Shader::setVec3(const std::string& name, glm::vec3 value){
	glUniform3f(getLoc(name), value.x, value.y, value.z);
    return (*this);
}

Shader& Shader::setVec2(const std::string& name, glm::vec2 value){
	glUniform2f(getLoc(name), value.x, value.y);
    return (*this);
}

Shader& Shader::setMatrix(const std::string& name, glm::mat4 value){
	glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
    return (*this);
}

Shader& Shader::setUniformBlockBinding(const std::string& name, int bindingPoint){
	unsigned int name_idx = glGetUniformBlockIndex(shaderProg, name.c_str());
	glUniformBlockBinding(shaderProg, name_idx, bindingPoint);
    return (*this);
}

unsigned int Shader::getLoc(const std::string& name) const{
		return glGetUniformLocation(shaderProg, name.c_str());
}

unsigned int Shader::compileShader(GLenum type, std::string& sourceCode) {
	
	unsigned int shdr = glCreateShader(type);
	const char* src = sourceCode.c_str();

	glShaderSource(shdr, 1, &src, NULL);
	glCompileShader(shdr);

	return shdr;

}

Shader& Shader::use() {
	glUseProgram(shaderProg);
    return (*this);
}

Shader::~Shader(){
    // glDeleteProgram(this->shaderProg);
}



