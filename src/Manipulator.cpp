#include <iostream>
#include <fstream>

#include "pch.h"
#include "Manipulator.h"
#include "../include/json.hpp"

Manipulator::Manipulator()
	: origin{ 0, 0, 0 }
{

}

void Manipulator::addPointToSkelet(double x, double y, double z)
{
	skelet.push_back(Point3D{x, y, z});
}

bool Manipulator::writeToJSON()
{
	nlohmann::ordered_json j;

	j["modelName"] = WStringToString(name);
	j["fileName"] = WStringToString(fileName);
	j["filePath"] = WStringToString(filePath);
	j["origin"] = { {"x", origin.x}, {"y", origin.y}, {"z", origin.z} };
	
	nlohmann::ordered_json j_verteces = nlohmann::ordered_json::array();
	for (const auto& pt : skelet) 
	{
		j_verteces.push_back({
				{"x", pt.x},
				{"y", pt.y},
				{"z", pt.z}
			});
	}
	j["vertices"] = j_verteces;

	std::ofstream file;
	file.open("manipulator.json");
	if (file.is_open())
	{
		file << j.dump(4);
	}
	file.close();

	return true;
}

std::string Manipulator::WStringToString(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
	str.pop_back();
	return str;
}


