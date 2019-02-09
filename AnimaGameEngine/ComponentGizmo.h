#ifndef __COMPONENT_GIZMO_H__
#define __COMPONENT_GIZMO_H__

#include "Component.h"
#include <vector>
#include <string>

class Shader;
class ModuleEditorCamera;

class ComponentGizmo : public Component
{
public:
	ComponentGizmo(ComponentType type, 
		const std::string& vertexPath,
		const std::string &fragmentPath, 
		GameObject *ownerGO);

	~ComponentGizmo();

	void Update(float dt) override;

private:
	std::vector<float> vertices;
	unsigned int numVertices = 34;
	unsigned int VBO, VAO;
	Shader *shader = nullptr;

	void CreateShader(const std::string& vertexPath, const std::string &fragmentPath);
	void GenerateVertexData();
	void SetVertexBuffers();
	void ClearVertexBuffers();
	void ComputeNumVerticesToDraw();
	
};

#endif
