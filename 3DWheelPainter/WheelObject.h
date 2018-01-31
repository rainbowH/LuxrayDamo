#pragma once
#include "MeshObject.h"
class WheelObject
{
public:
	WheelObject(const Object::MeshObject& flangeTread, const Object::MeshObject& connection,const QString& floatPlantPath)
		:wheel_flange_tread(flangeTread),
		wheel_connection(connection),
		float_plant_path(floatPlantPath)
	{}
	const Object::MeshObject& get_wheel_flange_tread() const
	{
		return wheel_flange_tread;
	}
	const Object::MeshObject& get_wheel_connection() const
	{
		return wheel_connection;
	}
	const QString& get_float_plant_path() const
	{
		return float_plant_path;
	}
	void set_wheel_flange_tread_model(const glm::mat4 & model)
	{
		this->wheel_flange_tread.SetModel(model);
	}
private:
	Object::MeshObject wheel_flange_tread;
	Object::MeshObject wheel_connection;
	QString float_plant_path;
};