#pragma once

#include "config.h"
#include "Shaders.h"
#include "Matrix.h"

class ShapeNode;


// Para realizar animaciones :D
struct Input_step{
	char type;
	float value_x,value_y,value_z;
	float angle;
	int frames;
	char axis;
};

// Para facilitarme la creación de puntos xd
struct Point{
	float x,y,z,angle;
};


class World{
public:
	std::vector<float> all_vertices;
	std::vector<unsigned int> all_EBOs;
	Matrix Mat_global;
	Shaders Shader_global;
	ShapeNode* root;
	std::deque<Input_step> pedidos;
public:
	World();
	~World();
	void DrawShape();
	std::vector<unsigned int> Add_Batch(std::vector<float>& vectors,std::vector<unsigned int>& indices,unsigned int &offset);
	void print(ShapeNode* rot);
};


class ShapeNode{
public:
	Matrix Mat;
	std::vector<unsigned int> EBOs_range;
	Shaders Shader;
	std::vector<ShapeNode*> children;
	unsigned int primitive,offset;
	float r,g,b;
	ShapeNode* parent;
	World* world;
	std::string name;
public:
	ShapeNode(World* world,unsigned int prim,const std::string &nam);
	virtual ~ShapeNode();
	bool IsLeaf();
	void AddChildren(ShapeNode* son);
	void ModifiedShaderTransform(const char &tpe,const float &first_val,float second_val,char axis);
	void ModifiedShaderColor(const float &r,const float &g,const float &b);
	void DrawShape(const Matrix& parent);
	virtual void Generate() = 0;
};



// -------------- LISTA DE POSIBLES FIGURAS POLIMORFICAS -------------
// Circulo
class Circle: public ShapeNode{
public:
	float radius;
	int number_segments;
	Point center;
public:
	Circle(World* world,const Point &cent,const float &rad,const int &num_seg);
	void Generate() override;
};

// PIZZA
class Sector;
class Pizza;

class Piña: public ShapeNode{
public:
	Sector *sectorOwn;
	Pizza* pizzaOwn;
    int target_index;
public:
	Piña(World* world,Sector& sec,Pizza& piz,int idx);
	void Generate() override;
};

class Pepperoni: public ShapeNode{
public:
	Sector *sectorOwn;
	float radius;
	Pizza* pizzaOwn;
    int target_index;
public:
	Pepperoni(World* world,Sector& sec,Pizza& piz,int idx,const float& rad);
	void Generate() override;
};

class Oregano: public ShapeNode{
public:
	Sector *sectorOwn;
public:
	Oregano(World* world,Sector& sec);
	void Generate() override;
};

class Sector: public ShapeNode {
public:
	Point startPoint,endPoint,center;
	float radius;
	int number_segments;
	unsigned int sector_Start,lines_Start;
public:
	Sector(World* world,const Point& start_point,const Point& end_point,const Point& centro,const float &rad,const int &num_seg);
	void Generate() override;
	
};

class Pizza: public ShapeNode{
public:
	Point center;
	int amount_sector;
	float RADIUS;
public:
	Pizza(World* world,const Point& cent,const int& amount_sec,const float& rad);
	// No necesidad de Pizza destructor ?
	int Get_Sector(Point evaluate,const float radio);
	void Generate() override;
};

