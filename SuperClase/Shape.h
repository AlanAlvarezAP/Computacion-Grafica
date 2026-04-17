#pragma once

#include "config.h"
#include "Shaders.h"
#include "Matrix.h"

class ShapeNode;


class World{
protected:
	std::vector<float> all_vertices;
	std::vector<unsigned int> all_EBOs;
	Matrix Mat_global;
	Shaders Shader_global;
	ShapeNode* root;
public:
	World();
	~World();
	void DrawShape();
};


class ShapeNode:{
protected:
	Matrix Mat;
	std::vector<unsigned int> EBOs_range;
	Shaders Shader;
	std::vector<ShapeNode*> children;
	GLenum primitive;
	ShapeNode* parent;
	World* world;
public:
	ShapeNode(World* world,std::vector<unsigned int> EBOs,GLenum prim);
	~SceneNode();
	bool isLeaf();
	void AddChildren(ShapeNode* son);
	void ModifiedShaderTransform(const char &tpe,const float &first_val,float second_val=0.0f,char axis='z');
	void ModifiedShaderColor(const float &r,const float &g,const float &b);
	void DrawShape();
	
};



