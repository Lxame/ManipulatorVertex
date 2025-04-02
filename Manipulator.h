#pragma once

#include <vector>
#include <string>

struct Point3D
{
	double x, y, z;
};

class Manipulator
{
public:
	Manipulator(double x, double y, double z);
	void setName(std::wstring name) { this->name = name; }
	void setFileName(std::wstring fileName) { this->file = fileName; }
	void addPointToSkelet(double x, double y, double z);
	bool writeToJSON();

private:
	Point3D origin;
	std::vector<Point3D> skelet;
	std::wstring name;
	std::wstring file;
	
};

