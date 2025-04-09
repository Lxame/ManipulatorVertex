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
	Manipulator() {};
	void setOrigin(double x, double y, double z) 
	{
		this->origin.x = x;
		this->origin.y = y;
		this->origin.z = z;
	};
	void setName(std::wstring name) { this->name = name; }
	void setFileName(std::wstring fileName) { this->fileName = fileName; }
	void setFilePath(std::wstring filePath) { this->filePath = filePath; }
	void addPointToSkelet(double x, double y, double z);
	bool writeToJSON();

private:
	Point3D origin;
	std::vector<Point3D> skelet;
	std::wstring name;
	std::wstring fileName;
	std::wstring filePath;
	
private:
	std::string WStringToString(const std::wstring& wstr);
};

