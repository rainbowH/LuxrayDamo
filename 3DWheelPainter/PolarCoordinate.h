#pragma once
#include <vector>

#include "Mark.h"
#include "Texture.h"
#include "OpenglPointMsg.h"
namespace Coordinate {
	const float kPIR = 3.1415926f;
	class PolarCoordinate {
	public:
		static const float kAngleMarkExtend;
		static const float kAngleMarkTextDisToMarkEnd;
		static const float kCircleMarkTextDisToMark;
		static const float kZAxisMarkTextDisToAxisEnd;

		PolarCoordinate():radius(50.0f), z_axis_length(100.0f),angle_marks(), circle_marks() {}
		PolarCoordinate(float radius):radius(radius), z_axis_length(100.0f), angle_marks(),circle_marks(){}

		void set_radius(float radius);
		void AddAngleMark(const Mark& mark);
		void AddCircleMark(const Mark& mark);
		void LoadObject(const Texture& font_texture);
	private:

		float radius;
		float z_axis_length;
		std::vector<Mark> angle_marks;
		std::vector<Mark> circle_marks;
	};
}