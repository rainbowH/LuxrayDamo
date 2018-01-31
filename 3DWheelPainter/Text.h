#pragma once
#include <string>

#include <glm/glm.hpp>
class Text
{
public:
	explicit Text():text("NULL"),size(10),color(glm::vec3(1.0f,1.0f,1.0f)){}
	explicit Text(const std::string& text,float size,const glm::vec3& color):text(text),size(size),color(color){}
	void set_text(const std::string& text);
	void set_size(float size);
	void set_color(const glm::vec3& color);
	std::string get_text() const;
	float get_size() const;
	glm::vec3 get_color() const;


private:
	std::string text;
	float size;
	glm::vec3 color;
};