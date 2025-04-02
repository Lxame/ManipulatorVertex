#include "pch.h"
#include "Manipulator.h"
#include "../JSONCPP/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

Manipulator::Manipulator(double x, double y, double z)
{
	origin.x = x;
	origin.y = y;
	origin.z = z;
}

void Manipulator::addPointToSkelet(double x, double y, double z)
{
	skelet.push_back(Point3D{x, y, z});
}

bool Manipulator::writeToJSON()
{
	json j;

	j["modelName"] = name;
	j["fileName"] = file;
	j["origin"] = { {"x", origin.x}, {"y", origin.y}, {"z", origin.z} };

	return true;
}


