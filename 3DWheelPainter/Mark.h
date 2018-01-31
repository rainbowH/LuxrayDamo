#pragma once

#include "Text.h"
namespace Coordinate {
	class Mark
	{
	public:
		explicit Mark(float position_ratio, const Text& text) :position_ratio(position_ratio), text(text)
		{
			//if (this->position_ratio <= 0 || this->position_ratio >= 1)
			//{
			//	std::cout << "Error::Ruler::set_length::illegal position_ratio" << std::endl;
			//}
		}

		float position_ratio;
		Text text;
	};
}