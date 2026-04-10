#pragma once

#include "Matrix.h"

struct Input_step{
	char type;
	std::pair<float,float> values;
	int frames;
	char axis;
};

class Point{
public:
	float x,y,angle;
};

class Sector{
private:
	Point start_point;
	Point end_point;
	std::vector<Point> Points;
	Matrix modelo;
public:
	unsigned int triang_ini,triang_end,border_ini,border_end,line_ini,line_end,pepperoni_ini,pepperoni_end,pina_ini,pina_end,oregano_ini,oregano_end;
	void Generate_Sector(float start_angle,float end_angle,const float RADIUS,const int number_segments,Point center);
	Matrix* Get_Matrix();
	Point* Get_Start();
	Point* Get_End();
	std::vector<Point>& Get_Points();
};

class Circle{
public:
	std::vector<Sector> sectores;
	std::vector<std::pair<int,int>>rango_vertices_sector;
	int Get_Sector(Point evaluate,const float radio);
	void Generate_Circle(int amount_sector,const float RADIUS);
	void Generate_Vertices_OpenGL(std::vector<float>& vertices,std::vector<unsigned int>& indices);
	void Add_Condiments_Pepperoni(Sector &sec,std::vector<Point>& vec);
	void Add_Condiments_Pina(Sector &sec,std::vector<Point>& vec);
	void Add_Condiments_Oregano(Sector &sec,std::vector<Point>& vec);
};