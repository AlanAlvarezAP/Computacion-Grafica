#pragma once

#include "config.h"

const float PI= 3.14159265358979323846f;

class Matrix{
public:
	std::vector<float> matrix;
	char type;
	float pos_x,pos_y,pos_z;
	
	Matrix(){
		matrix.assign(16, 0.0f);
		matrix[0]= matrix[5] = matrix[10] = matrix[15] = 1.0f;
		pos_x=pos_y=pos_z=0.0f;
	}
	
	void Restart_Identity(std::vector<float> &matriz){
		matriz.assign(16, 0.0f);
		matriz[0]= matriz[5] = matriz[10] = matriz[15] = 1.0f;
	}
	
	Matrix operator*(Matrix &v2) const{
		Matrix result;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				float acum=0.0f;
				for(int k=0;k<4;k++){
					acum += v2.matrix[k*4 + i] * matrix[j*4 + k];
				}
				result.matrix[j*4 + i] = acum;
			}
		}
		return result;
	}
	
	Matrix& operator=(const Matrix& v2){
		this->matrix=v2.matrix;
		this->type=v2.type;
		this->pos_x=v2.pos_x;
		this->pos_y=v2.pos_y;
		this->pos_z=v2.pos_z;
		return *this;
	}
	
	
	void PrintMatrix(){
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				std::cout << matrix[j*4+i] << " ";
			}
			std::cout << std::endl;
		}
	}
	
	void translate_norm(float first_val,float second_val,float third_val){
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		tmp.matrix[12] = first_val;
		tmp.matrix[13] = second_val;
		tmp.matrix[14] = third_val;
		
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void translate_inv(float first_val,float second_val,float third_val){
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		
		tmp.matrix[12]=-first_val;
		tmp.matrix[13]=-second_val;
		tmp.matrix[14]=-third_val;
		
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void rotate_norm(float ang,char axis){
		ang*=PI/180.0f;
		
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		
		switch(axis){
			case 'x':{
				tmp.matrix[5]=std::cos(ang);
				tmp.matrix[6]=-std::sin(ang);
				tmp.matrix[9]=std::sin(ang);
				tmp.matrix[10]=std::cos(ang);
				break;
			}
			case 'y':{
				tmp.matrix[0]=std::cos(ang);
				tmp.matrix[2]=std::sin(ang);
				tmp.matrix[8]=-std::sin(ang);
				tmp.matrix[10]=std::cos(ang);
				break;
			}
			case 'z':{
				tmp.matrix[0]=std::cos(ang);
				tmp.matrix[1]=-std::sin(ang);
				tmp.matrix[4]=std::sin(ang);
				tmp.matrix[5]=std::cos(ang);
				break;
			}
			default:{
				std::cout << "Axis desconocido :( " << std::endl;
				break;
			}
		}
		
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void rotate_inv(float ang,char axis){
		ang*=PI/180.0f;
		
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		
		switch(axis){
			case 'x':{
				tmp.matrix[5]=std::cos(ang);
				tmp.matrix[6]=std::sin(ang);
				tmp.matrix[9]=-std::sin(ang);
				tmp.matrix[10]=std::cos(ang);
				break;
			}
			case 'y':{
				tmp.matrix[0]=std::cos(ang);
				tmp.matrix[2]=-std::sin(ang);
				tmp.matrix[8]=std::sin(ang);
				tmp.matrix[10]=std::cos(ang);
				break;
			}
			case 'z':{
				tmp.matrix[0]=std::cos(ang);
				tmp.matrix[1]=std::sin(ang);
				tmp.matrix[4]=-std::sin(ang);
				tmp.matrix[5]=std::cos(ang);
				break;
			}
			default:{
				std::cout << "Axis desconocido :( " << std::endl;
				break;
			}
		}
		
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void scale_norm(float first_val,float second_val,float third_val){
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		
		tmp.matrix[0]=first_val;
		tmp.matrix[5]=second_val;
		tmp.matrix[10]=third_val;
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void scale_inv(float first_val,float second_val,float third_val){
		Matrix tmp;
		tmp.Restart_Identity(tmp.matrix);
		
		tmp.matrix[0]=1/first_val;
		tmp.matrix[5]=1/second_val;
		tmp.matrix[10]=1/third_val;
		tmp=(*this)*tmp;
		matrix=tmp.matrix;
	}
	
	void UpdateView(char tpe,float first_val,float second_val=0.0f,float third_val=0.0f,char axis='z'){
		this->type=tpe;
		switch(type){
			case 'a':{
				pos_x+=first_val;
				pos_y+=second_val;
				pos_z+=third_val;
				translate_norm(first_val,second_val,third_val);
				break;
			}
			case 's':{
				translate_inv(first_val,second_val,third_val);
				break;
			}
			case 'd':{
				translate_inv(pos_x, pos_y,pos_z);
				rotate_norm(first_val, axis);
				translate_norm(pos_x,pos_y,pos_z);
				break;
			}
			case 'f':{
				translate_inv(pos_x, pos_y,pos_z);
				rotate_inv(first_val, axis);
				translate_norm(pos_x,pos_y,pos_z);
				break;
			}
			case 'g':{
				translate_inv(pos_x, pos_y,pos_z);
				scale_norm(first_val, second_val,third_val);
				translate_norm(pos_x,pos_y,pos_z);
				break;
			}
			case 'h':{
				translate_inv(pos_x, pos_y,pos_z);
				scale_inv(first_val, second_val,third_val);
				translate_norm(pos_x,pos_y,pos_z);
				break;
			}
			default:{
				std::cout << "Caso desconocido al actualizar view :( " << std::endl;
				break;
			}
		}
	}
	
	
	
};