#pragma once

#include "Shape.h"

class Base_Piramid: public ShapeNode{
public:
	Point center;
	float radius;
	int sides;
	float steps;
	unsigned int sector_Start,lines_Start;
public:
	Base_Piramid(World* world,const Point& centro,const float &rad,const int &sid,const float &ste);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Sector_Piramid: public ShapeNode {
public:
	Point startPoint,endPoint,center;
	float radius,height;
	unsigned int sector_Start,lines_Start;
public:
	Sector_Piramid(World* world,const Point& start_point,const Point& end_point,const Point& centro,const float &rad,const float& heig);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Piramid: public ShapeNode{
public:
	Point center;
	int amount_sector;
	float RADIUS,HEIGHT;
public:
	Piramid(World* world,const Point& cent,const int& amount_sec,const float& rad,const float& heig);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
};