#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include "libraries/glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string &vertexPath, const std::string &fragPath);

	void Use() const;

	void SetInt(const std::string &name, int value) const;

	void SetFloat(const std::string &name, float value) const;

	void SetBool(const std::string &name, bool value) const;

	void SetVec2(const std::string &name, const glm::vec2 &value) const;

	void SetVec3(const std::string &name,  const glm::vec3 &value) const;

	void SetMat3(const std::string &name, const glm::mat3 &value) const;

	void SetMat4(const std::string &name, const glm::mat4 &value) const;

private:
	unsigned programId;

	void CheckErrors(unsigned int type, unsigned int shaderOrProgram);
};

#endif
