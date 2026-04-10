#pragma once

#include "config.h"

const float PI= 3.14159265358979323846f;

class Matrix{
public:
	std::vector<float> matrix;
	char type;
	
	Matrix(){
		matrix.assign(16, 0.0f);
		matrix[0]= matrix[5] = matrix[10] = matrix[15] = 1.0f;
	}
	
	void Restart_Identity(std::vector<float> &matriz){
		matriz.assign(16, 0.0f);
		matriz[0]= matriz[5] = matriz[10] = matriz[15] = 1.0f;
	}
	
	// Recordar v1 es el global y v2 es el modificado
	void matrixMulti(std::vector<float> &v1,std::vector<float> &v2){
		std::vector<float> result(v1.size());
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				float acum=0.0f;
				for(int k=0;k<4;k++){
					acum+=v1[k*4+i]*v2[j*4+k];
				}
				result[j*4+i]=acum;
			}
		}
		v1=std::move(result);
	}
	
	
	void translate_norm(float first_val,float second_val){
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		tmp[12] = first_val;
		tmp[13] = second_val;
		
		matrixMulti(matrix,tmp);
	}
	
	void translate_inv(float first_val,float second_val){
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		
		tmp[12]=-first_val;
		tmp[13]=-second_val;
		
		matrixMulti(matrix,tmp);
	}
	
	void rotate_norm(float ang){
		ang*=PI/180.0f;
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		
		tmp[0]=std::cos(ang);
		tmp[1]=std::sin(ang);
		tmp[4]=-std::sin(ang);
		tmp[5]=std::cos(ang);
		
		matrixMulti(matrix,tmp);
	}
	
	void rotate_inv(float ang){
		ang*=PI/180.0f;
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		
		tmp[0]=std::cos(ang);
		tmp[1]=-std::sin(ang);
		tmp[4]=std::sin(ang);
		tmp[5]=std::cos(ang);
		
		matrixMulti(matrix,tmp);
	}
	
	void scale_norm(float first_val,float second_val){
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		
		tmp[0]=first_val;
		tmp[5]=second_val;
		
		matrixMulti(matrix,tmp);
	}
	
	void scale_inv(float first_val,float second_val){
		std::vector<float> tmp(16);
		Restart_Identity(tmp);
		
		tmp[0]=1/first_val;
		tmp[5]=1/second_val;
		
		matrixMulti(matrix,tmp);
	}
	
	void UpdateView(char tpe,float first_val,float second_val=0.0f){
		this->type=tpe;
		switch(type){
			case 'a':{
				translate_norm(first_val,second_val);
				break;
			}
			case 's':{
				translate_inv(first_val,second_val);
				break;
			}
			case 'd':{
				rotate_norm(first_val);
				break;
			}
			case 'f':{
				rotate_inv(first_val);
				break;
			}
			case 'g':{
				scale_norm(first_val,second_val);
				break;
			}
			case 'h':{
				scale_inv(first_val,second_val);
				break;
			}
			default:{
				std::cout << "Caso desconocido al actualizar view :( " << std::endl;
				break;
			}
		}
	}
	
	
	
};