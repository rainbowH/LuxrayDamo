#include "PolarCoordinate.h"
#include <iostream>
#include <cmath>
namespace Coordinate {
	const float PolarCoordinate::kAngleMarkExtend = 4.5;
	const float PolarCoordinate::kAngleMarkTextDisToMarkEnd=4.5f;
	const float PolarCoordinate::kCircleMarkTextDisToMark=4.5f;

	void PolarCoordinate::set_radius(float radius)
	{
		if (radius <= 0)
		{
			std::cout << "Error::PolarCoordinate::set_radius::illegal radius" << std::endl;
		}
		this->radius = radius;
	}
	void PolarCoordinate::AddAngleMark(const Mark& mark)
	{
		if (mark.position_ratio < 0 || mark.position_ratio>=1)//不需要360度，0度就可以表示
		{
			std::cout << "Error::PolarCoordinate::AddAngleMark::illegal position_ratio" << std::endl;
		}
		this->angle_marks.push_back(mark);
	}
	void PolarCoordinate::AddCircleMark(const Mark& mark)
	{
		if (mark.position_ratio <0 || mark.position_ratio>1)//0和1是特殊点
		{
			std::cout << "Error::PolarCoordinate::AddCircleMark::illegal position_ratio" << std::endl;
		}
		this->circle_marks.push_back(mark);
	}
	void PolarCoordinate::LoadObject(const Texture& font_texture)
	{
		if (this->radius <= 0)
		{
			std::cout << "Error::PolarCoordinate::LoadObject::illegal radius" << std::endl;
		}
		OpenglObj::OpenglPointMsg whole_pmsg;
		//line
		//main circle line
		OpenglObj::OpenglPointMsg main_circle_pmsg = OpenglObj::LoadCircle(glm::vec3(0, 0, 0), this->radius, 50, glm::vec3(0.0f, 0.0f, 1.0f));
		whole_pmsg += main_circle_pmsg;
		//mark circle line
		for (auto &mark : this->circle_marks)
		{
			if (mark.position_ratio <0 || mark.position_ratio>1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Circle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			if (mark.position_ratio == 0|| mark.position_ratio==1)//0,1不需要画，但text要表示出来
			{
				continue;
			}

			float circle_radius = this->radius*mark.position_ratio;
			auto circle_mark_pmsg= OpenglObj::LoadCircle(glm::vec3(0, 0, 0), circle_radius, 50, glm::vec3(0.0f, 0.0f, 1.0f));
			whole_pmsg += circle_mark_pmsg;
		}
		//mark line
		OpenglObj::OpenglPointMsg angle_mark_msg;
		Vertex origin_point;
		unsigned int origin_pid = 0;
		origin_point.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		angle_mark_msg.AddVertice(origin_point);
		for (auto &mark : this->angle_marks)
		{
			if (mark.position_ratio < 0 || mark.position_ratio>=1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Angle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			float radian = 2.0f*kPIR*mark.position_ratio;
			Vertex mark_point;
			mark_point.Position = glm::vec3((radius + kAngleMarkExtend)*cos(radian), (radius + kAngleMarkExtend)*sin(radian), 0.0f);

			auto mark_pid = angle_mark_msg.GetVertices().size();
			angle_mark_msg.AddVertice(mark_point);

			angle_mark_msg.AddIndice(origin_pid);
			angle_mark_msg.AddIndice(mark_pid);
		}
		whole_pmsg += angle_mark_msg;

		//z axis
		Vertex z_point;
		z_point.Position = glm::vec3(0.0f, 0.0f, this->z_axis_length);

		unsigned int z_start_point_id= whole_pmsg.GetVertices().size();
		whole_pmsg.AddVertice(origin_point);
		whole_pmsg.AddVertice(z_point);
		whole_pmsg.AddIndice(z_start_point_id);
		whole_pmsg.AddIndice(z_start_point_id+1);

		//axis mark text

		//angle mark test
		for (auto &mark : this->angle_marks)
		{

		}
		//circle mark test
		for (auto &mark : this->circle_marks)
		{

		}

	}
}