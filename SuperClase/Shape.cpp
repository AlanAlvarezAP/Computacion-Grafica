#include "Shape.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


World::World(){
	root=nullptr;
}
World::~World(){
	delete root;
}

void World::DrawShape(){
    Matrix identity;
    root->DrawShape(identity);
}

ShapeNode::ShapeNode(World* world,std::vector<unsigned int> EBOs,GLenum prim){
	this->world=world;
	this->EBOs_range=EBOs;
	this->primitive=prim;
}

ShapeNode::~SceneNode(){
	for (auto son:children){
		delete son;
	}
}

bool ShapeNode::IsLeaf(){
	if(!this->EBOs_range.empty()){
		return true;
	}
	return false;
}

void ShapeNode::AddChildren(ShapeNode* son){
	son->parent=this;
	children.push_back(son);
}

void ShapeNode::ModifiedShaderTransform(const char &tpe,const float &first_val,float second_val=0.0f,char axis='z'){
	this->Mat.UpdateView(tpe,first_val,second_val,axis);
	this->Shader.SetMatrix(this->Mat);
}

void ShapeNode::ModifiedShaderColor(const float &r,const float &g,const float &b){
	this->Shader.SetColor(r,g,b);
}

void ShapeNode::DrawShape(const Matrix& parent){
	Matrix global = parent * this->Mat;
	this->Shader.SetMatrix(global);
	
	if(IsLeaf()){
		glDrawElements(primitive,EBOs_range.size(),GL_UNSIGNED_INT,(void*)((EBOs_range[0])*sizeof(unsigned int)));
		return;
	}
	
	for(auto son : children){
        son->DrawShape(global);
	}
}