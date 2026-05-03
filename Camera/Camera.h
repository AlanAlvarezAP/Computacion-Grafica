#pragma once

#include "Matrix.h"

const float YAW= -90.0f;
const float PITCH= 0.0f;
const float SPEED =  2.5f;
const float SENSITIVITY= 0.005f;
const float ZOOM= 45.0f;

#define TRANSFORM 180/PI

enum Camera_Mov{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

enum Camera_Status{
	FREE,
	TARGETING
};

class Matrix;


class Camera{
public:
	Point Position;
	Point Front;
	Point OwnUp;
	Point Right;
	Point WorldUp;
	
	float angle_yaw,angle_pitch;
	float mov_speed,mouse_sensi,fov;
	
public:
	Camera(const Point &pos = {0.0f,0.0f,0.8f},const Point& target ={0.0f,0.0f,0.0f},const Point& wUP={0.0f,1.0f,0.0f},Camera_Status stat=TARGETING);
	
	float Normalize(const Point& val1) const;
	float dot(const Point& a, const Point& b);
	Point CrossProduct(const Point& val1,const Point& val2);
	Matrix GetLookAt();
	Matrix GetProjection(float width, float height, float nearP, float farP);
	void ProcessKeyboard(Camera_Mov dir,float dt);
	void ProcessMouse(float xoff,float yoff,bool firstMov=true);
	void ProcessScroll(float yoff);
	void UpdateCam(Camera_Status stat,const Point& new_target={0.0f,0.0f,0.0f},bool newTarg=false);
	
};