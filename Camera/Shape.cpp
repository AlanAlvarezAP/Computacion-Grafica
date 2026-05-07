#include "Shape.h"
#include "Pizza.h"
#include "Piramid.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>



World::World(){
	root = new ShapeNode(this, GL_TRIANGLES, "ROOT");
	activeSceneNode=nullptr;
	globalColorCounter=0;
}
World::~World(){
	delete root;
}

void World::DrawShape(const Matrix& view,const Matrix& projection){
    Matrix identity;
    root->DrawShape(identity,view,projection);
}

std::vector<unsigned int> World::Add_Batch(std::vector<float>& vectors,std::vector<unsigned int>& indices,unsigned int& offset){
	unsigned int inicio=all_EBOs.size();
	offset=inicio;
	all_vertices.insert(all_vertices.end(),vectors.begin(),vectors.end());
	all_EBOs.insert(all_EBOs.end(),indices.begin(),indices.end());
	return std::vector<unsigned int>(all_EBOs.begin()+inicio,all_EBOs.end());
	
}


void World::print(ShapeNode* rot,int offset){
	if(!rot || (int)rot->children.size() <= 0){
		return;
	}
	
	if(root == rot){
		std::cout << " DATOS GENERALES " << std::endl;
		std::cout << " ALL VERTICES -> " << this->all_vertices.size() << std::endl;
		std::cout << " ALL INDICES -> " << this->all_EBOs.size() << std::endl;
	}
	std::string spaces;
	for (int i = 0; i < offset; i++){
        spaces+="    ";
	}
	std::cout << spaces << "THIS -> " << rot->name << " and size " << rot->EBOs_range.size() << " and offset of " << rot->offset << std::endl;
	for(int i=0;i<rot->children.size();i++){
		std::cout << spaces << "|-----Val -> " << rot->children[i]->name << " and size " << rot->children[i]->EBOs_range.size() << " and offset of " << rot->children[i]->offset << " Index == " << i << std::endl;
	}
	for(auto p:rot->children){
		print(p,offset+1);
	}
}

ShapeNode::ShapeNode(World* world,unsigned int prim,const std::string &nam){
	this->world=world;
	this->primitive=prim;
	this->parent=nullptr;
	this->name=nam;
	this->offset=0;
	this->IsDrawable=false;
	this->selected_part=-1;
	this->editWhole=true;
}

ShapeNode::~ShapeNode(){
	for (auto son:children){
		delete son;
	}
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
	color.r=r;
	color.g=g;
	color.b=b;
	this->Shader.SetColor(r,g,b);
}

void ShapeNode::DrawShape(const Matrix& parent,const Matrix& view,const Matrix& projection){
	Matrix global = parent*this->Mat;
	Shader.SetView(view);
	Shader.SetProjection(projection);
	/*Piramid* piramide=dynamic_cast<Piramid*>(this);
	if(piramide){
		std::cout << "--------------------" << std::endl;
		parent.PrintMatrix();
		global.PrintMatrix();
		piramide->Mat.PrintMatrix();
		std::cout << "--------------------" << std::endl;
	}*/
	if(IsDrawable){
		this->DrawGeometry(global);
    }
	
	for(auto son : children){
        son->DrawShape(global,view,projection);
	}
}

Matrix ShapeNode::GetWorldMatrix(){
    if (parent)
        return parent->GetWorldMatrix() * Mat;
    return Mat;
}

Point ShapeNode::GetWorldPosition() {
    Matrix w = GetWorldMatrix();
    return Point{w.matrix[12], w.matrix[13], w.matrix[14]};
}

void ShapeNode::SelectPart(int index){
	selected_part = index;
}

void ShapeNode::EditMode(){
	editWhole = !editWhole;
	std::cout << (editWhole ? "Modo: TODO\n" : "Modo: PARTE\n");
}

void ShapeNode::handleKey(int key, int mods,char CURRENT_AXIS){
	ShapeNode* target = this;

    if(!editWhole && selected_part >= 0 && selected_part < children.size()){
        target = children[selected_part];
    }

    Matrix* mat = &(target->Mat);

    switch(key){
		case GLFW_KEY_UP:{
			mat->UpdateView('a',0.0f,0.1f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_DOWN:{
			mat->UpdateView('a',0.0f,-0.1f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_RIGHT:{
			mat->UpdateView('a',0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_LEFT:{
			mat->UpdateView('a',-0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_PAGE_UP:{
			mat->UpdateView('a',0.0f,0.0f,0.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_PAGE_DOWN:{
			mat->UpdateView('a',0.0f,0.0f,-0.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_D:{
			mat->UpdateView('d',10.0f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_F:{
			mat->UpdateView('f',10.0f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_G:{
			mat->UpdateView('g', 1.1f, 1.1f,1.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_H:{
			mat->UpdateView('g', 0.9f, 0.9f,0.9f,CURRENT_AXIS,'W');
			break;
		}
		default:{
			break;
		}
	}
}





// -------------- LISTA DE POSIBLES FIGURAS POLIMORFICAS -------------
// Circulo
Circle::Circle(World* world,const Point &cent,const float &rad,const int &num_seg):ShapeNode(world,GL_TRIANGLES,"Circulo"),center(cent),radius(rad),number_segments(num_seg){
	ModifiedShaderColor(ColorTable[RUST].r,ColorTable[RUST].g,ColorTable[RUST].b);
}

void Circle::Generate(){
	std::vector<float> vectores;
	std::vector<unsigned int> indices;
	
	float angle_between=(2*PI)/number_segments;
	
	unsigned int base = this->world->all_vertices.size()/3;
	unsigned int base_tmp=base+1;
	
	vectores.push_back(center.x);
	vectores.push_back(center.y);
	vectores.push_back(center.z);
	
	vectores.push_back(center.x + radius);
	vectores.push_back(center.y);
	vectores.push_back(center.z);
	
	for(float i=1;i<=number_segments;i++){
		float ang = i * angle_between;
		vectores.push_back(center.x+radius*std::cos(ang));
        vectores.push_back(center.y+radius*std::sin(ang));
        vectores.push_back(0.0f);
        
        indices.push_back(base);
        indices.push_back(base_tmp);
        indices.push_back(++base_tmp);
	}
	
	this->EBOs_range = this->world->Add_Batch(vectores, indices,offset);
	
}

void Circle::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(color.r, color.g, color.b);
	
	glDrawElements(primitive,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
}

