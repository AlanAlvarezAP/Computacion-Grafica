#include "Piramid.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Piramid::Piramid(World* world,const Point& cent,const int& amount_sec,const float& rad,const float& heig):ShapeNode(world,0,"Pizza"),center(cent),amount_sector(amount_sec),RADIUS(rad),HEIGHT(heig){}

void Piramid::Generate(){
	float final_sectors_display=360.0f/(float)amount_sector;
	
	const float TRANSFORM=PI/180.0f;
	
	for(float i=0.0f;i<=360.0f;i+=final_sectors_display){
		Point start={
			center.x+RADIUS*std::cos(i*TRANSFORM),
			center.y+RADIUS*std::sin(i*TRANSFORM),
			-HEIGHT/2,
			i*TRANSFORM
		};
		Point end={center.x+RADIUS*std::cos((i+final_sectors_display)*TRANSFORM),
			center.y+RADIUS*std::sin((i+final_sectors_display)*TRANSFORM),
			-HEIGHT/2,
			(i+final_sectors_display)*TRANSFORM
		};
		Sector_Piramid *sec = new Sector_Piramid(world,start,end,center,RADIUS,HEIGHT);
		this->AddChildren(sec);
		sec->Generate();
		
	}
	
	Point new_center=center;
	new_center.z=-new_center.z;
	Base_Piramid *bas= new Base_Piramid(world,new_center,RADIUS,amount_sector,final_sectors_display);
	this->AddChildren(bas);
	bas->Generate();
}

Base_Piramid::Base_Piramid(World* world,const Point& centro,const float& rad,const int &sid,const float& ste):ShapeNode(world,GL_TRIANGLES,"Base"),center(centro),radius(rad),sides(sid),steps(ste),sector_Start(0),lines_Start(0){
	ModifiedShaderColor(ColorTable[ARENA].r,ColorTable[ARENA].g,ColorTable[ARENA].b);
}

void Base_Piramid::Generate(){
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> borderVertex;
	
	vertices.push_back(center.x);
	vertices.push_back(center.y);
	vertices.push_back(center.z);
	
	unsigned int base=this->world->all_vertices.size()/3;
	const float TRANSFORM=PI/180.0f;
	
	for(int i=0;i<=sides;i++){
		float ang=i*steps*TRANSFORM;
		Point punto={
			center.x+radius*std::cos(ang),
			center.y+radius*std::sin(ang),
			center.z,
			ang
		};
		
		vertices.push_back(punto.x);
		vertices.push_back(punto.y);
		vertices.push_back(punto.z);
		
	}
	
	for(int i=0;i<sides;i++){
		indices.push_back(base);
		indices.push_back(base+1+i);
		indices.push_back(base+2+i);
		
		borderVertex.push_back(base+1+i);
		borderVertex.push_back(base+2+i);
	}
	
	
	this->sector_Start=indices.size();
	indices.insert(indices.end(),borderVertex.begin(),borderVertex.end());
	this->EBOs_range = this->world->Add_Batch(vertices, indices,offset);
	this->lines_Start=indices.size()-this->sector_Start;
}

void Base_Piramid::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(color.r, color.g, color.b);
	
	glDrawElements(primitive,EBOs_range.size()-lines_Start,GL_UNSIGNED_INT,(void*)((this->offset)*sizeof(unsigned int)));
	
	this->Shader.SetColor(ColorTable[PERU].r,ColorTable[PERU].g,ColorTable[PERU].b);
	glDrawElements(GL_LINES,EBOs_range.size()-sector_Start,GL_UNSIGNED_INT,(void*)((this->offset+(EBOs_range.size()-lines_Start))*sizeof(unsigned int)));
	
}

Sector_Piramid::Sector_Piramid(World* world,const Point& start_point,const Point& end_point,const Point& centro,const float &rad,const float& heigh):ShapeNode(world,GL_TRIANGLES,"Sector"),startPoint(start_point),endPoint(end_point),center(centro),radius(rad),sector_Start(0),lines_Start(0),height(heigh){
	
	ModifiedShaderColor(ColorTable[ARENA].r,ColorTable[ARENA].g,ColorTable[ARENA].b);
}

void Sector_Piramid::Generate(){
	std::vector<float> vectores;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> borderVertex;
	
	unsigned int base = this->world->all_vertices.size()/3;
	
	vectores.push_back(center.x);
	vectores.push_back(center.y);
	vectores.push_back(center.z);
	
	
	vectores.push_back(startPoint.x);
	vectores.push_back(startPoint.y);
	vectores.push_back(startPoint.z);
	
	vectores.push_back(endPoint.x);
	vectores.push_back(endPoint.y);
	vectores.push_back(endPoint.z);
	
	indices.push_back(base);
	indices.push_back(base+1);
	indices.push_back(base+2);
	
	borderVertex.push_back(base);
	borderVertex.push_back(base+1);

	borderVertex.push_back(base);
	borderVertex.push_back(base+2);

		
	this->sector_Start=indices.size();
	indices.insert(indices.end(),borderVertex.begin(),borderVertex.end());
	this->EBOs_range = this->world->Add_Batch(vectores, indices,offset);
	this->lines_Start=indices.size()-this->sector_Start;
}

void Sector_Piramid::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(color.r, color.g, color.b);
	
	glDrawElements(primitive,EBOs_range.size()-lines_Start,GL_UNSIGNED_INT,(void*)((this->offset)*sizeof(unsigned int)));
	
	this->Shader.SetColor(ColorTable[PERU].r,ColorTable[PERU].g,ColorTable[PERU].b);
	glDrawElements(GL_LINES,EBOs_range.size()-sector_Start,GL_UNSIGNED_INT,(void*)((this->offset+(EBOs_range.size()-lines_Start))*sizeof(unsigned int)));
}