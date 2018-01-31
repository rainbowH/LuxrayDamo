#include "WorldEngine.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine_inl.h"
namespace Engine {
	//void Engine::addMesh(const Mesh& mesh, glm::mat4 model)
	//{
	//	Object obj(mesh, model);
	//	//std::cout << "initE1" << std::endl;
	//	objects.push_back(obj);
	//	//std::cout << "initE2" << std::endl;
	//}
	void WorldEngine::SetWheelGroup(const Object::MeshObject& obj)
	{
		this->wheelGroup.SetWheel(obj);
		this->wheelGroup.BuildModelAndActiveList();
	}

	void WorldEngine::DrawWheelGroup(ShaderProgram &shader)
	{
		this->wheelGroup.Draw(shader);
	}

	int WorldEngine::RayPick_WheelGroup(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight)
	{
		float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//���ŵ�-1,0f-1.0f
		float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;

		glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);
		int wheelSize = wheelGroup.Size();
		float minDis = FLT_MAX;
		float minI = -1;
		for (int i = 0; i != wheelSize; ++i)
		{
			float dis = this->IntersectWithCylinderForWheelGroup(wheelGroup.GetModel(i), WheelGroup::WheelWidth, WheelGroup::WheelRadius, ray_dir, cameraPosition);
			if (minDis > dis)
			{
				minDis = dis;
				minI = i;
			}
		}
		if (minI == -1)
		{
			return -1;
		}
		else
		{
			//this->ActivateWheel(minI, 0);
			return wheelGroup.GetOutPutID(minI);
		}

	}

	void WorldEngine::SetTrainFrame(const Object::MeshObject& obj)
	{
		this->trainFrame = obj;
	}
	void WorldEngine::DrawTrainFrame(ShaderProgram &shader)
	{
		//shader.setMat4("model", this->trainFrame.GetModel());
		this->trainFrame.Draw(shader);
	}
	void WorldEngine::SetTrainBox(const Object::MeshObject& obj)
	{
		this->trainBox = obj;
	}
	void WorldEngine::DrawTrainBox(ShaderProgram &shader)
	{
		//shader.setMat4("model", this->trainBox.GetModel());
		this->trainBox.Draw(shader);
	}

	//void WorldEngine::AddObject(const Object& obj)
	//{
	//	objects.push_back(obj);
	//}
	//void WorldEngine::AddMesh(const Mesh& mesh, glm::mat4 model)
	//{
	//	Object obj(mesh, model);
	//	objects.push_back(obj);
	//}
	//int WorldEngine::GetObjectsSize()
	//{
	//	return objects.size();
	//}

	//void WorldEngine::DrawObjects(ShaderProgram &shader)
	//{
	//	//�ƶ�
	//	//int size = objects.size();
	//	//for (int i = 0; i != size; ++i)
	//	//{
	//	//	shader.setMat4("model", objects[i].model);
	//	//	objects[i].mesh.Draw(shader);
	//	//}
	//	for (auto &i : objects)
	//	{
	//		shader.setMat4("model", i.GetModel());
	//		i.Draw(shader);
	//	}
	//	//std::cout << "Draw ok3" << std::endl;
	//}

	float WorldEngine::IntersectWithCylinderForWheelGroup(const glm::mat4& model, /*const glm::vec3& cylinderCenter, */float cylinderWidth, float cylinderRadius, const glm::vec3& ray_dir, const glm::vec3& cameraPosition)//������Z�᷽���ϵ�����pick
	{

		glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //ƽ��1  v1Ҫ��Բ��,ԭ��������ƽ���ŵģ�����pick��Ҳ�����ŵ�,���ƻ������ܶ�
		glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
		glm::vec3 P1V3Loc(0.0f, 0.0f, 1.0f);
		glm::vec3 P1RLoc(cylinderRadius, 0.0f, 0.0f);

		glm::vec3 P2V1Loc(0.0f, 0.0f + cylinderWidth, 0.0f);  //ƽ��2
		glm::vec3 P2V2Loc(1.0f, 0.0f + cylinderWidth, 0.0f);
		glm::vec3 P2V3Loc(0.0f, 0.0f + cylinderWidth, 1.0f);
		glm::vec3 P2RLoc(cylinderRadius, 0.0f + cylinderWidth, 0.0f);

		glm::vec3 P1V1Wor = glm::vec3(model*glm::vec4(P1V1Loc, 1.0f));
		glm::vec3 P1V2Wor = glm::vec3(model*glm::vec4(P1V2Loc, 1.0f));
		glm::vec3 P1V3Wor = glm::vec3(model*glm::vec4(P1V3Loc, 1.0f));
		glm::vec3 P1RWor = glm::vec3(model*glm::vec4(P1RLoc, 1.0f));

		glm::vec4 P1PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P1V1Wor, P1V2Wor, P1V3Wor, P1RWor);

		glm::vec3 P2V1Wor = glm::vec3(model*glm::vec4(P2V1Loc, 1.0f));
		glm::vec3 P2V2Wor = glm::vec3(model*glm::vec4(P2V2Loc, 1.0f));
		glm::vec3 P2V3Wor = glm::vec3(model*glm::vec4(P2V3Loc, 1.0f));
		glm::vec3 P2RWor = glm::vec3(model*glm::vec4(P2RLoc, 1.0f));

		glm::vec4 P2PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P2V1Wor, P2V2Wor, P2V3Wor, P2RWor);

		glm::vec4 TPD = this->GetTreadIntersectPositionAndDisForWheelGroup(ray_dir, cameraPosition, P1V1Wor, P1RWor, model, 0, cylinderWidth);

		//std::cout << "msg: " << P1PD.w<<" "<<P2PD.w<<" "<<TPD.w<<std::endl;
		glm::vec4 minPPD = P1PD.w > P2PD.w ? P2PD : P1PD;
		glm::vec4 minPD = TPD.w > minPPD.w ? minPPD : TPD;
		if (minPD.w == FLT_MAX)
		{
			//std::cout << "no thing!" << std::endl;
			return FLT_MAX;
		}
		else//
		{
			return minPD.w;
		}
	}

	glm::vec3 WorldEngine::GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view)
	{
		glm::vec3 ray_nds = glm::vec3(nds_x, nds_y, -1.0f);
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);//�ü�����

													  //camera
		glm::vec4 ray_camera = glm::inverse(projection)*ray_clip;
		ray_camera = glm::vec4(ray_camera.x, ray_camera.y, -1.0, 0.0);//û�вü�(��׼��)����������꣬���԰�����Ŀ������z����Ϊ-1
																	  //world
		glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view)*ray_camera));//����ҪcameraPosition��view�оͰ���cameraPosition����Ϣ���൱��ֱ��ת������

		glm::vec3 ray_dir = glm::normalize(glm::vec3(ray_world));

		//std::cout << "ray_world:" << ray_world.x << " " << ray_world.y << " " << ray_world.z << std::endl;


		return ray_dir;
	}


	glm::vec4 WorldEngine::GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc)
	{
		glm::vec3 dv1 = V2 - V1;
		glm::vec3 dv2 = V3 - V1;
		//glm::mat3 coefficient =  //���󣡲�����ôд
		//{
		//	0-ray_dir.x, dv1.x, dv2.x,
		//	0-ray_dir.y, dv1.y, dv2.y,
		//	0-ray_dir.z, dv1.z, dv2.z,
		//};
		glm::mat3 coefficient = glm::mat3(-ray_dir, dv1, dv2);

		glm::vec3 value = cameraPosition - V1;
		glm::vec3 solution = glm::inverse(coefficient)*value;

		//std::cout <<"solution: "<<solution.x << std::endl;
		/*if (isinf(solution.x))
		{
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
		else*/ if (solution.x > 0)//��ֹ�����䵽
		{
			float u = solution.y;
			float v = solution.z;
			//if (solution.x > 0.9)
			//{
			//std::cout <<"len:"<< solution.x <<" u:"<<u <<" v:"<< v << std::endl;
			//}
			glm::vec3 pickPoint = V1 + u*dv1 + v*dv2;
			if (dis(pickPoint, V1) > dis(rLoc, V1))
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX); //����Χȡ��
			}
			else
			{
				//std::cout << "OR:" << dis(rLoc, V1) << " NR:" << dis(pickPoint, V1) << std::endl;
				return glm::vec4(pickPoint, solution.x);//����;���
			}
		}
		else
		{
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}

	}

	glm::vec4 WorldEngine::GetTreadIntersectPositionAndDisForWheelGroup(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rWor, glm::mat4 model, float widthStart, float widthEnd)
	{

		float a = pow(ray_dir.x, 2) + pow(ray_dir.y, 2);
		float b = 2 * (cameraPosition.x*ray_dir.x + cameraPosition.y*ray_dir.y - V1.x*ray_dir.x - V1.y*ray_dir.y);
		float c = pow(cameraPosition.x, 2) + pow(cameraPosition.y, 2) + pow(V1.x, 2) + pow(V1.y, 2) - 2 * (V1.x*cameraPosition.x + V1.y*cameraPosition.y) - pow(dis(rWor, V1), 2);

		float J = pow(b, 2) - 4 * a*c;
		if (J < 0)
		{
			//std::cout << "no!" << std::endl;
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
		else
		{
			float solution1 = (-b + sqrtf(J)) / (2.0f*a);
			float solution2 = (-b - sqrtf(J)) / (2.0f*a);
			if (solution1 <= 0 && solution2 <= 0)//��������
			{
				//std::cout << "no!" << std::endl;
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}
			else if (solution1 > 0 && solution2 <= 0)//�ڲ�����
			{
				//std::cout << "haveN!" << std::endl;
				glm::vec3 pickPoint;
				pickPoint.x = cameraPosition.x + ray_dir.x*solution1;
				pickPoint.y = cameraPosition.y + ray_dir.y*solution1;
				pickPoint.z = cameraPosition.z + ray_dir.z*solution1;

				glm::vec4 pickWor = glm::vec4(glm::vec3(pickPoint), 1.0f);
				glm::vec3 pickLoc = glm::vec3(glm::inverse(model)*pickWor);
				if (pickLoc.y >= widthStart && pickLoc.y <= widthEnd)
				{

					//std::cout << "tx,ty:" << pickLoc.x << " " << pickLoc.y << std::endl;
					return glm::vec4(pickPoint, solution1);
				}
				else
				{
					return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
				}
			}
			else if (solution1 > 0 && solution2 > 0)//�ҵ�����ĵ�
			{
				//std::cout << "haveW!" << std::endl;
				glm::vec3 pickPoint;
				pickPoint.x = cameraPosition.x + ray_dir.x*solution2;
				pickPoint.y = cameraPosition.y + ray_dir.y*solution2;
				pickPoint.z = cameraPosition.z + ray_dir.z*solution2;

				glm::vec4 pickWor = glm::vec4(glm::vec3(pickPoint), 1.0f);  //ʰȡ���Ƿ��ڿ�ȷ�Χ�ڣ�
				glm::vec3 pickLoc = glm::vec3(glm::inverse(model)*pickWor);
				if (pickLoc.y >= widthStart && pickLoc.y <= widthEnd)
				{
					//std::cout << "tx,ty:" << pickLoc.x << " " << pickLoc.y << std::endl;
					return glm::vec4(pickPoint, solution2);
				}
				else
				{
					return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
				}

			}
			else
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}
		}
	}

	void WorldEngine::ActivateWheel(WheelSide side, int id, int outputId)//�趨Ҫ�������ӵ�λ��,�����ӱ����ʱ�����ID
	{
		if (side == WheelSide::A)
		{
			id += WheelGroup::WheelCount / 2;
			wheelGroup.ActivateWheel(id, outputId);
		}
		else if (side == WheelSide::B)
		{
			wheelGroup.ActivateWheel(id, outputId);
		}
	}
	void WorldEngine::ActivateWheel(int id, int outputId)//�趨Ҫ�������ӵ�λ��,�����ӱ����ʱ�����ID
	{
		wheelGroup.ActivateWheel(id, outputId);
	}
	void WorldEngine::FreezeWheel(WheelSide side, int id)
	{
		if (side == WheelSide::A)
		{
			id += WheelGroup::WheelCount / 2;
			wheelGroup.FreezeWheel(id);
		}
		else if (side == WheelSide::B)
		{
			wheelGroup.FreezeWheel(id);
		}
	}
	void WorldEngine::FreezeAllWheel()
	{
		wheelGroup.FreezeAllWheel();
	}
}