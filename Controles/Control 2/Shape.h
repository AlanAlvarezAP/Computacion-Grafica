#pragma once

#include "config.h"
#include "Shaders.h"
#include "Matrix.h"
#include "Colors.h"


class ShapeNode;


// Para realizar animaciones :D
class Animation_Step{
public:
	ShapeNode* target;
	char axis,type;
	float value_total;
	float duration,elapsed;
	
	Animation_Step(ShapeNode* targ,float durat,char tp,float val,char ax);
	
	void Update_animation(float dt);
	bool finished();
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
	ShapeNode* root,*activeSceneNode;
	std::queue<Animation_Step*> pedidos_norm;
	std::stack<Animation_Step*> pedidos_inv;
	int globalColorCounter;
public:
	World();
	~World();
	void DrawShape();
	std::vector<unsigned int> Add_Batch(std::vector<float>& vectors,std::vector<unsigned int>& indices,unsigned int &offset);
	void print(ShapeNode* rot);
	void Add_animation(Animation_Step* anim);
	void Execute_animations(float dt,char inv);
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
	bool IsDrawable;
	int selected_part;
    bool editWhole;
public:
	ShapeNode(World* world,unsigned int prim,const std::string &nam);
	virtual ~ShapeNode();
	void AddChildren(ShapeNode* son);
	void ModifiedShaderTransform(const char &tpe,const float &first_val,float second_val,char axis);
	void ModifiedShaderColor(const float &r,const float &g,const float &b);
	void DrawShape(const Matrix& parent);
	void EditMode();
	virtual void handleKey(int key, int mods,char CURRENT_AXIS) {}
	virtual void printMenu() {}
	virtual void Generate() {}
	virtual void DrawGeometry(const Matrix& parent){}
	virtual void SelectPart(int index);
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

