#pragma once
#include <vector>

#include <glm\glm.hpp>

#include "MeshObject.h"
#include "ShaderProgram.h"

enum WheelSide {
    A, B
};
class WheelGroup
{
public:
	static const float WheelSetDistance;
	static const float WheelPivotDistance;
	static const float TrainFrameDistance;
	static const float CarriageDistance;
	static const int WheelCount;
	static const float WheelRadius;
	static const float WheelWidth;
public:
	WheelGroup():wheel(){}
	void SetWheel(const Object::MeshObject& obj);
	void ActivateWheel(size_t id, int outputId);
	void FreezeWheel(size_t id);
	void FreezeAllWheel();
	void BuildModelAndActiveList();
	void Draw(ShaderProgram& shader);
	int GetOutPutID(size_t id);
	int Size() const;
	glm::mat4 GetModel(int id) const;
	
private:
	void BuildLineWheel(const glm::mat4& scale, glm::mat4 translate, const glm::mat4& rotate);
	void BuildLineWheelCarriage(const glm::mat4& scale, glm::mat4& translate, const glm::mat4& rotate);
	Object::MeshObject wheel;
	std::vector<glm::mat4> wheelsModel;
	std::vector<int> wheelsActiveList;
};

