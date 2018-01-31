//#ifndef LOCALENGLINE_H
//#define LOCALENGLINE_H
#pragma once
#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "MeshObject.h"
#include "LineObject.h"
#include "CoordinateAxis.h"
#include "PickMsg.h"
#include "WheelObject.h"

namespace Engine {

	class LocalEngine
	{
	public:
		LocalEngine()
		{
			currentWheelId = -1;
		}
		//void AddWheel(const Mesh& flangeTreadMesh, glm::mat4 flangeTreadModel, glm::vec3 flangeTreadColor, const Mesh& floatPlantMesh, glm::mat4 floatPlantModel, glm::vec3 floatPlantColor);
		//void AddWheel(const Object& flangeTread, const Object& floatPlant);
		//void AddWheel(const Mesh& mesh, glm::mat4 model, glm::vec3 color);
		//void AddWheel(const Object& obj);
		void DrawWheelFloatPlantAndConnection(ShaderProgram &shader,int id);
		void DrawWheelFlangeTread(ShaderProgram &shader, int id);
		void DrawCoordinateAxis(ShaderProgram &shader);

		void AddWheelObject(const WheelObject& wheelObj);
		void AddWheelFloatPlant(const QString& floatPlantPath, const Object::MeshObject& floatPlantObj);
		void SetCoordinateAxis(const Object::MeshObject& arrowObj, const Object::MeshObject& lineObj);
		void UpdateWheelFlangeTreadModel(glm::mat4 model,int id);
		//void SetCurrentId(int id);
		//void SetCurrentWheel(int id);
		int GetWheelsSize();
		//void DrawWheels(ShaderProgram &shader);
		//void DrawWheelFlangeTread(ShaderProgram &shader, int id, const std::vector<glm::vec2>& averRadius);
		//void DrawWheelFloatPlant(ShaderProgram &shader, int id, const std::vector<glm::vec2>);
		
		//pick操作
		PickMsg RayPick_Wheel(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight);
	private:
		glm::vec3 GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view);
		glm::vec4 GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc);//接收世界坐标
		glm::vec4 GetTreadIntersectPositionAndDisForWheel(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rLoc, glm::mat4 model, float widthStart, float widthEnd);//接收世界坐标

		//std::vector<Object> wheelFlangeTreads;
		//std::vector<Object> wheelConnections;
		
		std::vector<WheelObject> wheelObjects;
		std::map<QString, Object::MeshObject> wheelFloatPlants;
		//std::vector<Object> wheelFloatPlants;
		
		//std::vector<Object> wheels;
		CoordinateAxis coordinateAxis;
		//std::vector<TreadRange> ranges;
		int currentWheelId;
	};
}