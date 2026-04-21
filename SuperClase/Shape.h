#pragma once

#include "config.h"
#include "Shaders.h"
#include "Matrix.h"
#include "Colors.h"


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
	RGB color;
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
	virtual void DrawGeometry(const Matrix& parent)= 0;
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
	void DrawGeometry(const Matrix& parent) override;
};

// PIZZA

