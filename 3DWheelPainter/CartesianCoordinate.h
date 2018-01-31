#pragma once
#include <string>

#include "Mark.h"
#include "CoordinateObject.h"
#include "Range.h"
namespace Coordinate {
	//象限
	const unsigned char kCoordinateSingleOctant =0x01;//单卦限
	const unsigned char kCoordinateHalfOctant =0x0f;//半卦限
	const unsigned char kCoordinateWholeOctant =0xff;//全卦限
	//坐标
	enum Axis{X,Y,Z};
	enum MarkType{ X_PLUS_MARK,X_MINUS_MARK,Y_PLUS_MARK,Y_MINUS_MARK, ZOX_PLUS_MARK,ZOX_MINUS_MARK,ZOY_PLUS_MARK,ZOY_MINUS_MARK};

	class CartesianCoordinate
	{
	public:
		static const float kAxisTextDisToAxisEnd;
		static const float kXAxisMarkTextDisToMarkEnd;
		static const float kYAxisMarkTextDisToMarkEnd;
		static const float kZAxisMarkTextDisToAxis;
		static const float kTextMarkExtend;
		static const float kAxisExtend;
		static const float kNoTextMarkExtend;

		CartesianCoordinate() 
			:axis_length(glm::vec3(100.0f, 100.0f,100.0f)),
			x_axis_text(Text("X", 8, glm::vec3(1.0f, 1.0f, 1.0f))),
			y_axis_text(Text("Y", 8, glm::vec3(1.0f, 1.0f, 1.0f))),
			z_axis_text(Text("z", 8, glm::vec3(1.0f, 1.0f, 1.0f))),
			coordinate_octant(kCoordinateSingleOctant)
		{}
		//axis length
		void set_axis_length(Axis axis,float length);
		void set_axis_length(float x_length, float y_length, float z_length);
		//axis text
		void set_axis_text(Axis axis,const Text& text);
		void set_axis_text(const Text& x_text, const Text& y_text, const Text& z_text);
		//coordinate quadrant
		void set_coordinate_octant(unsigned char coordinate_octant);
		//add mark
		void AddMark(Axis axis, const Mark& mark);
		//load
		Object::CoordinateObject LoadObject(const Texture& font_texture);
	private:
		bool Judge_Octant(int q_id);
		void GetAxisRange(Range *range);
		void GetMarkRange(Range *range);

		glm::vec3 axis_length;
		Text x_axis_text;
		Text y_axis_text;
		Text z_axis_text;
		unsigned char coordinate_octant;
		std::vector<Mark> x_axis_marks;
		std::vector<Mark> y_axis_marks;
		std::vector<Mark> z_axis_marks;
	};
}