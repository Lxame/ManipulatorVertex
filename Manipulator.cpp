#include <iostream>
#include <fstream>

#include "pch.h"
#include "Manipulator.h"
#include "../JSONCPP/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

void Manipulator::addPointToSkelet(double x, double y, double z)
{
	skelet.push_back(Point3D{x, y, z});
}

bool Manipulator::writeToJSON()
{
	json j;

	j["modelName"] = WStringToString(name);
	j["fileName"] = WStringToString(fileName);
	j["filePath"] = WStringToString(filePath);
	j["origin"] = { {"x", origin.x}, {"y", origin.y}, {"z", origin.z} };
	
	json j_verteces = json::array();

	for (const auto& pt : skelet) {
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
		file << j;
	}
	file.close();

	return true;
}

std::string Manipulator::WStringToString(const std::wstring& wstr)
{
	//std::string str(wstr.begin(), wstr.end());
	//return str;

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
	str.pop_back();

	return str;
}


