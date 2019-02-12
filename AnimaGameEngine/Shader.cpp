#include "Shader.h"
#include <fstream>
#include "Globals.h"
#include <sstream>
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <vector>

Shader::Shader(const std::string &vertexPath, const std::string &fragPath)
{
	std::ifstream vertexIS;
	std::ifstream fragmentIS;
	std::stringstream vertexSS;
	std::stringstream fragmentSS;

	//Set exceptions to be caught
	vertexIS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentIS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexIS.open(vertexPath);
		fragmentIS.open(fragPath);

		vertexSS << vertexIS.rdbuf();
		fragmentSS << fragmentIS.rdbuf();

		vertexIS.close();
		fragmentIS.close();
	}
	catch (std::ifstream::failure e)
	{
		MYLOG("ERROR: Shader: Error while reading shader: %s", strerror(errno))
		return;
	}

	std::string vertexStr = vertexSS.str();
	std::string fragmentStr = fragmentSS.str();


	//Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *source = (const GLchar*)vertexStr.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);
	CheckErrors(GL_VERTEX_SHADER, vertexShader);

	//Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = (const GLchar*)fragmentStr.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	glCompileShader(fragmentShader);
	CheckErrors(GL_VERTEX_SHADER, fragmentShader);

	//Create program, attach shaders to it and link them
	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);
	CheckErrors(0, programId);

}

void Shader::Use() const
{
	glUseProgram(programId);
}

void Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetVec2(const std::string & name, const glm::vec2 & value) const
{
	glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string & name, const glm::vec3 & value) const
{
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::SetMat3(const std::string & name, const glm::mat3 & value) const
{
	glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string & name, const glm::mat4 & value) const
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::CheckErrors(unsigned int type, unsigned int shaderOrProgram)
{
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER)
	{
		glGetShaderiv(shaderOrProgram, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderOrProgram, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderOrProgram, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(shaderOrProgram);
			MYLOG("ERROR: Shader: %s", infoLog.data())
		}
	}
	else
	{
		glGetProgramiv(shaderOrProgram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(shaderOrProgram, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shaderOrProgram, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(shaderOrProgram);
			MYLOG("Error: Shader: %s", infoLog.data())
		}
	}
}
