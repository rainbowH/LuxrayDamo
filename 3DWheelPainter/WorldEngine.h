#pragma once
#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "MeshObject.h"
#include "WheelGroup.h"
namespace Engine {
	class WorldEngine
	{
	public:
		WorldEngine() {}
		//void AddMesh(const Mesh& mesh, glm::mat4 model = glm::mat4(1.0f));
		//void AddObject(const Object& obj);
		void SetTrainFrame(const Object::MeshObject& obj);
		void DrawTrainFrame(ShaderProgram &shader);

		void SetTrainBox(const Object::MeshObject& obj);
		void DrawTrainBox(ShaderProgram &shader);

		void SetWheelGroup(const Object::MeshObject& obj);
		void DrawWheelGroup(ShaderProgram &shader);

		//int GetObjectsSize();
		//void DrawObjects(ShaderProgram &shader);
		//pick操作
		int RayPick_WheelGroup(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight);

		void ActivateWheel(WheelSide side, int id, int outputId);
		void ActivateWheel(int id, int outputId);
		void FreezeWheel(WheelSide side, int id);
		void FreezeAllWheel();
	private:
		glm::vec3 GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view);
		glm::vec4 GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc);//接收世界坐标
		glm::vec4 GetTreadIntersectPositionAndDisForWheelGroup(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rLoc, glm::mat4 model, float widthStart, float widthEnd);
		float IntersectWithCylinderForWheelGroup(const glm::mat4& model,/*const glm::vec3& cylinderCenter,*/float cylinderWidth, float cylinderRadius, const glm::vec3& ray_dir, const glm::vec3& cameraPosition);

		Object::MeshObject trainFrame;
		Object::MeshObject trainBox;
		WheelGroup wheelGroup;
	};
}